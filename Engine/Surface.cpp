#include "Surface.h"
#include "assert.h"
#include <fstream>
#include "ChiliWin.h"

Surface::Surface(int width_in, int height_in)
	:
	height (height_in),
	width (width_in)
{
	pColor = new Color[width*height];
}

Surface::Surface(std::string filename)
{
	std::ifstream bmFile(filename, std::ios::binary);
	assert(bmFile);
	BITMAPFILEHEADER bmFileHeader;
	bmFile.read(reinterpret_cast<char*> (&bmFileHeader), sizeof(bmFileHeader));
	BITMAPINFOHEADER bmInfoHeader;
	bmFile.read(reinterpret_cast<char*> (&bmInfoHeader), sizeof(bmInfoHeader));
	assert(bmInfoHeader.biCompression == BI_RGB);
	assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32);
	bool is32Bit;
	int padding;
	width = bmInfoHeader.biWidth;
	if (bmInfoHeader.biBitCount == 32)
	{
		is32Bit = true;
		padding = 0;
	}

	else
	{
		is32Bit = false;
		padding = (4 - (width * 3) % 4) % 4;
	}
	int startCount, endCount, increment;
	if (bmInfoHeader.biHeight < 0)
	{
		height = -bmInfoHeader.biHeight;
		startCount = 0;
		endCount = height;
		increment = 1;

	}

	else
	{
		height = bmInfoHeader.biHeight;
		startCount = height - 1;
		endCount = 0;
		increment = -1;
	}

	
	pColor = new Color[width*height];

	bmFile.seekg(bmFileHeader.bfOffBits);
	for (int y = startCount; y != endCount; y+=increment)
	{
		for (int x = 0; x < width; ++x)
		{
			char r, g, b, c;
			b = bmFile.get();
			g = bmFile.get();
			r = bmFile.get();
			if (is32Bit)
			{
				c = bmFile.get();
			}
			putPixel(x, y, Color( r, g, b ));
		}
		bmFile.seekg(padding, std::ios::cur);
	}
}

Surface::Surface(const Surface& rhs)
	:
	Surface (rhs.width, rhs.height)

{
	int nPixels = width * height;
	for (int counter = 0; counter <= nPixels; ++counter)
	{
		pColor[counter] = rhs.pColor[counter];
	}
	
}

Surface & Surface::operator=(const Surface & rhs)
{

	width = rhs.width;
	height = rhs.height;
	delete[] pColor;
	pColor = new Color[width*height];
	int nPixels = width * height;
	for (int counter = 0; counter <= nPixels; ++counter)
	{
		pColor[counter] = rhs.pColor[counter];
	}
	return *this;
}

Surface::~Surface()
{
	delete[] pColor;
	pColor = nullptr;
}

int Surface::getWidth() const
{
	return width;
}

int Surface::getHeight() const
{
	return height;
}

void Surface::putPixel(int x, int y, Color c)
{
	assert (x >= 0);
	assert (x < width);
	assert (y >= 0);
	assert (y < height);
	pColor[y*width + x] = c;
}

const Color Surface::getPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	return pColor[y*width + x];
}

RectI Surface::getRect() const
{
	return RectI(Vei2(0, 0), width, height);
}
