#include "PowerUp.h"

void PowerUp::spawn(const Vec2 & pos_in, Timer & timer, std::mt19937& rng)
{
	reset(timer);
	pos = pos_in;
	std::uniform_int_distribution <int> randomPower(0, nPowers - 1);
	type = Type(randomPower(rng));
	//type = Type(2);
	visible = true;
	timer.init(&visible, timeVisible);
}

void PowerUp::update(const float dt, Timer & timer, const Rocket & rocket, bool * switches)
{
	if (visible)
	{
		pos += direction_norm * speed * dt;

		if (pos.outsideScreen(halfWidth, halfHeight))
		{
			timer.terminate(&visible);
		}
		if (rocket.testSphereCollision(pos, halfWidth))
		{
			timer.terminate(&visible);
			activatePower(switches, timer);

		}
	}
}




Vec2 PowerUp::getPos()
{
	return pos;
}

void PowerUp::activatePower(bool * switches, Timer & timer)
{
	switches[int(type)] = true;
	timer.init(&switches[int(type)], powerDuration);
}

void PowerUp::draw(Graphics & gfx)
{
	if (visible)
	{
		switch (type)
		{
		case Type::blue:
			Sprites::drawBluePowerUp(gfx, pos);
			break;
		case Type::green:
			Sprites::drawGreenPowerUp(gfx, pos);
			break;
		case Type::red:
			Sprites::drawRedPowerUp(gfx, pos);
			break;
		case Type::yellow:
			Sprites::drawYellowPowerUp(gfx, pos);
			break;

		}
	}
	



}

void PowerUp::reset(Timer & timer)
{
	timer.terminate(&visible);


}
