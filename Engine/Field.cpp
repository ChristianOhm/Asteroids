#include "Field.h"
#include "Alien.h"


void Field::doAsteroidDestruction(Asteroid& asteroid)
{

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

void Field::deleteAsteroid(int index)
{
	std::swap(asteroids[index], asteroids.back());
	asteroids.pop_back();
}

void Field::generateNewAsteroid(Vec2 & pos, Vec2 & direction, Asteroid::Size size)
{
	asteroids.emplace_back(Asteroid(pos, direction, size, asteroids.size(), speedModifier, 
		sprites2, rng));
}

void Field::checkAsteroidsHitpoints(Scorecounter& scorecounter)
{

	for (int counter = 0; counter < asteroids.size(); ++counter)
	{
		if (asteroids[counter].hitpoints <= 0)
		{
			doAsteroidDestruction(asteroids[counter]);
			scorecounter.add(scorepoints[(int)asteroids[counter].size]);
			deleteAsteroid(counter);
		}
	}	
}

Field::Field(const Sprites2& sprites2)
	:
	rng((rd())),
	sprites2(sprites2)
{

}

void Field::updateAsteroids(float dt, Scorecounter& scorecounter)
{

	checkAsteroidsHitpoints(scorecounter);

	for (Asteroid& A : asteroids)
	{
		A.updatePos(dt);

	}

	for (int counter = 0; counter < asteroids.size(); ++counter)
	{
		for (int counter2 = counter + 1; counter2 < asteroids.size(); ++counter2)
		{
				if (asteroids[counter].collideAsteroid(asteroids[counter2]))
				{
					asteroids[counter].bounceFromAsteroid(asteroids[counter2]);
					break;
				}
		}
	}

}

void Field::draw(Graphics & gfx)
{
	for (const Asteroid& A : asteroids)
	{
		A.draw(gfx);

	}
}

bool Field::checkHit(Vec2 & posBullet)
{
	for (Asteroid& A : asteroids)
	{
		if (A.checkBulletCollision(posBullet))
		{

			--A.hitpoints;
			return true;
		}

	}
	return false;
}

void Field::testRocketAsteroidCollision(Rocket & rocket, Timer & timer)
{
	bool noCollision = true;
	for (Asteroid& A : asteroids)
	{
		if (rocket.testSphereCollision(A.getPos(), A.getRadius()))
		{
			A.bounceFromRocket(rocket);
			if (rocket.lastCollision != A.getIdentifier())
			{
				rocket.takingDamage(A.damageOnHit, timer);
				rocket.lastCollision = A.getIdentifier();
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
	for (const Asteroid& A : asteroids)
	{
		float asteroidRight = (A.getPos()).x + A.getRadius();
		if (alienLeft - asteroidRight > 0 && alienLeft - asteroidRight < Alien::safetyX)
		{
			float alienTop = alienPos.y - Alien::halfHeight - Alien::safetyY;
			float alienBottom = alienPos.y + Alien::halfHeight + Alien::safetyY;
			float asteroidTop = (A.getPos()).y - A.getRadius();
			float asteroidBottom = (A.getPos()).y + A.getRadius();
			if (asteroidBottom > alienTop && asteroidTop < alienBottom)
			{
				collisionAhead = true;
				if ((A.getPos()).y <= alienPos.y)
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

bool Field::testAlienFieldCollision(const Vec2& alienPos)
{
	for (const Asteroid& A : asteroids)
	{
		if (A.collideAlien(alienPos, Alien::halfWidth, Alien::halfHeight))
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

		} while (!noIssue);
		Vec2 newDirection = Vec2(randomDirection(rng), randomDirection(rng));
		generateNewAsteroid(newPos, newDirection, Asteroid::Size::large);
	}
}

bool Field::levelComplete()
{
	return (asteroids.size() == 0);
}
