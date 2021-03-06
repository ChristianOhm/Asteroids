#pragma once
#include "Asteroid.h"
#include "Vec2.h"
#include <random>
#include "Graphics.h"
#include "Rocket.h"
#include <vector>
#include "Sprites2.h"
#include "Scorecounter.h"

class Field
{
private:
	const Sprites2& sprites2;
	std::vector<Asteroid> asteroids;
	void doAsteroidDestruction(Asteroid& asteroid);
	void deleteAsteroid(int index);
	void generateNewAsteroid(Vec2& pos, Vec2& direction, Asteroid::Size size);
	void checkAsteroidsHitpoints(Scorecounter& scorecounter);
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_real_distribution <float> angle{ 0, 359 };
	const float  convert = 3.14159265358979f / 180.0f;
	static constexpr float asteroidFreeZoneRadiusSq = 10000;
	float speedModifier = 1;
	float asteroidMinDistSq = 6724;
	int scorepoints[3] = { 10, 50, 200 };



public:
	Field(const Sprites2& sprites2);
	void updateAsteroids(float dt, Scorecounter& scorecounter);
	void draw(Graphics& gfx);
	bool checkHit(Vec2& posBullet);
	void testRocketAsteroidCollision(Rocket& rocket, Timer& timer);
	void checkAlienPath(const Vec2& alienPos, bool& collisionAhead, Vec2& evadeDirection);
	bool testAlienFieldCollision(const Vec2& alienPos);
	void initLevel(std::mt19937& rng, int level);
	bool levelComplete();
};