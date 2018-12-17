#include "Animation.h"
#include <assert.h>

void Animation::advance()
{
	assert(spritePtr != nullptr);
	++currentFrame;
	if (currentFrame > (frames.size()-1))
	{
		currentFrame = 0;
	}
}

Animation::Animation(int x, int y, int width, int height, int numberX, int numberY, float holdTime, const Surface* spritePtr_in, Color chroma)
	:
	holdTime (holdTime),
	chroma (chroma),
	spritePtr(spritePtr_in)
{
	for (int j = 0; j < numberY; ++j)
	{
		for (int i = 0; i < numberX; ++i)
		{
			frames.push_back(RectI(x + i * width, x + (i + 1) * width, y + j * height, y + (j + 1)*height));
		}
	}
	
}

Animation::Animation()

{
	spritePtr = nullptr;
}

void Animation::customInit(int x, int y, int width, int height, int numberX, 
	int numberY, float holdTime_in, const Surface * spritePtr_in, Color chroma_in, 
	int startingFrame)
{
	holdTime = holdTime_in;
	chroma = chroma_in;
	spritePtr = spritePtr_in;
	currentFrame = startingFrame;

	for (int j = 0; j < numberY; ++j)
	{
		for (int i = 0; i < numberX; ++i)
		{
			frames.push_back(RectI(x + i * width, x + (i + 1) * width, y + j * height, y + (j + 1)*height));
		}
	}
}



void Animation::update(float dt)
{
	assert(spritePtr != nullptr);
	currentFrameTime += dt;
	while (currentFrameTime >= holdTime)
	{
		advance();
		currentFrameTime -= holdTime;
	}
}

void Animation::draw(const Vec2 pos, Graphics & gfx) const
{
	assert(spritePtr != nullptr);
	gfx.drawSprite(*spritePtr, frames[currentFrame], int(pos.x), int(pos.y), chroma);
}
