#include "Asteroid.h"
#include <random>

Asteroid::Asteroid(const Vec2 pos_in, const Vec2 direction_in, const Size size_in, 
	const int identifier_in, const float speedModifier, const Sprites2& sprites2, std::mt19937& rng)

{
	const Surface* tempSpritePtr = nullptr;
	int tempAnimationSize;
	std::uniform_int_distribution<int> randomFrame(0, 7);
	int tempStartingFrame = randomFrame(rng);
	std::uniform_int_distribution<int> randomSkin(0, 1);
	
	switch (size_in)
	{
	case Size::tiny:
		radius = radius_tiny;
		hitpoints = 1;
		speed = 100.0f * speedModifier;
		mass = 50.0f;
		damageOnHit = 2;
		tempSpritePtr = &sprites2.asteroid_small;
		tempAnimationSize = 32;
		break;
	case Size::medium:
		radius = radius_medium;
		hitpoints = 5;
		speed = 50.0f * speedModifier;
		mass = 100.0f;
		damageOnHit = 5;
		tempSpritePtr = &sprites2.asteroid_medium;
		tempAnimationSize = 64;
		break;
	case Size::large:
		radius = radius_large;
		hitpoints = 10;
		speed = 20.0f * speedModifier;
		mass = 500.0f;
		damageOnHit = 10;
		tempSpritePtr = &sprites2.asteroid_huge;
		tempAnimationSize = 128;
		break;
	}
	int tempYpos = randomSkin(rng) * tempAnimationSize * 4;
	size = size_in;
	direction = direction_in * speed;
	pos = pos_in;
	identifier = identifier_in;
	lastCollision = 999; //initialize to collision with wall
	drawOffset = Vec2(tempAnimationSize/2, tempAnimationSize / 2);
	animation.customInit(0, tempYpos, tempAnimationSize, tempAnimationSize, 8, 4, 0.15f, tempSpritePtr,
		Colors::Magenta, tempStartingFrame);
}



void Asteroid::draw(Graphics & gfx) const
{
	animation.draw(pos-drawOffset, gfx);
}

bool Asteroid::updatePos(float dt)
{
	animation.update(dt);
	pos = pos + direction * dt;
	bool reflect = false;

	if (pos.x - radius < 0)
	{
		pos.x = 0 + radius;
		direction.x = -direction.x;
		lastCollision = 999;
		reflect = true;
		
	}

	if (pos.x + radius >= Graphics::ScreenWidth)
	{
		pos.x = Graphics::ScreenWidth -1 - radius;
		direction.x = -direction.x;
		lastCollision = 999;
		reflect = true;
	}

	if (pos.y - radius < 0)
	{
		pos.y = 0 + radius;
		direction.y = -direction.y;
		lastCollision = 999;
		reflect = true;
	}

	if (pos.y + radius >= Graphics::ScreenHeight)
	{
		pos.y = Graphics::ScreenHeight - 1 - radius;
		direction.y = -direction.y;
		lastCollision = 999;
		reflect = true;
	}
	return reflect;
}

bool Asteroid::collideAsteroid(Asteroid & otherAsteroid)
{
	Vec2  diff = pos - otherAsteroid.getPos();
	float dist = diff.GetLength();
	if (dist < radius + otherAsteroid.getRadius())
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vec2 Asteroid::getPos() const
{
	return pos;
}

Vec2 Asteroid::getDirection() const
{
	return direction;
}

void Asteroid::setDirection(Vec2 newDirection)
{
	direction = newDirection;
}

float Asteroid::getSpeed() const
{
	return speed;
}



float Asteroid::getRadius() const
{
	return radius;
}

float Asteroid::getMass() const
{
	return mass;
}

void Asteroid::bounceFromAsteroid(Asteroid & otherAsteroid)
{
	if (lastCollision != otherAsteroid.getIdentifier() || identifier != otherAsteroid.getLastCollision())
	{
		Vec2 un = pos - otherAsteroid.getPos();
		un = un.GetNormalized();
		Vec2 ut = Vec2(-un.y, un.x);
		Vec2 v1 = direction;
		Vec2 v2 = otherAsteroid.getDirection();
		float vn1 = un * v1;
		float vt1 = ut * v1;
		float vn2 = un * v2;
		float vt2 = ut * v2;
		float v´t1 = vt1;
		float v´t2 = vt2;
		float sumMass = mass + otherAsteroid.getMass();
		float v´n1 = (vn1 * (mass - otherAsteroid.getMass()) + 2 * otherAsteroid.getMass()*vn2) / sumMass;
		float v´n2 = ((vn2 * (otherAsteroid.getMass() - mass) + 2 * mass*vn1)) / sumMass;
		Vec2 v´n1Vec = un * v´n1;
		Vec2 v´t1Vec = ut * v´t1;
		Vec2 v´n2Vec = un * v´n2;
		Vec2 v´t2Vec = ut * v´t2;
		Vec2 v´1 = v´n1Vec + v´t1Vec;
		Vec2 v´2 = v´n2Vec + v´t2Vec;
		direction = v´1;
		lastCollision = otherAsteroid.getIdentifier();
		otherAsteroid.SetLastCollision(identifier);
		otherAsteroid.setDirection(v´2);
	}
	

}

void Asteroid::bounceFromRocket(Rocket & rocket)
{
	if (lastCollision != 998)
	{
		Vec2 un = pos - rocket.getPos();
		un = un.GetNormalized();
		Vec2 ut = Vec2(-un.y, un.x);
		Vec2 v1 = direction;
		Vec2 v2 = rocket.getmoveVector();
		float vn1 = un * v1;
		float vt1 = ut * v1;
		float vn2 = un * v2;
		float sumMass = mass + Rocket::mass;
		vn1 = (vn1 * (mass - Rocket::mass) + 2 * Rocket::mass*vn2) / sumMass;
		direction = un * vn1 + ut * vt1;
		lastCollision = 998;
	}
	

	
	

}

bool Asteroid::checkBulletCollision(Vec2 & posBullet)
{
	float distanceSq = Vec2(pos - posBullet).GetLengthSq();
	return (distanceSq < radius * radius);
}

int Asteroid::getIdentifier()
{
	return identifier;
}

int Asteroid::getLastCollision()
{
	return lastCollision;;
}

void Asteroid::SetLastCollision(int identifier)
{
	lastCollision = identifier;
}

bool Asteroid::collideAlien(const Vec2 & alienPos, float alienWidth, float alienHeight) const
{
	float alienRight = alienPos.x + alienWidth;
	float alienLeft = alienPos.x - alienWidth;
	float alienTop = alienPos.y - alienHeight;
	float alienBottom = alienPos.y + alienHeight;
	float nearestX = std::max(alienLeft,std::min(pos.x, alienRight));
	float nearestY = std::max(alienTop, std::min(pos.y, alienBottom));
	Vec2 delta(nearestX - pos.x, nearestY - pos.y);
	return ((delta.GetLengthSq()) < radius * radius);
}




	


