#pragma once
#include "Vec2.h"
#include "Timer.h"
#include <random>
#include "Sprites.h"
#include "Rocket.h"

class PowerUp
{

public:
	void spawn(const Vec2& pos_in, Timer& timer, std::mt19937& rng);
	void update(const float dt, Timer& timer, const Rocket& rocket, bool* switches);
	Vec2 getPos();
	void activatePower(bool* switches, Timer& timer);
	void draw(Graphics& gfx);
	static constexpr int nPowers = 3;
	void reset(Timer& timer);
	static constexpr float radius = 13;
	static constexpr float angleForPower0 = 0.2f;
	static constexpr int energyIncreaseForPower2 = 30;

private:
	Vec2 pos;
	bool visible = false;
	static constexpr float halfHeight = 13;
	static constexpr float halfWidth = 13;
	static constexpr float speed = 10.0f;
	static constexpr float timeVisible = 30;
	static constexpr float powerDuration = 20;
	const Vec2 direction_norm = { 0,-1 };
	enum class Type
	{
		blue,
		red,
		green,
		yellow
	};
	Type type;
};