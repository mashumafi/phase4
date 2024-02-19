#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_MOBILITY_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_MOBILITY_EVALUATOR_H

#include <phase4/engine/ai/score/tapered_evaluation.h>

#include <phase4/engine/board/position.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/operators.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>

#include <cstdint>

namespace phase4::engine::ai::score::evaluators {

class MobilityEvaluator {
public:
	static inline int32_t evaluate(const board::Position &position, int32_t openingPhase, int32_t endingPhase, common::Bitset &fieldsAttackedByWhite, common::Bitset &fieldsAttackedByBlack) {
		const int32_t whiteEvaluation = evaluate(position, common::PieceColor::WHITE, openingPhase, endingPhase, fieldsAttackedByWhite);
		const int32_t blackEvaluation = evaluate(position, common::PieceColor::BLACK, openingPhase, endingPhase, fieldsAttackedByBlack);
		return whiteEvaluation - blackEvaluation;
	}

private:
	static inline int32_t evaluate(const board::Position &position, common::PieceColor color, int32_t openingPhase, int32_t endingPhase, common::Bitset &fieldsAttackedByColor) {
		const auto [centerMobility, outsideMobility] = board::Operators::getMobility(position, color, fieldsAttackedByColor);

		const int32_t centerMobilityScore = centerMobility * board::EvaluationConstants::CENTER_MOBILITY_MODIFIER;
		const int32_t outsideMobilityScore = outsideMobility * board::EvaluationConstants::OUTSIDE_MOBILITY_MODIFIER;

		const int32_t openingScore = centerMobilityScore + outsideMobilityScore;
		return TaperedEvaluation::adjustToPhase(openingScore, 0, openingPhase, endingPhase);
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
