#include "Bullet.h"

void Bullet::draw(Graphics & gfx)
{
	gfx.drawCircle(int(pos.x), int(pos.y),int(radius), 0, c);
}



bool Bullet::updatePosCheckScreen(float dt)
{
	pos = pos + direction * dt * speed;
	return pos.outsideScreen(radiusHit);

}

Vec2 Bullet::getPos()
{
	return pos;
}

Bullet::Bullet(PDVec pdVec_in, Origin origin_in, Bullet * nextBulletPtr_in)
	:
	nextBulletPtr(nextBulletPtr_in),
	pos (pdVec_in.pos),
	direction (pdVec_in.direction),
	origin (origin_in)
{
	switch (origin)
	{
	case Origin::player:
		speed = 400;
		radius = 2.0f;
		c = Colors::White;
		break;

	case Origin::alien:
		speed = 400;
		radius = 2.0f;
		c = Colors::Green;
		break;
	}
}

Bullet::~Bullet()
{
	delete nextBulletPtr;
}

Bullet * Bullet::separate()
{
	Bullet* temp = nextBulletPtr;
	nextBulletPtr = nullptr;
	return temp;
}

void Bullet::reLink(Bullet * toReplace, Bullet * replaceWith)
{
	if (nextBulletPtr == toReplace)
	{
		nextBulletPtr = replaceWith;
	}
	else if (nextBulletPtr != 0)
	{
		nextBulletPtr->reLink(toReplace, replaceWith);
	}
}

Bullet * Bullet::getNext()
{
	return nextBulletPtr;
}


