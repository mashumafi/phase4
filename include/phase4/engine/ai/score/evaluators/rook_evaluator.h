#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_ROOK_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_ROOK_EVALUATOR_H

#include <phase4/engine/ai/score/tapered_evaluation.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/patterns/file_pattern_generator.h>

#include <phase4/engine/common/bitboard.h>
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

		common::Bitboard rooks = position.colorPieceMask(color, common::PieceType::ROOK);
		while (rooks != 0) {
			const common::Bitboard lsb = rooks.getLsb();
			const common::Square field(lsb.bitScan());
			rooks = rooks.popLsb();

			const common::Bitboard file = moves::patterns::FilePatternGenerator::getPatternForField(field) | lsb;
			const common::Bitboard rooksOnFile = file & position.colorPieceMask(color, common::PieceType::ROOK);
			const common::Bitboard friendlyPawnsOnFile = file & position.colorPieceMask(color, common::PieceType::PAWN);
			const common::Bitboard enemyPawnsOnFile = file & position.colorPieceMask(enemyColor, common::PieceType::PAWN);

			if (rooksOnFile.fastCount() > 1) {
				// We don't assume that there will be more than two rooks - even if, then this color is probably anyway winning
				doubledRooks = 1;
			}

			if (friendlyPawnsOnFile == 0 && enemyPawnsOnFile == 0) {
				rooksOnOpenFile++;
			}
		}

		const int32_t doubledRooksOpeningScore = doubledRooks * board::EvaluationConstants::DOUBLED_ROOKS;
		const int32_t rooksOnOpenFileOpeningScore = rooksOnOpenFile * board::EvaluationConstants::ROOK_ON_OPEN_FILE;
		const int32_t openingScore = doubledRooksOpeningScore + rooksOnOpenFileOpeningScore;

		return TaperedEvaluation::adjustToPhase(openingScore, 0, openingPhase, endingPhase);
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
