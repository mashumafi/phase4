#ifndef PHASE4_ENGINE_AI_TIME_TIME_SCHEDULER_H
#define PHASE4_ENGINE_AI_TIME_TIME_SCHEDULER_H

#include <phase4/engine/common/math.h>

#include <cstdint>

namespace phase4::engine::ai::time {

class TimeScheduler {
public:
	static int32_t calculateTimeForMove(int32_t remainingTime, int32_t incTime, int32_t moveNumber) {
		return remainingTime / common::Math::max_int32(20, 40 - moveNumber) + incTime;
	}
};

} //namespace phase4::engine::ai::time

#endif
