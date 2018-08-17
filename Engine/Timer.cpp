#include "Timer.h"

void Timer::init(bool * stateToChange_in, float timeMS)
{
	//1. Find free timer
	int counter = 0;
	for (; active[counter] && counter < nTimer - 1; ++counter);

	//2. Initialize if last element is free
	if (!active[counter])
	{
		active[counter] = true;
		currentTime[counter] = 0.0f;
		endTime[counter] = timeMS;
		stateToChange[counter] = stateToChange_in;
	}

	
	
}

void Timer::terminate(bool * stateToChange_in)
{
	for (int counter = 0; counter < nTimer; ++counter)
	{
		if (stateToChange[counter] == stateToChange_in)
		{
			*stateToChange[counter] = !*stateToChange[counter];
			active[counter] = false;
			stateToChange[counter] = 0;
			break;
		}
	}

}

void Timer::addTime(float dt)
{
	for (int counter = 0; counter < nTimer; ++counter)
	{
		if (active[counter])
		{
			currentTime[counter] += dt;
			if (currentTime[counter] >= endTime[counter])
			{
				*stateToChange[counter] = !*stateToChange[counter];
				active[counter] = false;
				stateToChange[counter] = 0;
			}
		}
	}
}

Timer::Timer()
{
	for (int counter = 0; counter < nTimer; ++counter)
	{
		currentTime[counter] = 0.0f;
		endTime[counter] = 0.0f;
		stateToChange[counter] = 0;
		active[counter] = false;
	}
}
