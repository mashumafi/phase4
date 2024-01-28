#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_MATERIAL_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_MATERIAL_EVALUATOR_H

#include <phase4/engine/board/position.h>

#include <phase4/engine/common/piece_color.h>

namespace phase4::engine::ai::score::evaluators {

class MaterialEvaluator {
public:
	static inline int32_t evaluate(board::Position &position) {
		return position.m_material[common::PieceColor::WHITE.get_raw_value()] - position.m_material[common::PieceColor::BLACK.get_raw_value()];
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
