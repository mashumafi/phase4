#ifndef PHASE4_ENGINE_BOARD_ORDERING_MOVE_ORDERING_H
#define PHASE4_ENGINE_BOARD_ORDERING_MOVE_ORDERING_H

#include <phase4/engine/board/ordering/move_ordering_constants.h>
#include <phase4/engine/board/ordering/static_exchange_evaluation.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/search_constants.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/move.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>

#include <cstdint>

namespace phase4::engine::board::ordering {

class MoveOrdering {
public:
	static void assignLoudValues(const Position &position, const moves::Moves &moves, moves::MoveValues &moveValues, moves::Move hashOrPvMove) {
		moveValues.resize(moves.size());
		for (size_t moveIndex = 0; moveIndex < moves.size(); ++moveIndex) {
			if (hashOrPvMove == moves[moveIndex]) {
				moveValues[moveIndex] = MoveOrderingConstants::HASH_MOVE;
			} else if (moves[moveIndex].flags().isEnPassant()) {
				moveValues[moveIndex] = MoveOrderingConstants::EN_PASSANT;
			} else if (moves[moveIndex].flags().isPromotion()) {
				moveValues[moveIndex] = (short)(MoveOrderingConstants::PROMOTION + moves[moveIndex].flags().get_raw_value()); // TODO: cast?
			} else if (moves[moveIndex].flags().isCapture()) {
				const common::PieceColor enemyColor = position.m_colorToMove.invert();

				const common::PieceType attackingPiece = position.m_pieceTable[moves[moveIndex].from()];
				const common::PieceType capturedPiece = position.m_pieceTable[moves[moveIndex].to()];

				uint8_t attackers = SeePiece::getAttackingPiecesWithColor(position, position.m_colorToMove, moves[moveIndex].to());
				uint8_t defenders = SeePiece::getAttackingPiecesWithColor(position, enemyColor, moves[moveIndex].to());
				int32_t seeEvaluation = StaticExchangeEvaluation::evaluate(attackingPiece, capturedPiece, attackers, defenders);

				moveValues[moveIndex] = (int32_t)(MoveOrderingConstants::CAPTURE + seeEvaluation);
			} else if (moves[moveIndex].flags().isCastling()) {
				moveValues[moveIndex] = MoveOrderingConstants::CASTLING;
			} else {
				moveValues[moveIndex] = MoveOrderingConstants::PAWN_NEAR_PROMOTION;
			}
		}
	}

	static void assignQuietValues(const Session &session, const moves::Moves &moves, moves::MoveValues &moveValues, int startIndex, int ply) {
		moveValues.resize(moves.size());
		const Position &position = session.m_position;
		for (size_t moveIndex = startIndex; moveIndex < moves.size(); ++moveIndex) {
			if (session.m_killerHeuristic.killerMoveExists(moves[moveIndex], position.m_colorToMove, ply)) {
				moveValues[moveIndex] = MoveOrderingConstants::KILLER_MOVE;
			} else {
				moveValues[moveIndex] = session.m_historyHeuristic.getMoveValue(position.m_colorToMove, position.m_pieceTable[moves[moveIndex].from()], moves[moveIndex].to(), MoveOrderingConstants::HISTORY_HEURISTIC_MAX_SCORE);
			}
		}
	}

	static void assignQValues(const Position &position, const moves::Moves &moves, moves::MoveValues &moveValues) {
		moveValues.resize(moves.size());
		const common::PieceColor enemyColor = position.m_colorToMove.invert();
		for (size_t moveIndex = 0; moveIndex < moves.size(); ++moveIndex) {
			if (moves[moveIndex].flags().isEnPassant()) {
				moveValues[moveIndex] = MoveOrderingConstants::EN_PASSANT;
			} else {
				const common::PieceType attackingPiece = position.m_pieceTable[moves[moveIndex].from()];
				const common::PieceType capturedPiece = position.m_pieceTable[moves[moveIndex].to()];

				const uint8_t attackers = SeePiece::getAttackingPiecesWithColor(position, position.m_colorToMove, moves[moveIndex].to());
				const uint8_t defenders = SeePiece::getAttackingPiecesWithColor(position, enemyColor, moves[moveIndex].to());
				const int32_t seeEvaluation = StaticExchangeEvaluation::evaluate(attackingPiece, capturedPiece, attackers, defenders);

				moveValues[moveIndex] = seeEvaluation;
			}
		}
	}

	static void sortNextBestMove(moves::Moves moves, moves::MoveValues &moveValues, size_t currentIndex) {
		assert(moves.size() == moveValues.size());

		if (moves.size() <= 1) {
			return;
		}

		int16_t max = moveValues[currentIndex];
		size_t maxIndex = currentIndex;

		for (size_t i = currentIndex + 1; i < moves.size(); ++i) {
			if (moveValues[i] > max) {
				max = moveValues[i];
				maxIndex = i;
			}
		}

		std::swap(moves[maxIndex], moves[currentIndex]);
		std::swap(moveValues[maxIndex], moveValues[currentIndex]);
	}
};

} //namespace phase4::engine::board::ordering

#endif
