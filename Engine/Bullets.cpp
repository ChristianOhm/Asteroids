#include "Bullets.h"

void Bullets::launchBullet(PDVec newBullet_in, Bullet::Origin origin_in)
{
	for (int counter = 0; counter < maxBullets; ++counter)
	{
		if (!bullets[counter].underway)
		{
			bullets[counter].init(newBullet_in.pos, newBullet_in.direction, origin_in);
			break;
		}
	}
}

void Bullets::updateBullets(float dt, Timer & timer, Field& field, Rocket& rocket, Alien& alien)
{
	for (int counter = 0; counter < maxBullets; ++counter)
	{
		bullets[counter].updatePos(dt);
		if (bullets[counter].showHitAnimation)
		{
			bullets[counter].updateHitAnimationCounter(dt);
		}
		if (bullets[counter].underway)
		{
			
			if (field.checkHit(bullets[counter].getPos()))
			{
				bullets[counter].underway = false;
				bullets[counter].showHitAnimation = true;
			}
			if (alien.underway && bullets[counter].origin == Bullet::Origin::player && alien.checkBulletHit(bullets[counter].getPos()))
			{
				bullets[counter].underway = false;
				bullets[counter].showHitAnimation = true;
			}
			if (bullets[counter].origin == Bullet::Origin::alien &&
				rocket.testSphereCollision(bullets[counter].getPos(), bullets[counter].radius))
			{
				bullets[counter].underway = false;
				bullets[counter].showHitAnimation = true;
				rocket.takingDamage(Bullet::damageOnHit, timer);
				


			}
		

		}
	}
}

void Bullets::draw(Graphics & gfx)
{
	for (int counter = 0; counter < maxBullets; ++counter)
	{
		if (bullets[counter].underway || bullets[counter].showHitAnimation)
		{
			bullets[counter].draw(gfx);
		}
	}
}

bool Bullets::noUnderway()
{
	for (int counter = 0; counter < maxBullets; ++counter)
	{
		if (bullets[counter].underway)
		{
			return false;
		}
	}
	return true;
}
