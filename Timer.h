#pragma once
class Timer
{
private:
	int timer_;
	int timeMem_;
public:
	Timer();
	Timer(int timer);
	bool CountDown();
	void Reset() { timer_ = timeMem_; };
};