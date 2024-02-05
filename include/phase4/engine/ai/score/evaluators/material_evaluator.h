#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_MATERIAL_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_MATERIAL_EVALUATOR_H

#include <phase4/engine/board/position.h>

#include <phase4/engine/common/piece_color.h>

namespace phase4::engine::ai::score::evaluators {

class MaterialEvaluator {
public:
	static inline int32_t evaluate(const board::Position &position) {
		const int32_t whiteEvaluation = position.material(common::PieceColor::WHITE);
		const int32_t blackEvaluation = position.material(common::PieceColor::BLACK);
		return whiteEvaluation - blackEvaluation;
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
