#include "Rocket.h"

void Rocket::updateDirection()
{
	rotDirection = Vec2(-cos(angle), sin(angle));
}

void Rocket::updateSpeed(float dt)
{
	Vec2 newMoveVector = moveVector +  rotDirection * dt * acceleration;
	if (newMoveVector.GetLengthSq() < maxSpeedSq)
	{
		moveVector = newMoveVector;
	}
}

void Rocket::draw(Graphics & gfx)
{
	Sprites::drawRocket(gfx, pos, angle);
	//gfx.PutPixel(pos.x, pos.y, Colors::Red);
	if (shieldActive)
	{
		gfx.drawCircle(int(pos.x), int(pos.y), halfLength, halfLength-2, Colors::Green);
	}
	shieldDisplay.draw(gfx, shieldEnergy);
}

void Rocket::update(Keyboard & kbd, float dt)
{
	updateControls(kbd);
	updatePhysics(dt);
	updatePos(dt);
}

void Rocket::updateControls(Keyboard & kbd)
{
	if (kbd.KeyIsPressed(VK_RIGHT))
	{
		angle -= 0.04;
		angleChanged = true;
	}

	if (kbd.KeyIsPressed(VK_LEFT))
	{
		angle += 0.04;
		angleChanged = true;
	}

	if (kbd.KeyIsPressed(VK_UP))
	{
		engineOn = true;
	}
	else
	{
		engineOn = false;
	}
}

void Rocket::updatePhysics(float dt)
{
	updateDirection();
	if (engineOn)
	{
		updateSpeed(dt);
	}
}

void Rocket::updatePos(float dt)
{
	pos = pos + moveVector * dt;
	if (pos.x - halfSize < 0)
	{
		pos.x = Graphics::ScreenWidth - halfSize;
	}

	if (pos.x + halfSize > Graphics::ScreenWidth)
	{
		pos.x = 0 + halfSize;
	}

	if (pos.y - halfSize < 0)
	{
		pos.y = Graphics::ScreenHeight - halfSize;
	}

	if (pos.y + halfSize > Graphics::ScreenHeight)
	{
		pos.y = 0 + halfSize;
	}
}

bool Rocket::fire(Keyboard & kbd, float dt, PDVec& newBullet)
{
	if (!canShoot)
	{
		fireCooldown += dt;
		if (fireCooldown > fireDelay)
		{
			canShoot = true;
		}
	}

	if (kbd.KeyIsPressed(VK_SPACE) && canShoot)
	{
		canShoot = false;
		fireCooldown = 0.0f;
		newBullet.pos = pos + rotDirection;
		newBullet.direction = rotDirection;
		return true;
	}
	else
	{
		return false;
	}
}

Vec2 Rocket::getPos()
{
	return pos;
}

Vec2 Rocket::getDirection()
{
	return rotDirection;
}

Vec2 Rocket::getmoveVector()
{
	return moveVector;
}

bool Rocket::testSphereCollision(Vec2 spherePos, float sphereRadius) const
{
	Vec2 collisionVector = spherePos - pos;
	float collisionVectorLength = collisionVector.GetLength();
	Vec2 collisionVectorNorm = collisionVector / collisionVectorLength;
	float longAxisFraction = collisionVectorNorm * rotDirection;
	if (longAxisFraction < 0)
	{
		longAxisFraction *= -1;
	}
	float shortAxisFraction = 1 - longAxisFraction;
	float effectiveRadiusRocket = longAxisFraction*halfLength + shortAxisFraction * halfWidth;
	return (collisionVectorLength < sphereRadius + effectiveRadiusRocket);

}



void Rocket::ShieldDisplay::draw(Graphics & gfx, int energy)
{
	int right = int(upperLeftCorner.x) + pixelsPerEnergy * energy;
	right = std::min(right, Graphics::ScreenWidth);
	right = std::max(right, int(upperLeftCorner.x));
	gfx.DrawRect(upperLeftCorner, Vec2(right, upperLeftCorner.y + height), color);

}

void Rocket::takingDamage(int damageReceived, Timer & timer)
{
	shieldEnergy -= damageReceived;
	if (!shieldActive)
	{
		shieldActive = true;
		timer.init(&shieldActive, 1);
	}
	
}

void Rocket::gainingShield(int shieldGained)
{
	shieldEnergy = std::min (shieldEnergy+shieldGained, maxShieldEnergy);
}

void Rocket::init()
{
	angle = 3.0f * 3.14159265f / 2.0f;
	pos = Vec2(400, 353.5f);
	rotDirection = Vec2(0, 0);
	moveVector = Vec2(0, 0);
}

bool Rocket::stopMovement(float dt)
{
	if (moveVector.GetLengthSq() < 3)
	{
		moveVector = { 0,0 };
		return true;
	}
	moveVector = moveVector / 1.02f;
	return false;
}

void Rocket::updateNoInput(float dt)
{
	updatePhysics(dt);
	updatePos(dt);
}

bool Rocket::rotateLeft()
{
	
	
	if (rotDirection.x >= -1.001f && rotDirection.x <= -0.999f)
	{
		return true;
	}
	if (rotDirection.y > 0)
	{
		angle -= 0.04;
		angleChanged = true;
	}
	else
	{
		angle += 0.04;
		angleChanged = true;
	}
	return false;
}

bool Rocket::flyToLeft()
{
	if (pos.x <= halfWidth + 15.0f)
	{
		return true;
	}
	engineOn = true;
	return false;
	
}

bool Rocket::testSquareCollision(Vec2 center, float halfWidth, float halfHeight)
{
	float squareRight = center.x + halfWidth;
	float squareLeft = center.x - halfWidth;
	float squareTop = center.y - halfHeight;
	float squareBottom = center.y + halfHeight;
	Vec2 collisionVectorNorm = (center - pos).Normalize();
	float longAxisFraction = collisionVectorNorm * rotDirection;
	if (longAxisFraction < 0)
	{
		longAxisFraction *= -1;
	}
	float shortAxisFraction = 1 - longAxisFraction;
	float effectiveRadiusRocket = longAxisFraction * halfLength + shortAxisFraction * halfWidth;
	float nearestX = std::max(squareLeft, std::min(pos.x, squareRight));
	float nearestY = std::max(squareTop, std::min(pos.y, squareBottom));
	Vec2 delta(nearestX - pos.x, nearestY - pos.y);
	return ((delta.GetLengthSq()) < effectiveRadiusRocket * effectiveRadiusRocket);

}

int Rocket::getShieldEnergy()
{
	return shieldEnergy;
}

void Rocket::setFireRate(bool powerMode)
{
	if (powerMode)
	{
		fireDelay = powerFireDelay;
	}
	else
	{
		fireDelay = defaultFireDelay;
	}
}
