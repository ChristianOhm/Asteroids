/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

Graphics::Graphics( HWNDKey& key )
{
	assert( key.hWnd != nullptr );

	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = key.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif
	
	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating device and swap chain" );
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		(LPVOID*)&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Getting back buffer" );
	}

	// create a view on backbuffer that we can render to
	if( FAILED( hr = pDevice->CreateRenderTargetView( 
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating render target view on backbuffer" );
	}


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets( 1,pRenderTargetView.GetAddressOf(),nullptr );


	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float( Graphics::ScreenWidth );
	vp.Height = float( Graphics::ScreenHeight );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1,&vp );


	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc,nullptr,&pSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sysbuffer texture" );
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
		&srvDesc,&pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating view on sysBuffer texture" );
	}


	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof( FramebufferShaders::FramebufferPSBytecode ),
		nullptr,
		&pPixelShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating pixel shader" );
	}
	

	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		nullptr,
		&pVertexShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex shader" );
	}
	

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( FSQVertex ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if( FAILED( hr = pDevice->CreateBuffer( &bd,&initData,&pVertexBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex buffer" );
	}

	
	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateInputLayout( ied,2,
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		&pInputLayout ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating input layout" );
	}


	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc,&pSamplerState ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sampler state" );
	}

	// allocate memory for sysbuffer (16-byte aligned for faster access)
	pSysBuffer = reinterpret_cast<Color*>( 
		_aligned_malloc( sizeof( Color ) * Graphics::ScreenWidth * Graphics::ScreenHeight,16u ) );
}

Graphics::~Graphics()
{
	// free sysbuffer memory (aligned free)
	if( pSysBuffer )
	{
		_aligned_free( pSysBuffer );
		pSysBuffer = nullptr;
	}
	// clear the state of the device context before destruction
	if( pImmediateContext ) pImmediateContext->ClearState();
}

		


void Graphics::drawCircle(int xPos, int yPos, int outerRadius, int innerRadius, Color c)
{

	int rSquareInner = innerRadius * innerRadius;
	int rSquareOuter = outerRadius * outerRadius;

	for (int x = xPos - outerRadius; x <= xPos + outerRadius; ++x)
	{
		for (int y = yPos - outerRadius; y <= yPos + outerRadius; ++y)
		{
			int a = xPos - x;
			int b = y - yPos;
			int sumSquares = a * a + b * b;
			if (sumSquares <= rSquareOuter && sumSquares >= rSquareInner)
			{
				PutPixel(x, y, c);
			}
		}
	}
}

void Graphics::DrawRect(int x0, int y0, int x1, int y1, Color c)
{
	if (x0 > x1)
	{
		std::swap(x0, x1);
	}
	if (y0 > y1)
	{
		std::swap(y0, y1);
	}

	for (int y = y0; y < y1; ++y)
	{
		for (int x = x0; x < x1; ++x)
		{
			PutPixel(x, y, c);
		}
	}
}

Color Graphics::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < int(Graphics::ScreenWidth));
	assert(y >= 0);
	assert(y < int(Graphics::ScreenHeight));
	return pSysBuffer[Graphics::ScreenWidth * y + x];
}

void Graphics::DrawRect(Vec2 & upperLeft, Vec2 & lowerRight, Color c)
{
	DrawRect(int(upperLeft.x), int(upperLeft.y), int(lowerRight.x), int(lowerRight.y), c);
}

void Graphics::DrawRect(Vec2 & upperLeft, Vec2 & lowerRight, float padding, Color c)
{
	DrawRect(int(upperLeft.x + padding), int(upperLeft.y + padding), int(lowerRight.x - padding), int(lowerRight.y - padding), c);
}

void Graphics::DrawRect(Vec2 & pos, float halfWidth, float halfHight, Color c)
{
	DrawRect(int(pos.x + halfWidth), int(pos.y + halfHight), int(pos.x - halfWidth), int(pos.y - halfHight), c);


}
void Graphics::drawSpriteNonChroma(const Surface & surface, int x_off, int y_off)
{
	drawSpriteNonChroma(surface, surface.getRect(), getScreenRect(), x_off, y_off);

}

void Graphics::drawSpriteNonChroma(const Surface & surface, const RectI & sourceRect, int x_off, int y_off)
{

	drawSpriteNonChroma(surface, sourceRect, getScreenRect(), x_off, y_off);
}

