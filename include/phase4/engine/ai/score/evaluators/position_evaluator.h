#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_POSITION_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_POSITION_EVALUATOR_H

#include <phase4/engine/ai/score/tapered_evaluation.h>

#include <phase4/engine/board/position.h>

#include <phase4/engine/common/game_phase.h>
#include <phase4/engine/common/piece_color.h>

namespace phase4::engine::ai::score::evaluators {

class PositionEvaluator {
public:
	static inline int32_t evaluate(const board::Position &position, int32_t openingPhase, int32_t endingPhase) {
		const int32_t whiteEvaluation = evaluate(position, common::PieceColor::WHITE, openingPhase, endingPhase);
		const int32_t blackEvaluation = evaluate(position, common::PieceColor::BLACK, openingPhase, endingPhase);
		;
		return whiteEvaluation - blackEvaluation;
	}

	static inline int32_t evaluate(const board::Position &position, common::PieceColor color, int32_t openingPhase, int32_t endingPhase) {
		const int32_t positionOpeningScore = position.positionEval(color, common::GamePhase::OPENING);
		const int32_t positionEndingScore = position.positionEval(color, common::GamePhase::ENDING);
		return TaperedEvaluation::adjustToPhase(positionOpeningScore, positionEndingScore, openingPhase, endingPhase);
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
