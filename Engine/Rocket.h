#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include"Keyboard.h"
#include "PDVec.h"
#include "Timer.h"
#include "Surface.h"


class Rocket
{
private:

	float angle = 0;
	bool angleChanged = false;
	Vec2 pos = Vec2(400,325.0f);
	Vec2 rotDirection = Vec2(0,0);
	Vec2 moveVector = Vec2(0, 0);
	float maxSpeedSq = 200.0f * 200.0f;
	float acceleration = 100.0f;
	float fireCooldown = 0.0f;
	bool canShoot = true;
	static constexpr float defaultFireDelay = 0.02;
	static constexpr float powerFireDelay = 0.01;
	float fireDelay = defaultFireDelay;
	bool engineOn = false;
	void updateDirection();
	void updateSpeed(float dt);
	static constexpr int maxShieldEnergy = 100;
	int shieldEnergy = maxShieldEnergy;
	void updateControls(Keyboard& kbd);
	void updatePhysics(float dt);
	void updatePos(float dt);
	const Surface& surface;
	class ShieldDisplay
	{
	public:
		void draw(Graphics& gfx, int energy);
		Vec2 upperLeftCorner = { 20,20 };
		static constexpr int pixelsPerEnergy = 7;
		static constexpr Color color = Colors::Green;
		static constexpr int height = 20;
		


	};
	ShieldDisplay shieldDisplay;
public:
	Rocket(const Surface& surface);
	void draw(Graphics& gfx);
	void update(Keyboard& kbd, float dt);
	static constexpr int halfSize = 18;
	bool fire(Keyboard& kbd, float dt, PDVec& newBullet);
	Vec2 getPos();
	Vec2 getDirection();
	Vec2 getmoveVector();
	bool testSphereCollision(Vec2 spherePos, float sphereRadius) const;
	bool shieldActive = false;
	static constexpr float halfLength = 27.0f / 2.0f;
	static constexpr float halfWidth = 20.0f / 2.0f;
	static constexpr float mass = 500;
	int lastCollision = 999;
	void takingDamage(int damageReceived, Timer& timer);
	void gainingShield(int shieldGained);
	void init();
	bool stopMovement(float dt);
	void updateNoInput(float dt);
	bool rotateLeft();
	bool flyToLeft();
	bool testSquareCollision(Vec2 center, float halfWidth, float halfHeight);
	int getShieldEnergy();
	void setFireRate(bool powerMode);
};