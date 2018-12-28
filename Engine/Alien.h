#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "Field.h"
#include "Timer.h"
#include <random>
#include"PDVec.h"
#include"Rocket.h"
#include "PowerUp.h"
#include "Surface.h"
#include"Animation.h"


class Alien
{
private:
	Vec2 pos;
	float timeSinceSpawn = 0.0f;
	const Surface* alien;
	Animation animation;

	static constexpr int maxHitpoints = 3;
	static constexpr int bulletsPerSalve = 5;
	static constexpr float fireDelay = 0.1f;
	static constexpr float salveDelay = 1.0f;
	int timesFiredSalve = 0;
	float fireCooldown = 0.0f;
	float salveCooldown = 0.0f;
	bool duringSalve = false;
	float timeTillSpawn = 10.0f;
	void spawn(std::mt19937& rng);
	bool testShoot(float dt);

public:
	Alien(const Surface* alien);
	static constexpr float halfWidth = 26;
	static constexpr float halfHeight = 12;
	static constexpr float speed = 80;
	static constexpr float safetyX = 50;
	static constexpr float safetyY = 50;
	static constexpr int damageOnHit = 20;
	bool underway = false;
	void draw(Graphics& gfx);
	void updatePos(bool collisionAhead, Vec2& evadeDirection, float dt);
	bool testSpawn(float dt);
	Vec2 getPos();
	int hitpoints;
	
	bool checkBulletHit(Vec2& posBulet);
	void checkHitpoints(PowerUp& powerUp, Timer& timer, std::mt19937 & rng);
	void setSpawnTime(float newSpawnTime);
	void update(Field& field, float dt, Timer& timer, std::mt19937& rng, Rocket& rocket, PowerUp& powerUp);
	bool shoot(const Vec2 target, const float dt, PDVec& bulletPD );
};
