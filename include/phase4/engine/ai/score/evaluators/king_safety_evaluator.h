#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_KING_SAFETY_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_KING_SAFETY_EVALUATOR_H

#include <phase4/engine/ai/score/tapered_evaluation.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/patterns/file_pattern_generator.h>
#include <phase4/engine/moves/patterns/forward_box_pattern_generator.h>

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/math.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/square.h>

#include <cstdint>

namespace phase4::engine::ai::score::evaluators {

class KingSafetyEvaluator {
public:
	static inline int32_t evaluate(const board::Position &position, int32_t openingPhase, int32_t endingPhase, common::Bitboard fieldsAttackedByWhite, common::Bitboard fieldsAttackedByBlack) {
		const int32_t whiteEvaluation = evaluate(position, common::PieceColor::WHITE, openingPhase, endingPhase, fieldsAttackedByBlack);
		const int32_t blackEvaluation = evaluate(position, common::PieceColor::BLACK, openingPhase, endingPhase, fieldsAttackedByWhite);
		return whiteEvaluation - blackEvaluation;
	}

private:
	static inline int32_t evaluate(const board::Position &position, common::PieceColor color, int32_t openingPhase, int32_t endingPhase, common::Bitboard fieldsAttackedByEnemy) {
		const common::Bitboard king = position.colorPieceMask(color, common::PieceType::KING);
		const common::Square kingField(king.bitScan());
		const common::FieldIndex kingPosition = kingField.asFieldIndex();
		const common::Bitboard fieldsAroundKing = moves::patterns::ForwardBoxPatternGenerator::getPattern(color, kingField);

		const common::Bitboard attackedFieldsAroundKing = fieldsAroundKing & fieldsAttackedByEnemy;
		const int8_t attackersCount = attackedFieldsAroundKing.fastCount();
		const int32_t attackersCountOpeningScore = attackersCount * board::EvaluationConstants::KING_IN_DANGER;

		int32_t pawnShieldOpeningScore = 0;
		int32_t openFilesNextToKingScore = 0;
		if (position.castlingDone(color)) {
			const common::Bitboard pawnsNearKing = fieldsAroundKing & position.colorPieceMask(color, common::PieceType::PAWN);
			const int32_t pawnShield = pawnsNearKing.fastCount();

			pawnShieldOpeningScore = pawnShield * board::EvaluationConstants::PAWN_SHIELD;

			const int16_t openFileCheckFrom = common::Math::max_int16(0, kingPosition.x - 1);
			const int16_t openFileCheckTo = common::Math::min_int16(7, kingPosition.x + 1);
			for (uint16_t file = openFileCheckFrom; file <= openFileCheckTo; ++file) {
				if ((moves::patterns::FilePatternGenerator::getPatternForFile(7 - file) & position.colorPieceMask(color, common::PieceType::PAWN)) == 0) {
					openFilesNextToKingScore += board::EvaluationConstants::OPEN_FILE_NEXT_TO_KING;
				}
			}
		}

		const int32_t openingScore = pawnShieldOpeningScore + attackersCountOpeningScore + openFilesNextToKingScore;
		return TaperedEvaluation::adjustToPhase(openingScore, 0, openingPhase, endingPhase);
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
