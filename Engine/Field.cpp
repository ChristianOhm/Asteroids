#include "Field.h"
#include "Alien.h"


void Field::doAsteroidDestruction(Asteroid& asteroid)
{

	asteroid.destroyed = true;
	if (!(asteroid.size == Asteroid::Size::tiny))
	{
		float startAngle = angle(rng);
		float angle_1 = startAngle * convert;
		float angle_2 = (startAngle + 120) * convert;
		float angle_3 = (startAngle + 240) * convert;
		Vec2 direction_1(cos(angle_1), sin(angle_1));
		Vec2 direction_2(cos(angle_2), sin(angle_2));
		Vec2 direction_3(cos(angle_3), sin(angle_3));
		Vec2 pos_1;
		Vec2 pos_2;
		Vec2 pos_3;
		Asteroid::Size size;

		if (asteroid.size == Asteroid::Size::large)
		{
			pos_1 = asteroid.getPos() + direction_1 * Asteroid::radius_large;
			pos_2 = asteroid.getPos() + direction_2 * Asteroid::radius_large;
			pos_3 = asteroid.getPos() + direction_3 * Asteroid::radius_large;
			size = Asteroid::Size::medium;
		}

		else if (asteroid.size == Asteroid::Size::medium)
		{
			pos_1 = asteroid.getPos() + direction_1 * Asteroid::radius_medium;
			pos_2 = asteroid.getPos() + direction_2 * Asteroid::radius_medium;
			pos_3 = asteroid.getPos() + direction_3 * Asteroid::radius_medium;
			size = Asteroid::Size::tiny;
		}


		generateNewAsteroid(pos_1, direction_1, size);
		generateNewAsteroid(pos_2, direction_2, size);
		generateNewAsteroid(pos_3, direction_3, size);

	}

}

void Field::generateNewAsteroid(Vec2 & pos, Vec2 & direction, Asteroid::Size & size)
{
	for (int counter = 0; counter < maxAsteroids; ++counter)
	{
		if (asteroids[counter].destroyed)
		{
			asteroids[counter].init(pos, direction, size, counter, speedModifier);
break;
		}
	}
}

void Field::checkAsteroidsHitpoints()
{

	for (int counter = 0; counter < maxAsteroids; ++counter)
	{
		if (!asteroids[counter].destroyed && asteroids[counter].hitpoints <= 0)
		{
			doAsteroidDestruction(asteroids[counter]);
		}
	}

}

Field::Field()
	:
	rng((rd())),
	collisionSound({ L"sound\\Collision_0.wav", L"sound\\Collision_1.wav" }),//, L"sound\\Collision_2.wav", L"sound\\Collision_3.wav" }),
	wallCollideSound({ L"sound\\Wall_0.wav", L"sound\\Wall_1.wav" , L"sound\\Wall_2.wav",L"sound\\Wall_3.wav" })
{

}

void Field::updateAsteroids(float dt)
{

	checkAsteroidsHitpoints();

	for (int counter = 0; counter < maxAsteroids - 1; ++counter)
	{
		if (!asteroids[counter].destroyed)
		{
			if (asteroids[counter].updatePos(dt))
			{
				//wallCollideSound.Play(rng);
			}

		}
	}


	for (int counter = 0; counter < maxAsteroids; ++counter)
	{
		if (!asteroids[counter].destroyed)
		{
			for (int counter2 = counter + 1; counter2 < maxAsteroids; ++counter2)
			{
				if (!asteroids[counter2].destroyed && asteroids[counter].collideAsteroid(asteroids[counter2]))
				{
					asteroids[counter].bounceFromAsteroid(asteroids[counter2]);
					collisionSound.Play(rng);
					break;
				}
			}
		}
	}

}

void Field::draw(Graphics & gfx)
{
	for (int counter = 0; counter < maxAsteroids; ++counter)
	{
		if (!asteroids[counter].destroyed)
		{
			asteroids[counter].draw(gfx);
		}
	}

}

bool Field::checkHit(Vec2 & posBullet)
{

	for (int counter = 0; counter < maxAsteroids; ++counter)
	{
		if (!asteroids[counter].destroyed && asteroids[counter].checkBulletCollision(posBullet))
		{

			--asteroids[counter].hitpoints;
			return true;

		}
	}

	return false;
}

