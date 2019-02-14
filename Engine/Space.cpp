#include "Space.h"




void Space::processPowerUpSwitches(Timer & timer)
{
	if (powerUpSwitches[1])
	{
		rocket.setFireRate(true);
	
	}
	else
	{
		rocket.setFireRate(false);
	}

	if (powerUpSwitches[2])
	{
		rocket.gainingShield(PowerUp::energyIncreaseForPower2);
		timer.terminate(&powerUpSwitches[2]);
	}
}

void Space::update(Keyboard& kbd, float dt, Timer& timer)


{
	if (!gameOver)
	{
		processPowerUpSwitches(timer);
		PDVec newBullet({ 0,0 }, { 0,0 });
		rocket.update(kbd, dt);
		

		if (rocket.fire(kbd, dt, newBullet))
		{

			if (powerUpSwitches[0])
			{
				bullets.launchBullet(newBullet.rotateLeft(PowerUp::angleForPower0), Bullet::Origin::player);
				bullets.launchBullet(newBullet.rotateRight(PowerUp::angleForPower0), Bullet::Origin::player);
			}

			else
			{
				bullets.launchBullet(newBullet, Bullet::Origin::player);
			}
		}
		if (alien.shoot(rocket.getPos(), dt, newBullet))
		{

			bullets.launchBullet(newBullet, Bullet::Origin::alien);
		}
		field.testRocketAsteroidCollision(rocket, timer);
		powerUp.update(dt, timer, rocket, powerUpSwitches);
	}
	alien.update(field, dt, timer, rng, rocket, powerUp, scorecounter);
	field.updateAsteroids(dt, scorecounter);
	bullets.updateBullets(dt, timer, field, rocket, alien);
	


}
	

bool Space::levelComplete(Timer& timer)
{
	if (field.levelComplete() && !alien.underway)
	{
		timer.init(&sequenceEmergencyStop, sequenceEmergencyTime);
		return true;
	}
		
	return false;

}

void Space::initLevel(int currentLevel, Timer& timer)
{
	field.initLevel(rng, currentLevel);
	rocket.init();
	powerUp.reset(timer);
}

void Space::draw(Graphics & gfx)
{

	field.draw(gfx);
	rocket.draw(gfx);
	if (alien.underway)
	{
		alien.draw(gfx);
	}
	powerUp.draw(gfx);
	bullets.draw(gfx);
	scorecounter.display(gfx);

}

Space::Space(const Sprites2& sprites2_in)
	:
	rng(std::random_device()()),
	sprites2(sprites2_in),
	field(sprites2),
	alien(&sprites2.alien),
	powerUp(sprites2.powerUps),
	rocket(sprites2.rocket, sprites2.engine, sprites2.shield),
	scorecounter(sprites2.largeChars, Vei2(700, 20), Colors::Green)
	
{
	field.initLevel(rng, 1);
	for (int counter = 0; counter < PowerUp::nPowers; ++counter)
	{
		powerUpSwitches[counter] = false;
	}

}




bool Space::endSequenceComplete(float dt, Timer& timer)
{
	bullets.updateBullets(dt, timer, field, rocket, alien);
	rocket.updateNoInput(dt);
	switch (stageLevelEnd)
	{
	case StageLevelEnd::stop:
		if (rocket.stopMovement(dt))
		{
			stageLevelEnd = StageLevelEnd::rotate;
		}
		break;

	case StageLevelEnd::rotate:
		if (rocket.rotateLeft())
		{
			stageLevelEnd = StageLevelEnd::flyLeft;
		}
		break;
	case StageLevelEnd::flyLeft:
		if (rocket.flyToLeft())
		{
			stageLevelEnd = StageLevelEnd::stop;
			return true;
		}
	}
	return sequenceEmergencyStop;
}

bool Space::rocketDestroyed()
{
	
	return (rocket.getShieldEnergy() <= 0);
}



