#ifndef PHASE4_ENGINE_AI_SCORE_TAPERED_EVALUATION_H
#define PHASE4_ENGINE_AI_SCORE_TAPERED_EVALUATION_H

#include <phase4/engine/common/position_constants.h>

namespace phase4::engine::ai::score::evaluators {

class TaperedEvaluation {
public:
	static inline int32_t adjustToPhase(int openingScore, int endingScore, int openingPhase, int endingPhase) {
		return (openingScore * openingPhase + endingScore * endingPhase) / common::PositionConstants::PHASE_RESOLUTION;
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