void Graphics::drawSpriteNonChroma(const Surface & surface, RectI sourceRect, const RectI & clipRect, int x_off, int y_off)
{
	assert(sourceRect.left >= 0);
	assert(sourceRect.top >= 0);
	assert(sourceRect.right <= surface.getWidth());
	assert(sourceRect.bottom <= surface.getHeight());

	if (x_off < clipRect.left)
	{
		sourceRect.left += (clipRect.left - x_off);
		x_off = clipRect.left;
	}
	if (y_off < clipRect.top)
	{
		sourceRect.top += (clipRect.top - y_off);
		y_off = clipRect.top;
	}
	if ((x_off + sourceRect.getWidth() > clipRect.right))
	{
		sourceRect.right -= (sourceRect.getWidth() - clipRect.right + x_off);
	}
	if ((y_off + sourceRect.getHeight() > clipRect.bottom))
	{
		sourceRect.bottom -= (sourceRect.getHeight() - clipRect.bottom + y_off);
	}

	for (int x = sourceRect.left; x < sourceRect.right; ++x)
	{
		for (int y = sourceRect.top; y < sourceRect.bottom; ++y)
		{
			PutPixel(x + x_off - sourceRect.left, y + y_off - sourceRect.top, surface.getPixel(x, y));
		}
	}

}

void Graphics::drawSprite(const Surface & surface, RectI sourceRect, const RectI & clipRect, int x_off, int y_off, Color chroma)
{
	assert(sourceRect.left >= 0);
	assert(sourceRect.top >= 0);
	assert(sourceRect.right <= surface.getWidth());
	assert(sourceRect.bottom <= surface.getHeight());

	if (x_off < clipRect.left)
	{
		sourceRect.left += (clipRect.left - x_off);
		x_off = clipRect.left;
	}
	if (y_off < clipRect.top)
	{
		sourceRect.top += (clipRect.top - y_off);
		y_off = clipRect.top;
	}
	if ((x_off + sourceRect.getWidth() > clipRect.right))
	{
		sourceRect.right -= (sourceRect.getWidth() - clipRect.right + x_off);
	}
	if ((y_off + sourceRect.getHeight() > clipRect.bottom))
	{
		sourceRect.bottom -= (sourceRect.getHeight() - clipRect.bottom + y_off);
	}

	for (int x = sourceRect.left; x < sourceRect.right; ++x)
	{
		for (int y = sourceRect.top; y < sourceRect.bottom; ++y)
		{
			Color pixel = surface.getPixel(x, y);
			if (pixel != chroma)
			{
				PutPixel(x + x_off - sourceRect.left, y + y_off - sourceRect.top, pixel);
			}

		}
	}

}

void Graphics::drawSprite(const Surface & surface, int x, int y, Color chroma)
{
	drawSprite(surface, surface.getRect(), getScreenRect(), x, y, chroma);
}

void Graphics::drawSprite(const Surface & surface, RectI sourceRect, int x, int y, Color chroma)
{
	drawSprite(surface, sourceRect, getScreenRect(), x, y, chroma);
}

RectI Graphics::getScreenRect() const
{
	return RectI(Vei2(0, 0), ScreenWidth, ScreenHeight);
}
void Graphics::drawSprite(const Surface & surface, const Color & substitute, RectI sourceRect, const RectI & clipRect, int x_off, int y_off, Color chroma)
{
	assert(sourceRect.left >= 0);
	assert(sourceRect.top >= 0);
	assert(sourceRect.right <= surface.getWidth());
	assert(sourceRect.bottom <= surface.getHeight());

	if (x_off < clipRect.left)
	{
		sourceRect.left += (clipRect.left - x_off);
		x_off = clipRect.left;
	}
	if (y_off < clipRect.top)
	{
		sourceRect.top += (clipRect.top - y_off);
		y_off = clipRect.top;
	}
	if ((x_off + sourceRect.getWidth() > clipRect.right))
	{
		sourceRect.right -= (sourceRect.getWidth() - clipRect.right + x_off);
	}
	if ((y_off + sourceRect.getHeight() > clipRect.bottom))
	{
		sourceRect.bottom -= (sourceRect.getHeight() - clipRect.bottom + y_off);
	}

	for (int x = sourceRect.left; x < sourceRect.right; ++x)
	{
		for (int y = sourceRect.top; y < sourceRect.bottom; ++y)
		{
			Color pixel = surface.getPixel(x, y);
			if (pixel != chroma)
			{
				PutPixel(x + x_off - sourceRect.left, y + y_off - sourceRect.top, substitute);
			}

		}
	}

}

void Graphics::drawSprite(const Surface & surface, const Color & substitute, int x, int y, Color chroma)
{
	drawSprite(surface, substitute, surface.getRect(), getScreenRect(), x, y, chroma);
}

void Graphics::drawSprite(const Surface & surface, const Color & substitute, RectI sourceRect, int x, int y, Color chroma)
{
	drawSprite(surface, substitute, sourceRect, getScreenRect(), x, y, chroma);
}


