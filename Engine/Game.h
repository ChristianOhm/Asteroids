/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <chrono>
#include "Rocket.h"
#include "FrameTimer.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "Field.h"
#include "Sound.h"
#include "Alien.h"
#include <random>
#include "Timer.h"
#include "Space.h"
#include"Sprites2.h"
#include "Scrolltext.h"
#include <string>
#include "Highscores.h"



class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel(float dt);
	enum class Status
	{
		introShowing,
		levelRunning,
		pause,
		levelEnding,
		lost,
		highscores_enter,
		highscores_show
	};

	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	Sprites2 sprites2;
	FrameTimer ft;
	Space space;
	Timer timer;
	Scrolltext storyline;
	bool pauseMode = true;
	Status gameStatus = Status::introShowing;
	int currentLevel = 1;
	Highscores highscores;
	bool lostTimerOver = false;
	/********************************/
	/*  User Variables              */
	/********************************/
};