#pragma once
#include "Graphics.h"
#include "Vec2.h"

class Sprites
{
public:
	static void drawRocket(Graphics& gfx, const Vec2& pos, float angle);
	static void drawLargeAsteroid(Graphics& gfx, const Vec2& pos);
	static void drawMediumAsteroid(Graphics& gfx, const Vec2& pos);
	static void drawTinyAsteroid(Graphics& gfx, const Vec2& pos);
	static void drawAlienShip(Graphics& gfx, const Vec2& pos);
	static void drawGetReady(Graphics& gfx, const Vec2& pos);
	static void drawLeavingSector(Graphics& gfx, const Vec2 pos);
	static void drawGameOver(Graphics& gfx, const Vec2 pos);
	static void drawBluePowerUp(Graphics& gfx, const Vec2 pos);
	static void drawGreenPowerUp(Graphics& gfx, const Vec2 pos);
	static void drawRedPowerUp(Graphics& gfx, const Vec2 pos);
	static void drawYellowPowerUp(Graphics& gfx, const Vec2 pos);
};