void Field::testRocketAsteroidCollision(Rocket & rocket, Timer & timer)
{
	bool noCollision = true;
	for (int counter = 0; counter < maxAsteroids; ++counter)
	{
		if (!asteroids[counter].destroyed && rocket.testSphereCollision(asteroids[counter].getPos(), asteroids[counter].getRadius()))
		{
			asteroids[counter].bounceFromRocket(rocket);
			if (rocket.lastCollision != asteroids[counter].getIdentifier())
			{
				rocket.takingDamage(asteroids[counter].damageOnHit, timer);
				rocket.lastCollision = asteroids[counter].getIdentifier();
			}


		}
	}
	if (noCollision)
	{
		rocket.lastCollision = 999;
	}
}



void Field::checkAlienPath(const Vec2& alienPos, bool& collisionAhead, Vec2& evadeDirection)
{

	float alienLeft = (alienPos.x - Alien::halfWidth);

	for (int counter = 0; counter < maxAsteroids; ++counter)
	{
		if (!asteroids[counter].destroyed)
		{
			float asteroidRight = (asteroids[counter].getPos()).x + asteroids[counter].getRadius();
			if (alienLeft - asteroidRight > 0 && alienLeft - asteroidRight < Alien::safetyX)
			{
				float alienTop = alienPos.y - Alien::halfHeight - Alien::safetyY;
				float alienBottom = alienPos.y + Alien::halfHeight + Alien::safetyY;
				float asteroidTop = (asteroids[counter].getPos()).y - asteroids[counter].getRadius();
				float asteroidBottom = (asteroids[counter].getPos()).y +  asteroids[counter].getRadius();
				if (asteroidBottom > alienTop && asteroidTop < alienBottom)
				{
					collisionAhead = true;
					if ((asteroids[counter].getPos()).y <= alienPos.y)
					{
						evadeDirection = { -0.5f, 1 };
						break;
					}
					else
					{
						evadeDirection = { -0.5,-1 };
						break;
					}
				}
			}
		}
	}
}

bool Field::testAlienFieldCollision(const Vec2& alienPos)
{
	for (int counter = 0; counter < maxAsteroids; ++counter)
	{
		if (!asteroids[counter].destroyed&&asteroids[counter].collideAlien(alienPos, Alien::halfWidth, Alien::halfHeight))
		{
			return true;
		}
	}
	return false;
}

void Field::initLevel(std::mt19937 & rng, int level)
{
	std::uniform_real_distribution <float> randomX(Asteroid::radius_large, Graphics::ScreenWidth-1-Asteroid::radius_large);
	std::uniform_real_distribution <float> randomY(Asteroid::radius_large, Graphics::ScreenHeight - 1 - Asteroid::radius_large);
	std::uniform_real_distribution <float> randomDirection(-1, 1);
	speedModifier += 0.1f * level;
	for (int counter = 0; counter < level * 2; ++counter)
	{
		Vec2 newPos;
		Vec2 center(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2);
		bool noIssue;
		do
		{
			noIssue = true;
			newPos = Vec2(randomX(rng), randomY(rng));
			float distFromCenterSq = (newPos - center).GetLengthSq();
			if (distFromCenterSq <= asteroidFreeZoneRadiusSq)
			{
				noIssue = false;
			}
			for (int counter2 = 0; counter2 < counter; ++counter2)
			{
				float distFromOtherAsteroidSq = (asteroids[counter2].getPos() - newPos).GetLengthSq();
				if (distFromOtherAsteroidSq <= asteroidMinDistSq)
				{
					noIssue = false;
					break;
				}
			}
			
		}
		while (!noIssue);
		Vec2 newDirection = Vec2(randomDirection(rng), randomDirection(rng));
		asteroids[counter].init(newPos, newDirection, Asteroid::Size::large, counter, speedModifier);
	}
}

bool Field::levelComplete()
{
	bool complete = true;
	for (int counter = 0; counter < maxAsteroids; ++counter)
	{
		if (!asteroids[counter].destroyed)
		{
			complete = false;
			break;
		}
	}
	return complete;
}
