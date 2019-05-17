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
#include "Vec2.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	space(sprites2),
	storyline("Data/Story.txt", sprites2.smallChars, Colors::Green, RectI(20, 780, 20, 580)),
	ending("Data/Ending.txt", sprites2.smallChars, Colors::Green, RectI(20, 780, 20, 580)),
	highscores(sprites2.largeChars)
{	
}
	

void Game::Go()
{
	gfx.BeginFrame();
	float elapsedTime = ft.Mark();
	while (elapsedTime > 0.0f)
	{
		float dt = std::min(0.15f, elapsedTime);
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
	case Status::introShowing:
		storyline.draw(gfx);
		break;
	
	case Status::pause:
		{
		space.draw(gfx);
		std::string pauseText;
		if (currentLevel < maxLevel)
		{
			pauseText = " Stage " + std::to_string(currentLevel) + "\n\nGet Ready!";
		}
		else
		{
			pauseText = "Final Stage \n\n Get Ready!";
		}

		sprites2.largeChars.printText(pauseText, Vei2(320.0f, 286.0f), gfx, Colors::Green);
		break;
		}
		
	case Status::levelRunning:
		space.draw(gfx);
		break;

	case Status::levelEnding:
		{
		std::string endingText = "Leaving Sector...";
		space.draw(gfx);
		sprites2.largeChars.printText(endingText, Vei2(264.0f, 286.0f), gfx, Colors::Green);
		break;
		}
		
	case Status::lost:
		{
		std::string lostText = "Game Over!";
		space.draw(gfx);
		sprites2.largeChars.printText(lostText, Vei2(320.0f, 286.0f), gfx, Colors::Green);
		break;
		}

	case Status::endingShowing:
		ending.draw(gfx);
		break;

	case Status::highscores_enter:
		{
		space.draw(gfx);
		highscores.display(gfx);
		}
	case Status::highscores_show:
		{
		space.draw(gfx);
		highscores.display(gfx);
		}
		
	}
}


void Game::UpdateModel(float dt)

{
	switch (gameStatus)
	{
	case Status::introShowing:
		storyline.update(dt);
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			gameStatus = Status::pause;
			timer.init(&pauseMode, 2);
		}
		break;
	
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
			timer.init(&lostTimerOver, 3);
			space.gameOver = true;
		}

		else if (space.levelComplete(timer))
		{
			gameStatus = Status::levelEnding;
		}
		break;

	case Status::levelEnding:
		if (space.endSequenceComplete(dt, timer))
		{
			++currentLevel;
			if (currentLevel > maxLevel)
			{
				space.gameOver = true;
				gameStatus = Status::endingShowing;
			}
			else
			{
				gameStatus = Status::pause;
				space.initLevel(currentLevel, timer);
				pauseMode = true;
				timer.init(&pauseMode, 2);
			}
		
		}
		break;

	case Status::lost:
		space.update(wnd.kbd, dt, timer);
		if (lostTimerOver)
		{
			if (highscores.checkNew(space.getScore()))
			{
				gameStatus = Status::highscores_enter;
				highscores.add(currentLevel, space.getScore());
				wnd.kbd.FlushChar();

			}
			else
			{
				gameStatus = Status::highscores_show;
			}
		}
		break;


	case Status::endingShowing:
		ending.update(dt);
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			if (highscores.checkNew(space.getScore()))
			{
				gameStatus = Status::highscores_enter;
				highscores.add(currentLevel, space.getScore());
				wnd.kbd.FlushChar();

			}
			else
			{
				gameStatus = Status::highscores_show;
			}
		}
		break;

	case Status::highscores_enter:
		space.update(dt, timer);
	
		if (highscores.addNametoNew(wnd.kbd.ReadChar()))
		{
			highscores.save();
			gameStatus = Status::highscores_show;
		}
		break;
	
	case Status::highscores_show:
		space.update(dt, timer);
		break;
	


	}
	




}




