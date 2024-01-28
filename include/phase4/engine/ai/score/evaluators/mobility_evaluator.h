#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_MOBILITY_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_MOBILITY_EVALUATOR_H

#include <phase4/engine/ai/score/tapered_evaluation.h>

#include <phase4/engine/board/position.h>

#include <phase4/engine/board/operators/bishop_operator.h>
#include <phase4/engine/board/operators/knight_operator.h>
#include <phase4/engine/board/operators/queen_operator.h>
#include <phase4/engine/board/operators/rook_operator.h>

#include <phase4/engine/score/evaluation_constants.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>

#include <cstdint>

namespace phase4::engine::ai::score::evaluators {

class MobilityEvaluator {
public:
	static int32_t evaluate(const board::Position &position, int32_t openingPhase, int32_t endingPhase, common::Bitset &fieldsAttackedByWhite, common::Bitset &fieldsAttackedByBlack) {
		int32_t whiteEvaluation = evaluate(position, common::PieceColor::WHITE, openingPhase, endingPhase, fieldsAttackedByWhite);
		int32_t blackEvaluation = evaluate(position, common::PieceColor::BLACK, openingPhase, endingPhase, fieldsAttackedByBlack);
		return whiteEvaluation - blackEvaluation;
	}

	static int32_t evaluate(const board::Position &position, common::PieceColor color, int32_t openingPhase, int32_t endingPhase, common::Bitset &fieldsAttackedByColor) {
		const auto [knightCenter, knightOutside] = board::operators::KnightOperator::getMobility(position, color, fieldsAttackedByColor);
		const auto [bishopCenter, bishopOutside] = board::operators::BishopOperator::getMobility(position, color, fieldsAttackedByColor);
		const auto [rookCenter, rookOutside] = board::operators::RookOperator::getMobility(position, color, fieldsAttackedByColor);
		const auto [queenCenter, queenOutside] = board::operators::QueenOperator::getMobility(position, color, fieldsAttackedByColor);

		const int32_t centerMobility = knightCenter + bishopCenter + rookCenter + queenCenter;
		const int32_t centerMobilityScore = centerMobility * engine::score::EvaluationConstants::CENTER_MOBILITY_MODIFIER;

		const int32_t outsideMobility = knightOutside + bishopOutside + rookOutside + queenOutside;
		const int32_t outsideMobilityScore = outsideMobility * engine::score::EvaluationConstants::OUTSIDE_MOBILITY_MODIFIER;

		const int32_t openingScore = centerMobilityScore + outsideMobilityScore;
		return TaperedEvaluation::adjustToPhase(openingScore, 0, openingPhase, endingPhase);
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
