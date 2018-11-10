#pragma once
#include "Bullet.h"
#include "Vec2.h"
#include "PDVec.h"
#include "Timer.h"
#include "Graphics.h"
#include "Field.h"
#include "Rocket.h"
#include "Alien.h"


class Bullets
{
private:

	Bullet* topBulletPtr = nullptr;
	Bullet* topHitAnimationPtr = nullptr;
	void deleteBullet(Bullet* toDelete);
	void deleteHitAnimation(Bullet* toDelete);
	void startHitAnimation(Vec2 pos_in, Timer& timer);

public:
	void launchBullet(PDVec newBullet_in, Bullet::Origin origin_in);
	void updateBullets(float dt, Timer& timer, Field& field, Rocket& rocket, Alien& alien);
	void draw(Graphics& gfx);
	

};