#include "Alien.h"

void Alien::spawn(std::mt19937 & rng)
{
	std::uniform_real_distribution <float> randomY(Alien::halfHeight, Graphics::ScreenHeight - 1 - Alien::halfHeight);
	float spawnY = randomY(rng);
	float spawnX = Graphics::ScreenWidth - 1 - Alien::halfWidth;
	pos = Vec2(spawnX, spawnY);
	underway = true;
	timeSinceSpawn = 0.0f;
	hitpoints = maxHitpoints;
}

void Alien::draw(Graphics & gfx)
{
	animation.draw(pos - Vec2(halfWidth, halfHeight), gfx);
}

void Alien::updatePos(bool collisionAhead, Vec2& evadeDirection, float dt)
{
	animation.update(dt);
	if (!collisionAhead)
	{
		pos.x = pos.x - dt * speed;
	}
	else
	{
		pos = pos + evadeDirection * speed * dt;
	}

	if (pos.x - halfWidth <0 || pos.x + halfWidth > Graphics::ScreenWidth - 1 || pos.y - halfHeight < 0 || pos.y + halfHeight > Graphics::ScreenHeight - 1)
	{
		underway = false;
	}
}


bool Alien::testSpawn(float dt)
{
	timeSinceSpawn += dt;
	return (timeSinceSpawn >= timeTillSpawn);
}

Vec2 Alien::getPos()
{
	return pos;
}

bool Alien::testShoot(float dt)
{
	if (!duringSalve)
	{
		salveCooldown += dt;
		if (salveCooldown >= salveDelay)
		{
			salveCooldown = 0.0f;
			duringSalve = true;
			timesFiredSalve = 1;
			return true;
		}
		else
		{
			return false;
		}

	}
	else
	{
		fireCooldown += dt;
		if (fireCooldown >= fireDelay)
		{
			fireCooldown = 0.0f;
			++timesFiredSalve;
			if (timesFiredSalve >= bulletsPerSalve)
			{
				duringSalve = false;
			}
			return true;
		}
		else
		{
			return false;
		}
	}
}

Alien::Alien(const Surface* alien)
	:
	alien(alien),
	animation(0,0,52,24,2,1,0.5f,alien, Colors::Magenta)
{
}

bool Alien::checkBulletHit(Vec2 & posBullet)
{
	float alienLeft = pos.x - halfWidth;
	float alienRight = pos.x + halfWidth;
	float alienTop = pos.y - halfHeight;
	float alienBottom = pos.y + halfHeight;
	if (posBullet.x >= alienLeft && posBullet.x <= alienRight && posBullet.y >= alienTop && posBullet.y <= alienBottom)
	{
		--hitpoints;
		return true;
	}
	
	return false;
}

void Alien::checkHitpoints(PowerUp & powerUp, Timer & timer, std::mt19937 & rng)
{
	if (hitpoints <= 0)
	{
		underway = false;
		powerUp.spawn(pos, timer, rng);
	}
}



void Alien::setSpawnTime(float newSpawnTime)
{
	timeTillSpawn = newSpawnTime;
}


void Alien::update(Field & field, float dt, Timer & timer, std::mt19937 & rng, Rocket& rocket, PowerUp& powerUp)
{
	if (underway)
	{
		bool collisionAhead = false;
		Vec2 evadeDirection(0, 0);
		field.checkAlienPath(pos, collisionAhead, evadeDirection);
		updatePos(collisionAhead, evadeDirection, dt);
		if (field.testAlienFieldCollision(pos))
		{
			underway = false;
		}
		if (rocket.testSquareCollision(pos, halfHeight, halfHeight))
		{
			underway = false;
			rocket.takingDamage(damageOnHit, timer);
		}

		checkHitpoints(powerUp, timer, rng);
	}

	else if (testSpawn(dt))
	{
		spawn(rng);
	}
}

bool Alien::shoot(const Vec2 target, const float dt, PDVec & bulletPD)
{
	if (underway && testShoot(dt))
	{
		bulletPD.direction = (Vec2(target - pos)).GetNormalized();
		bulletPD.pos = pos;
		return true;
	}
	return false;
}


