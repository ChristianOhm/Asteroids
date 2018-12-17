#pragma once
#include "Graphics.h"
#include "Vec2.h"

class Sprites
{
public:
	static void drawRocket(Graphics& gfx, const Vec2& pos, float angle);
	static void drawBluePowerUp(Graphics& gfx, const Vec2 pos);
	static void drawGreenPowerUp(Graphics& gfx, const Vec2 pos);
	static void drawRedPowerUp(Graphics& gfx, const Vec2 pos);
	static void drawYellowPowerUp(Graphics& gfx, const Vec2 pos);
};