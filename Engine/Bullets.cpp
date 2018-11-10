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
			startHitAnimation(currentBullet->getPos(), timer);
			deleteBullet(currentBullet);
		}
		
		if (!stop && alien.underway && currentBullet->origin == Bullet::Origin::player
			&& alien.checkBulletHit(currentBullet->getPos()))
		{
			stop = true;
			deleteBullet(currentBullet);
		}

		if (!stop && currentBullet->origin == Bullet::Origin::alien 
			&& rocket.testSphereCollision(currentBullet->getPos(), currentBullet->getRadius()))
		{
			stop = true;
			deleteBullet(currentBullet);
			rocket.takingDamage(Bullet::damageOnHit, timer);
		}
	} 

	Bullet* nextHitAnimation = topHitAnimationPtr;

	while (nextHitAnimation != nullptr)
	{
		Bullet* currentHitAnimation = nextHitAnimation;
		nextHitAnimation = currentHitAnimation->getNext();
		if (currentHitAnimation->deactivateHitAnimation)
		{
			deleteHitAnimation(currentHitAnimation);
		}
	}
}

void Bullets::draw(Graphics & gfx)
{
	
 	for (Bullet* currentBullet = topBulletPtr; currentBullet != 0; currentBullet = currentBullet->getNext())
	{
		currentBullet->draw(gfx);
	}

	for (Bullet* currentHitAnimation = topHitAnimationPtr; currentHitAnimation != 0; currentHitAnimation = currentHitAnimation->getNext())
	{
		currentHitAnimation->draw(gfx);
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

void Bullets::deleteHitAnimation(Bullet * toDelete)
{
	assert(toDelete != 0);
	Bullet* oldNext = toDelete->separate();
	if (toDelete == topHitAnimationPtr)
	{
		topHitAnimationPtr = oldNext;
		delete toDelete;
	}
	else
	{
		topHitAnimationPtr->reLink(toDelete, oldNext);
		delete toDelete;
	}
}

void Bullets::startHitAnimation(Vec2 pos_in, Timer& timer)
{
	if (!pos_in.outsideScreen(Bullet::radiusHitAnimation))
	{
		topHitAnimationPtr = new Bullet(pos_in, topHitAnimationPtr);
		timer.init(&topHitAnimationPtr->deactivateHitAnimation, Bullet::durationHitAnimation);
	}
	
}