void Graphics::drawSpriteRotate(const Surface & surface, RectI sourceRect, const RectI & clipRect, int x_off, int y_off, float angle, Color chroma)
{

	int diagonalHalf = (int)(std::sqrt(sourceRect.getHeight()*sourceRect.getHeight() + sourceRect.getWidth() * sourceRect.getWidth())) / 2;
	Vei2 centerOfRotation = sourceRect.GetCenter();
	x_off -= centerOfRotation.x;
	y_off -= centerOfRotation.y;
	RectI targetRect = RectI::FromCenter(centerOfRotation, diagonalHalf, diagonalHalf);

	if (targetRect.left + x_off < clipRect.left)
	{
		targetRect.left = clipRect.left - x_off;
	}

	if (targetRect.right + x_off > clipRect.right)
	{
		targetRect.right = clipRect.right - x_off;
	}

	if (targetRect.top + y_off < clipRect.top)
	{
		targetRect.top = clipRect.top - y_off;
	}

	if (targetRect.bottom + y_off > clipRect.bottom)
	{
		targetRect.bottom = clipRect.bottom - y_off;
	}

	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	int halfHeight = (int)sourceRect.getHeight() / 2;
	int halfWidth = (int)sourceRect.getWidth() / 2;
	for (int x = targetRect.left; x < targetRect.right; ++x)
	{
		for (int y = targetRect.top; y < targetRect.bottom; ++y)
		{
			int xTrans = (int)((x - centerOfRotation.x) * cosAngle + (y - centerOfRotation.y) * sinAngle) + halfWidth;
			int yTrans = (int)((y - centerOfRotation.y) * cosAngle - (x - centerOfRotation.x) * sinAngle) + halfHeight;

			if (sourceRect.Contains(Vei2(xTrans, yTrans)))
			{
				Color pixel = surface.getPixel(xTrans, yTrans);
				if (pixel != chroma)
				{
					PutPixel(x + x_off, y + y_off, pixel);
				}

			}

		}

	}


}

void Graphics::drawSpriteGhost(const Surface & surface, RectI sourceRect, const RectI & clipRect, int x_off, int y_off, Color chroma)
{
	assert(sourceRect.left >= 0);
	assert(sourceRect.top >= 0);
	assert(sourceRect.right <= surface.getWidth());
	assert(sourceRect.bottom <= surface.getHeight());

	if (x_off < clipRect.left)
	{
		sourceRect.left += (clipRect.left - x_off);
		x_off = clipRect.left;
	}
	if (y_off < clipRect.top)
	{
		sourceRect.top += (clipRect.top - y_off);
		y_off = clipRect.top;
	}
	if ((x_off + sourceRect.getWidth() > clipRect.right))
	{
		sourceRect.right -= (sourceRect.getWidth() - clipRect.right + x_off);
	}
	if ((y_off + sourceRect.getHeight() > clipRect.bottom))
	{
		sourceRect.bottom -= (sourceRect.getHeight() - clipRect.bottom + y_off);
	}

	for (int x = sourceRect.left; x < sourceRect.right; ++x)
	{
		for (int y = sourceRect.top; y < sourceRect.bottom; ++y)
		{
			Color pixel = surface.getPixel(x, y);
			if (pixel != chroma)
			{
				Color dst = GetPixel(x + x_off - sourceRect.left, y + y_off - sourceRect.top);
				Color blend = Color
					(
						(dst.GetR() + pixel.GetR()) / 2,
						(dst.GetG() + pixel.GetG()) / 2,
						(dst.GetB() + pixel.GetB()) / 2
					);
		

				PutPixel(x + x_off - sourceRect.left, y + y_off - sourceRect.top, blend);
			}

		}
	}

}


void Graphics::EndFrame()
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
		D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
	}
	// setup parameters for copy operation
	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData );
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( Color );
	const size_t srcPitch = Graphics::ScreenWidth;
	const size_t rowBytes = srcPitch * sizeof( Color );
	// perform the copy line-by-line
	for( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
	{
		memcpy( &pDst[ y * dstPitch ],&pSysBuffer[y * srcPitch],rowBytes );
	}
	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
	pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u,0u );

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(),L"Presenting back buffer [device removed]" );
		}
		else
		{
			throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
		}
	}
}

void Graphics::BeginFrame()
{
	// clear the sysbuffer
	memset( pSysBuffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

void Graphics::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	pSysBuffer[Graphics::ScreenWidth * y + x] = c;
}


//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line )
	:
	ChiliException( file,line,note ),
	hr( hr )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Chili Graphics Exception";
}