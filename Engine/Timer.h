#pragma once
class Timer
{
private:
	
	class SingleTimer
	{
	private:
		float currentTime;
		float endTime;
		bool* stateToChange;
		SingleTimer* nextSingleTimerPtr;

	public:
		SingleTimer* separate();
		void reLink(SingleTimer* toReplace, SingleTimer* replaceWith);
		SingleTimer* getNext();
		SingleTimer(float duration_in, bool* stateToCHanege_in, SingleTimer* nextSingleTimerPtr_in);
		SingleTimer();
		bool timeOver(float dt);
		bool terminate(bool* stateToChangeSearch);

	};
	SingleTimer* topSingleTimerPtr;
	void deleteSingleTimer(SingleTimer* toDelete);
public:
	void init(bool* stateToChange_in, float timeMS);
	void terminate(bool* stateToChange_in);
	void addTime(float dt);
	Timer();

};