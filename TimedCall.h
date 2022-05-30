#pragma once
#include <functional>
class TimedCall {
  private:
	std::function<void(void)> fire_;
	uint32_t time_;
	bool isFinished = 0;

  public:
	TimedCall(std::function<void(void)> fire, uint32_t time);
	void Update();
	bool IsFinished() { return isFinished; }
};