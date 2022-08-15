#include "Timer.h"

Timer::Timer()
{
	timer_ = 0;
	timeMem_ = 0;
}

Timer::Timer(int timer)
{
	timer_ = timer;
	timeMem_ = timer;
}

bool Timer::CountDown()
{
	if (--timer_ <= 0) 
	{ 
		timer_ = timeMem_;;
		return true; 
	}
	return false;
}