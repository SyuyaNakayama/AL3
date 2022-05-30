#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> fire, uint32_t time) {
	fire_ = fire;
	time_ = time;
}

void TimedCall::Update() {
	if (isFinished) {
		return;
	}
	if (--time_ <= 0) {
		isFinished = 1;
		fire_();
	}
}