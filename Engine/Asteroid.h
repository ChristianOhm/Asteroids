#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "Sprites.h"
#include <random>
#include "Rocket.h"




class Asteroid
{
public:
	enum class Size
	{
		tiny,
		medium,
		large,
		virtualAsteroid
	};
	void init(Vec2 pos_in, Vec2 direction_in, Size size_in, int identifier_in, float speedModifier);
	void draw(Graphics& gfx);
	bool destroyed = true;
	bool updatePos(float dt);
	bool collideAsteroid(Asteroid& otherAsteroid);
	Vec2 getPos();
	Vec2 getDirection();
	void setDirection(Vec2 newDirection);
	float getSpeed();
	float getRadius();
	float getMass();
	void bounceFromAsteroid(Asteroid& otherAsteroid);
	void bounceFromRocket(Rocket& rocket);
	int hitpoints;
	static constexpr float radius_tiny = 13.0f;
	static constexpr float radius_medium = 25.0f;
	static constexpr float radius_large = 41.0f;
	Size size;
	bool checkBulletCollision(Vec2& posBullet);
	int getIdentifier();
	int getLastCollision();
	void SetLastCollision(int identifier);
	bool collideAlien(const Vec2& alienPos, float alienWidth, float alienHeight);
	int damageOnHit;
	

private:
	Vec2 pos;
	Vec2 direction;
	int identifier;
	int lastCollision;
	float speed;
	float radius;
	float mass;








};