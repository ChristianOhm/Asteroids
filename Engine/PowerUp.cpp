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
		gfx.drawSprite(surface, colors[(int)type], (int)pos.x, (int)pos.y, Colors::Magenta);
	}
	



}

void PowerUp::reset(Timer & timer)
{
	timer.terminate(&visible);


}

PowerUp::PowerUp(const Surface& surface)
	:
	surface(surface)
{
	for (int counter = 0; counter <= 3; ++counter)
	{
		colors.emplace_back(RectI(counter * 25, (counter + 1) * 25, 0, 25));
	}
}
