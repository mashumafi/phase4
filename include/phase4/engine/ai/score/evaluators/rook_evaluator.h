#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_ROOK_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_ROOK_EVALUATOR_H

#include <phase4/engine/ai/score/tapered_evaluation.h>

#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/patterns/file_pattern_generator.h>

#include <phase4/engine/score/evaluation_constants.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>

#include <cstdint>

namespace phase4::engine::ai::score::evaluators {

class RookEvaluator {
public:
	static inline int32_t evaluate(const board::Position &position, int32_t openingPhase, int32_t endingPhase) {
		const int32_t whiteEvaluation = evaluate(position, common::PieceColor::WHITE, openingPhase, endingPhase);
		const int32_t blackEvaluation = evaluate(position, common::PieceColor::BLACK, openingPhase, endingPhase);
		return whiteEvaluation - blackEvaluation;
	}

private:
	static inline int32_t evaluate(const board::Position &position, common::PieceColor color, int32_t openingPhase, int32_t endingPhase) {
		int32_t doubledRooks = 0;
		int32_t rooksOnOpenFile = 0;
		const common::PieceColor enemyColor = color.invert();

		common::Bitset rooks = position.m_colorPieceMasks[color.get_raw_value()][common::PieceType::ROOK.get_raw_value()];
		while (rooks != 0) {
			const common::Bitset lsb = rooks.getLsb();
			const common::Square field(lsb.bitScan());
			rooks = rooks.popLsb();

			const common::Bitset file = moves::patterns::FilePatternGenerator::getPatternForField(field) | lsb;
			const common::Bitset rooksOnFile = file & position.m_colorPieceMasks[color.get_raw_value()][common::PieceType::ROOK.get_raw_value()];
			const common::Bitset friendlyPawnsOnFile = file & position.m_colorPieceMasks[color.get_raw_value()][common::PieceType::PAWN.get_raw_value()];
			const common::Bitset enemyPawnsOnFile = file & position.m_colorPieceMasks[enemyColor.get_raw_value()][common::PieceType::PAWN.get_raw_value()];

			if (rooksOnFile.count() > 1) {
				// We don't assume that there will be more than two rooks - even if, then this color is probably anyway winning
				doubledRooks = 1;
			}

			if (friendlyPawnsOnFile == 0 && enemyPawnsOnFile == 0) {
				rooksOnOpenFile++;
			}
		}

		const int32_t doubledRooksOpeningScore = doubledRooks * engine::score::EvaluationConstants::DOUBLED_ROOKS;
		const int32_t rooksOnOpenFileOpeningScore = rooksOnOpenFile * engine::score::EvaluationConstants::ROOK_ON_OPEN_FILE;
		const int32_t openingScore = doubledRooksOpeningScore + rooksOnOpenFileOpeningScore;

		return TaperedEvaluation::adjustToPhase(openingScore, 0, openingPhase, endingPhase);
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
