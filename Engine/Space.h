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
#include "Alien.h"
#include <random>
#include "Timer.h"
#include "PDVec.h"
#include "Bullets.h"
#include "PowerUp.h"
#include "Sprites2.h"
#include "Scorecounter.h"


class Space
{
private:
	const Sprites2& sprites2;
	Rocket rocket;
	Field field;
	Alien alien;
	Bullets bullets;
	PowerUp powerUp;
	Scorecounter scorecounter;
	
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
	bool sequenceEmergencyStop = false;
	float sequenceEmergencyTime = 12.0f;
	

public:
	void update(Keyboard& kbd, float dt, Timer& timer);
	bool levelComplete(Timer& timer);
	void initLevel(int level, Timer& timer);
	void draw(Graphics& gfx);
	Space(const Sprites2& sprites2_in);
	bool endSequenceComplete(float dt, Timer& timer);
	bool rocketDestroyed();
	bool gameOver = false;
};
