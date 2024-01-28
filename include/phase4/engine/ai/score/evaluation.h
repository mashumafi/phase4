#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATION_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATION_H

#include <phase4/engine/ai/score/evaluation_statistics.h>

#include <phase4/engine/ai/score/evaluators/material_evaluator.h>
#include <phase4/engine/ai/score/evaluators/pawn_structure_evaluator.h>
#include <phase4/engine/ai/score/evaluators/position_evaluator.h>

#include <phase4/engine/board/position.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/position_constants.h>

namespace phase4::engine::ai::score {

class Evaluation {
public:
	static int32_t evaluate(board::Session& session, bool enableCache, EvaluationStatistics& statistics) {
		int32_t openingPhase = session.m_position.getPhaseRatio();
		int32_t endingPhase = common::PositionConstants::PHASE_RESOLUTION - openingPhase;

		int32_t result = evaluators::MaterialEvaluator::evaluate(session.m_position);
		result += enableCache ? evaluators::PawnStructureEvaluator::evaluate(session, statistics, openingPhase, endingPhase) : evaluators::PawnStructureEvaluator::evaluateWithoutCache(session, statistics, openingPhase, endingPhase);
		result += evaluators::PositionEvaluator::evaluate(session.m_position, openingPhase, endingPhase);

		// TODO: the rest
		return result;
	}

	static int32_t fastEvaluate(board::Session& session, EvaluationStatistics& statistics) {
		int32_t openingPhase = session.m_position.getPhaseRatio();
		int32_t endingPhase = common::PositionConstants::PHASE_RESOLUTION - openingPhase;

		int32_t result = evaluators::MaterialEvaluator::evaluate(session.m_position);
		result += evaluators::PawnStructureEvaluator::evaluate(session, statistics, openingPhase, endingPhase);
		result += evaluators::PositionEvaluator::evaluate(session.m_position, openingPhase, endingPhase);
		return session.m_position.m_colorToMove == common::PieceColor::WHITE ? result : -result;
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
