#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include <random>
#include "Rocket.h"
#include "Surface.h"
#include "Sprites2.h"
#include "Animation.h"



class Asteroid
{
public:
	enum class Size
	{
		tiny,
		medium,
		large,
	};
	Asteroid (const Vec2 pos_in, const Vec2 direction_in, const Size size_in, const int identifier_in, 
		const float speedModifier, const Sprites2& sprites2, std::mt19937& rng);
	void draw(Graphics& gfx) const;
	bool updatePos(float dt);
	bool collideAsteroid(Asteroid& otherAsteroid);
	Vec2 getPos() const;
	Vec2 getDirection() const;
	void setDirection(Vec2 newDirection);
	float getSpeed() const;
	float getRadius() const;
	float getMass() const;
	void bounceFromAsteroid(Asteroid& otherAsteroid);
	void bounceFromRocket(Rocket& rocket);
	int hitpoints;
	static constexpr float radius_tiny = 12.5f;
	static constexpr float radius_medium = 25.0f;
	static constexpr float radius_large = 45.0f;
	Size size;
	bool checkBulletCollision(Vec2& posBullet);
	int getIdentifier();
	int getLastCollision();
	void SetLastCollision(int identifier);
	bool collideAlien(const Vec2& alienPos, float alienWidth, float alienHeight) const;
	int damageOnHit;
	

private:
	Vec2 pos;
	Vec2 drawOffset;
	Vec2 direction;
	int identifier;
	int lastCollision;
	float speed;
	float radius;
	float mass;
	Animation animation;







};