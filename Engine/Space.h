#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <chrono>
#include "Rocket.h"
#include "FrameTimer.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "Field.h"
#include "Sound.h"
#include "Alien.h"
#include <random>
#include "Timer.h"
#include "PDVec.h"
#include "Bullets.h"
#include "PowerUp.h"

class Space
{
private:
	Rocket rocket;
	Field field;
	Sound shoot;
	Alien alien;
	Bullets bullets;
	PowerUp powerUp;
	std::random_device rd;
	std::mt19937 rng;
	enum class StageLevelEnd
	{
		notStarted,
		stop,
		rotate,
		flyLeft,
		complete
	};
	StageLevelEnd stageLevelEnd = StageLevelEnd::stop;
	bool powerUpSwitches[PowerUp::nPowers];
	void processPowerUpSwitches(Timer& timer);

public:
	void update(Keyboard& kbd, float dt, Timer& timer);
	bool levelComplete();
	void initLevel(int level, Timer& timer);
	void draw(Graphics& gfx);
	Space();
	bool endSequenceComplete(float dt, Timer& timer);
	bool rocketDestroyed();

};
