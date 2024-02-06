#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_BISHOP_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_BISHOP_EVALUATOR_H

#include <phase4/engine/ai/score/tapered_evaluation.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/square.h>

#include <cstdint>

namespace phase4::engine::ai::score::evaluators {
class BishopEvaluator {
public:
	static inline int32_t evaluate(const board::Position &position, int32_t openingPhase, int32_t endingPhase) {
		const int32_t whiteEvaluation = evaluate(position, common::PieceColor::WHITE, openingPhase, endingPhase);
		const int32_t blackEvaluation = evaluate(position, common::PieceColor::BLACK, openingPhase, endingPhase);
		return whiteEvaluation - blackEvaluation;
	}

private:
	static inline int32_t evaluate(const board::Position &position, common::PieceColor color, int32_t openingPhase, int32_t endingPhase) {
		int32_t pairOfBishops = 0;
		if (position.colorPieceMask(color, common::PieceType::BISHOP).fastCount() > 1) {
			pairOfBishops = 1;
		}

		int32_t fianchettos = 0;
		int32_t fianchettosWithoutBishop = 0;
		const common::Bitset kingPattern = color == common::PieceColor::WHITE ? WHITE_KING_FIANCHETTO_PATTERN : BLACK_KING_FIANCHETTO_PATTEREN;
		const common::Bitset pawnsPattern = color == common::PieceColor::WHITE ? WHITE_PAWNS_FIANCHETTO_PATTERN : BLACK_PAWNS_FIANCHETTO_PATTERN;

		if (position.m_castlingDone[color.get_raw_value()] && (position.colorPieceMask(color, common::PieceType::KING) & kingPattern) != 0 && (position.colorPieceMask(color, common::PieceType::PAWN) & pawnsPattern) == pawnsPattern) {
			const common::Bitset bishopPattern = color == common::PieceColor::WHITE ? WHITE_BISHOP_FIANCHETTO_PATTERN : BLACK_BISHOP_FIANCHETTO_PATTERN;
			if ((position.colorPieceMask(color, common::PieceType::BISHOP) & bishopPattern) == bishopPattern) {
				fianchettos++;
			} else {
				fianchettosWithoutBishop++;
			}
		}

		const int32_t pairOfBishopsOpeningScore = pairOfBishops * board::EvaluationConstants::PAIR_OF_BISHOPS;
		const int32_t fianchettosScore = fianchettos * board::EvaluationConstants::FIANCHETTO;
		const int32_t fianchettosWithoutBishopScore = fianchettosWithoutBishop * board::EvaluationConstants::FIANCHETTO_WITHOUT_BISHOP;
		const int32_t openingScore = pairOfBishopsOpeningScore + fianchettosScore + fianchettosWithoutBishopScore;

		return TaperedEvaluation::adjustToPhase(openingScore, 0, openingPhase, endingPhase);
	}

	static constexpr common::Bitset WHITE_KING_FIANCHETTO_PATTERN = common::Square::F1.asBitboard() | common::Square::G1.asBitboard() | common::Square::H1.asBitboard();
	static constexpr common::Bitset WHITE_PAWNS_FIANCHETTO_PATTERN = common::Square::F2.asBitboard() | common::Square::G3.asBitboard() | common::Square::H2.asBitboard();
	static constexpr common::Bitset WHITE_BISHOP_FIANCHETTO_PATTERN = common::Square::G2.asBitboard();

	static constexpr common::Bitset BLACK_KING_FIANCHETTO_PATTEREN = common::Square::F8.asBitboard() | common::Square::G8.asBitboard() | common::Square::H8.asBitboard();
	static constexpr common::Bitset BLACK_PAWNS_FIANCHETTO_PATTERN = common::Square::F7.asBitboard() | common::Square::G6.asBitboard() | common::Square::H7.asBitboard();
	static constexpr common::Bitset BLACK_BISHOP_FIANCHETTO_PATTERN = common::Square::G7.asBitboard();
};

} //namespace phase4::engine::ai::score::evaluators

#endif
