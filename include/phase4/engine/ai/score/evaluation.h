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
	static int32_t evaluate(board::Session &session, bool enableCache, EvaluationStatistics &statistics) {
		using namespace evaluators;

		const int32_t openingPhase = session.m_position.getPhaseRatio();
		const int32_t endingPhase = common::PositionConstants::PHASE_RESOLUTION - openingPhase;

		int32_t result = MaterialEvaluator::evaluate(session.m_position);
		result += enableCache ? PawnStructureEvaluator::evaluate(session, statistics, openingPhase, endingPhase) : PawnStructureEvaluator::evaluateWithoutCache(session, statistics, openingPhase, endingPhase);
		result += PositionEvaluator::evaluate(session.m_position, openingPhase, endingPhase);

		if (endingPhase != common::PositionConstants::PHASE_RESOLUTION) {
			common::Bitset fieldsAttackedByWhite = 0ul;
			common::Bitset fieldsAttackedByBlack = 0ul;

			result += MobilityEvaluator::evaluate(session.m_position, openingPhase, endingPhase, fieldsAttackedByWhite, fieldsAttackedByBlack);
			result += KingSafetyEvaluator::evaluate(session.m_position, openingPhase, endingPhase, fieldsAttackedByWhite, fieldsAttackedByBlack);
			result += RookEvaluator::evaluate(session.m_position, openingPhase, endingPhase);
			result += BishopEvaluator::evaluate(session.m_position, openingPhase, endingPhase);
		}

		return result;
	}

	static int32_t fastEvaluate(board::Session &session, EvaluationStatistics &statistics) {
		using namespace evaluators;

		int32_t openingPhase = session.m_position.getPhaseRatio();
		int32_t endingPhase = common::PositionConstants::PHASE_RESOLUTION - openingPhase;

		int32_t result = MaterialEvaluator::evaluate(session.m_position);
		result += PawnStructureEvaluator::evaluate(session, statistics, openingPhase, endingPhase);
		result += PositionEvaluator::evaluate(session.m_position, openingPhase, endingPhase);
		return session.m_position.m_colorToMove == common::PieceColor::WHITE ? result : -result;
	}
};

} //namespace phase4::engine::ai::score

#endif
