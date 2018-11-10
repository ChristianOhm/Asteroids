#pragma once
#include "Asteroid.h"
#include "Vec2.h"
#include <random>
#include "Graphics.h"
#include "Sprites.h"
#include "SoundEffect.h"
#include "Rocket.h"
#include <vector>

class Field
{
private:
	static constexpr int maxAsteroids = 50;
	Asteroid asteroids[maxAsteroids];

	void doAsteroidDestruction(Asteroid& asteroid);
	void generateNewAsteroid(Vec2& pos, Vec2& direction, Asteroid::Size& size);
	void checkAsteroidsHitpoints();
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_real_distribution <float> angle{ 0, 359 };
	const float  convert = 3.14159265358979f / 180.0f;
	SoundEffect collisionSound;
	SoundEffect wallCollideSound;
	static constexpr float asteroidFreeZoneRadiusSq = 10000;
	float speedModifier = 1;
	float asteroidMinDistSq = 6724;



public:
	Field();
	void updateAsteroids(float dt);
	void draw(Graphics& gfx);
	bool checkHit(Vec2& posBullet);
	void testRocketAsteroidCollision(Rocket& rocket, Timer& timer);
	void checkAlienPath(const Vec2& alienPos, bool& collisionAhead, Vec2& evadeDirection);
	bool testAlienFieldCollision(const Vec2& alienPos);
	void initLevel(std::mt19937& rng, int level);
	bool levelComplete();
};