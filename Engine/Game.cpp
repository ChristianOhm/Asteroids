/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Sprites.h"
#include "Vec2.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	timer.init(&pauseMode, 2);
}
	

void Game::Go()
{
	gfx.BeginFrame();
	float elapsedTime = ft.Mark();
	while (elapsedTime > 0.0f)
	{
		float dt = std::min(0.025f, elapsedTime);
		elapsedTime -= dt;
		timer.addTime(dt);
		UpdateModel(dt);

	}
	
	ComposeFrame();
	gfx.EndFrame();
}

void Game::ComposeFrame()
{

	switch (gameStatus)
	{
	case Status::pause:
		space.draw(gfx);
		Sprites::drawGetReady(gfx, { 275.0f, 200.0f });
		break;

	case Status::levelRunning:
		space.draw(gfx);
		break;

	case Status::levelEnding:
		space.draw(gfx);
		Sprites::drawLeavingSector(gfx, { 210.0f, 200.0f });
		break;

	case Status::lost:
		space.draw(gfx);
		Sprites::drawGameOver(gfx, { 275.0f, 200.0f });
	}
}


void Game::UpdateModel(float dt)

{
	switch (gameStatus)
	{
	case Status::pause:
		if (!pauseMode)
		{
			gameStatus = Status::levelRunning;
		}
		break;

	case Status::levelRunning:
		space.update(wnd.kbd, dt, timer);
		if (space.rocketDestroyed())
		{
			gameStatus = Status::lost;
		}

		else if (space.levelComplete())
		{
			gameStatus = Status::levelEnding;
		}
		break;

	case Status::levelEnding:
		if (space.endSequenceComplete(dt, timer))
		{
			gameStatus = Status::pause;
			++currentLevel;
			space.initLevel(currentLevel, timer);
			pauseMode = true;
			timer.init(&pauseMode, 2);
		}
		break;

	}



}




