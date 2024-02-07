#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATION_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATION_H

#include <phase4/engine/ai/score/evaluation_statistics.h>

#include <phase4/engine/ai/score/evaluators/bishop_evaluator.h>
#include <phase4/engine/ai/score/evaluators/king_safety_evaluator.h>
#include <phase4/engine/ai/score/evaluators/material_evaluator.h>
#include <phase4/engine/ai/score/evaluators/mobility_evaluator.h>
#include <phase4/engine/ai/score/evaluators/pawn_structure_evaluator.h>
#include <phase4/engine/ai/score/evaluators/position_evaluator.h>
#include <phase4/engine/ai/score/evaluators/rook_evaluator.h>

#include <phase4/engine/board/position.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/position_constants.h>

namespace phase4::engine::ai::score {

class Evaluation {
public:
	static inline int32_t evaluate(board::Session &session, EvaluationStatistics &statistics) {
		using namespace evaluators;

		const int32_t openingPhase = session.position().getPhaseRatio();
		const int32_t endingPhase = common::PositionConstants::PHASE_RESOLUTION - openingPhase;

		int32_t result = MaterialEvaluator::evaluate(session.position());
		result += PawnStructureEvaluator::evaluate(session, statistics, openingPhase, endingPhase);
		result += PositionEvaluator::evaluate(session.position(), openingPhase, endingPhase);

		if (endingPhase != common::PositionConstants::PHASE_RESOLUTION) {
			common::Bitset fieldsAttackedByWhite = 0;
			common::Bitset fieldsAttackedByBlack = 0;

			result += MobilityEvaluator::evaluate(session.position(), openingPhase, endingPhase, fieldsAttackedByWhite, fieldsAttackedByBlack);
			result += KingSafetyEvaluator::evaluate(session.position(), openingPhase, endingPhase, fieldsAttackedByWhite, fieldsAttackedByBlack);
			result += RookEvaluator::evaluate(session.position(), openingPhase, endingPhase);
			result += BishopEvaluator::evaluate(session.position(), openingPhase, endingPhase);
		}

		return result;
	}

	/// @brief evaluate a position, results will match session evaluation without caching
	/// @param position position to evaluate
	/// @param statistics statistics tracking
	/// @return result of evaluation
	static inline int32_t evaluate(const board::Position &position) {
		using namespace evaluators;

		const int32_t openingPhase = position.getPhaseRatio();
		const int32_t endingPhase = common::PositionConstants::PHASE_RESOLUTION - openingPhase;

		int32_t result = MaterialEvaluator::evaluate(position);
		result += PawnStructureEvaluator::evaluateWithoutCache(position, openingPhase, endingPhase);
		result += PositionEvaluator::evaluate(position, openingPhase, endingPhase);

		if (endingPhase != common::PositionConstants::PHASE_RESOLUTION) {
			common::Bitset fieldsAttackedByWhite = 0;
			common::Bitset fieldsAttackedByBlack = 0;

			result += MobilityEvaluator::evaluate(position, openingPhase, endingPhase, fieldsAttackedByWhite, fieldsAttackedByBlack);
			result += KingSafetyEvaluator::evaluate(position, openingPhase, endingPhase, fieldsAttackedByWhite, fieldsAttackedByBlack);
			result += RookEvaluator::evaluate(position, openingPhase, endingPhase);
			result += BishopEvaluator::evaluate(position, openingPhase, endingPhase);
		}

		return result;
	}

	static inline int32_t fastEvaluate(board::Session &session, EvaluationStatistics &statistics) {
		using namespace evaluators;

		const int32_t openingPhase = session.position().getPhaseRatio();
		const int32_t endingPhase = common::PositionConstants::PHASE_RESOLUTION - openingPhase;

		int32_t result = MaterialEvaluator::evaluate(session.position());
		result += PawnStructureEvaluator::evaluate(session, statistics, openingPhase, endingPhase);
		result += PositionEvaluator::evaluate(session.position(), openingPhase, endingPhase);
		return session.position().m_colorToMove == common::PieceColor::WHITE ? result : -result;
	}
};

} //namespace phase4::engine::ai::score

#endif
