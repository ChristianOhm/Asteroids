#include "Bullet.h"

void Bullet::draw(Graphics & gfx)
{
	if (underway)
	{
		gfx.drawCircle(int(pos.x), int(pos.y),int(radius), 0, c);
	}
	if (showHitAnimation)
	{
   		gfx.drawCircle(int(pos.x), int(pos.y), int(radiusHit), 0, colorHit);
	}

}

void Bullet::updateHitAnimationCounter(float dt)
{
	hitAnimationCounter += dt;
	if (hitAnimationCounter >= hitAnimationTime)
	{
		showHitAnimation = false;
	}
	
}

void Bullet::updatePos(float dt)
{
	pos = pos + direction * dt * speed;
	if (pos.outsideScreen(radiusHit))
	{
		underway = false;
		showHitAnimation = false;
	}
}

void Bullet::init(Vec2 pos_in, Vec2 direction_in, Origin origin_in)
{
	origin = origin_in;
	switch (origin)
	{
	case Origin::player:
		speed = 400;
		radius = 2.0f;
		c = Colors::White;
		colorHit = Colors::Red;
		break;

	case Origin::alien:
		speed = 400;
		radius = 2.0f;
		c = Colors::Green;
		colorHit = Colors::Red;
		break;
	}
	pos = pos_in;
	direction = direction_in;
	underway = true;
}


Vec2 Bullet::getPos()
{
	return pos;
}


