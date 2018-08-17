#pragma once
class Timer
{
private:
	static constexpr int nTimer = 10;
	float currentTime[nTimer];
	float endTime[nTimer];
	bool* stateToChange[nTimer];
	bool active[nTimer];

public:
	void init(bool* stateToChange_in, float timeMS);
	void terminate(bool* stateToChange_in);
	void addTime(float dt);
	Timer();

};