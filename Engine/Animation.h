#pragma once
#include "Surface.h"
#include "RectI.h"
#include "Graphics.h"
#include<vector>


class Animation
{
private:
	std::vector<RectI> frames;
	const Surface* spritePtr;
	Color chroma;
	int currentFrame = 0;
	float currentFrameTime = 0;
	float holdTime;
	void advance();
	bool sleeping;

public:
	Animation(int x_start, int y_start, int width, int height,
		int numberX, int numberY, float holdTime, const Surface* spritePtr_in, 
		Color chroma);
	Animation();
	void customInit(int x_start, int y_start, int width, int height,
		int numberX, int numberY, float holdTime, const Surface* spritePtr_in,
		Color chroma, int startingFrame);
	void update(float dt);
	void draw(const Vec2 pos, Graphics& gfx) const;
};