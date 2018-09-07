#include "Bullets.h"
#include <assert.h>

void Bullets::launchBullet(PDVec newBullet_in, Bullet::Origin origin_in)
{
	topBulletPtr = new Bullet(newBullet_in, origin_in, topBulletPtr);
}

void Bullets::updateBullets(float dt, Timer & timer, Field& field, Rocket& rocket, Alien& alien)
{
	Bullet* nextBullet = topBulletPtr;

	while (nextBullet != nullptr)
	{
		Bullet* currentBullet = nextBullet;
		nextBullet = currentBullet->getNext();
		bool stop = false;

		if (currentBullet->updatePosCheckScreen(dt))
		{
			stop = true;
			deleteBullet(currentBullet);
		}
		
		if (!stop && field.checkHit(currentBullet->getPos()))
		{
			stop = true;
			deleteBullet(currentBullet);
		}
		
		if (!stop && alien.underway && currentBullet->origin == Bullet::Origin::player
			&& alien.checkBulletHit(currentBullet->getPos()))
		{
			stop = true;
			deleteBullet(currentBullet);
		}

		if (!stop && currentBullet->origin == Bullet::Origin::alien 
			&& rocket.testSphereCollision(currentBullet->getPos(), currentBullet->radius))
		{
			stop = true;
			deleteBullet(currentBullet);
			rocket.takingDamage(Bullet::damageOnHit, timer);

		}
	} 

}

void Bullets::draw(Graphics & gfx)
{
	
 	for (Bullet* currentBullet = topBulletPtr; currentBullet != 0; currentBullet = currentBullet->getNext())
	{
		currentBullet->draw(gfx);
	}


}


void Bullets::deleteBullet(Bullet * toDelete)
{
	assert(toDelete != 0);
	Bullet* oldNext = toDelete->separate();
	if (toDelete == topBulletPtr)
	{
		topBulletPtr = oldNext;
		delete toDelete;
	}
	else
	{
		topBulletPtr->reLink(toDelete, oldNext);
		delete toDelete;
	}
	
	
	
	

}
