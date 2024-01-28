#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_POSITION_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_POSITION_EVALUATOR_H

#include <phase4/engine/ai/score/tapered_evaluation.h>

#include <phase4/engine/board/position.h>

#include <phase4/engine/common/game_phase.h>
#include <phase4/engine/common/piece_color.h>

namespace phase4::engine::ai::score::evaluators {

class PositionEvaluator {
public:
	static inline int32_t evaluate(board::Position &position, int openingPhase, int endingPhase) {
		return evaluate(position, common::PieceColor::WHITE, openingPhase, endingPhase) -
				evaluate(position, common::PieceColor::BLACK, openingPhase, endingPhase);
	}

	static inline int32_t evaluate(board::Position &position, common::PieceColor color, int openingPhase, int endingPhase) {
		int32_t positionOpeningScore = position.m_positionEval[color.get_raw_value()][common::GamePhase::OPENING];
		int32_t positionEndingScore = position.m_positionEval[color.get_raw_value()][common::GamePhase::ENDING];
		return TaperedEvaluation::adjustToPhase(positionOpeningScore, positionEndingScore, openingPhase, endingPhase);
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
