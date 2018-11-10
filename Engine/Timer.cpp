#include "Timer.h"
#include <assert.h>

void Timer::deleteSingleTimer(SingleTimer * toDelete)
{
	assert(toDelete != 0);
	SingleTimer* oldNext = toDelete->separate();
	if (toDelete == topSingleTimerPtr)
	{
		topSingleTimerPtr = oldNext;
		delete toDelete;
	}
	else
	{
		topSingleTimerPtr->reLink(toDelete, oldNext);
		delete toDelete;
	}
}

void Timer::init(bool * stateToChange_in, float timeMS)
{
	topSingleTimerPtr = new SingleTimer(timeMS, stateToChange_in, topSingleTimerPtr);
}

void Timer::terminate(bool * stateToChange_in)
{
	SingleTimer* nextSingleTimer = topSingleTimerPtr;

	while (nextSingleTimer != nullptr)
	{
		SingleTimer* currentSingleTimer = nextSingleTimer;
		nextSingleTimer = currentSingleTimer->getNext();
		if (currentSingleTimer->terminate(stateToChange_in))
		{
			deleteSingleTimer(currentSingleTimer);
			break;
		}
	}
}

void Timer::addTime(float dt)
{
	SingleTimer* nextSingleTimer = topSingleTimerPtr;

	while (nextSingleTimer != nullptr)
	{
		SingleTimer* currentSingleTimer = nextSingleTimer;
		nextSingleTimer = currentSingleTimer->getNext();
		if (currentSingleTimer->timeOver(dt))
		{
			deleteSingleTimer(currentSingleTimer);
		}
	}
}

Timer::Timer()
{
	topSingleTimerPtr = nullptr;
}

Timer::SingleTimer * Timer::SingleTimer::separate()
{
	SingleTimer * temp = nextSingleTimerPtr;
	nextSingleTimerPtr = nullptr;
	return temp;;
}

void Timer::SingleTimer::reLink(SingleTimer * toReplace, SingleTimer * replaceWith)
{
	if (nextSingleTimerPtr == toReplace)
	{
		nextSingleTimerPtr = replaceWith;
	}
	else if (nextSingleTimerPtr != 0)
	{
		nextSingleTimerPtr->reLink(toReplace, replaceWith);
	}
}

Timer::SingleTimer * Timer::SingleTimer::getNext()
{
	return nextSingleTimerPtr;
}

Timer::SingleTimer::SingleTimer(float duration_in, bool * stateToChanege_in, SingleTimer * nextSingleTimerPtr_in)
	:
	endTime (duration_in),
	stateToChange(stateToChanege_in),
	nextSingleTimerPtr (nextSingleTimerPtr_in)
{
	currentTime = 0.0f;
}

Timer::SingleTimer::SingleTimer()
{
	delete nextSingleTimerPtr;
	nextSingleTimerPtr = nullptr;
}

bool Timer::SingleTimer::timeOver(float dt)
{
	currentTime += dt;
	if (currentTime >= endTime)
	{
		*stateToChange = !*stateToChange;
		return true;
	}
	else
	{
		return false;
	}
}

bool Timer::SingleTimer::terminate(bool * stateToChangeSearch)
{
	if (stateToChange == stateToChangeSearch)
	{
		*stateToChange = !*stateToChange;
		return true;
	}
	else
	{
		return false;
	}
}
