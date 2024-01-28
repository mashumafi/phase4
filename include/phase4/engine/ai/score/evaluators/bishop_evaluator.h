#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_BISHOP_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_BISHOP_EVALUATOR_H

#include <phase4/engine/ai/score/tapered_evaluation.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>

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
		if (position.m_colorPieceMasks[color.get_raw_value()][common::PieceType::BISHOP.get_raw_value()].count() > 1) {
			pairOfBishops = 1;
		}

		int32_t fianchettos = 0;
		int32_t fianchettosWithoutBishop = 0;
		const common::Bitset kingPattern = color == common::PieceColor::WHITE ? WhiteKingFianchettoPattern : BlackKingFianchettoPatteren;
		const common::Bitset pawnsPattern = color == common::PieceColor::WHITE ? WhitePawnsFianchettoPattern : BlackPawnsFianchettoPattern;
		const common::Bitset bishopPattern = color == common::PieceColor::WHITE ? WhiteBishopFianchettoPattern : BlackBishopFianchettoPattern;

		if (position.m_castlingDone[color.get_raw_value()] && (position.m_colorPieceMasks[color.get_raw_value()][common::PieceType::KING.get_raw_value()] & kingPattern) != 0 && (position.m_colorPieceMasks[color.get_raw_value()][common::PieceType::PAWN.get_raw_value()] & pawnsPattern) == pawnsPattern) {
			if ((position.m_colorPieceMasks[color.get_raw_value()][common::PieceType::BISHOP.get_raw_value()] & bishopPattern) == bishopPattern) {
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

	static constexpr common::Bitset WhiteKingFianchettoPattern = 7;
	static constexpr common::Bitset WhitePawnsFianchettoPattern = 132352;
	static constexpr common::Bitset WhiteBishopFianchettoPattern = 512;
	static constexpr common::Bitset BlackKingFianchettoPatteren = 504403158265495552;
	static constexpr common::Bitset BlackPawnsFianchettoPattern = 1409573906808832;
	static constexpr common::Bitset BlackBishopFianchettoPattern = 562949953421312;
};

} //namespace phase4::engine::ai::score::evaluators

#endif
