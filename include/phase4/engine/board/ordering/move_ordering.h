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
	static inline int16_t seeEvaluate(const Position &position, moves::Move move) {
		const common::PieceColor enemyColor = position.colorToMove().invert();

		const common::PieceType attackingPiece = position.pieceTable(move.from());
		const common::PieceType capturedPiece = position.pieceTable(move.to());

		const uint8_t attackers = SeePiece::getAttackingPiecesWithColor(position, position.colorToMove(), move.to());
		// TOOD: Update defenders to include walls/sliding
		const uint8_t defenders = SeePiece::getAttackingPiecesWithColor(position, enemyColor, move.to());
		return StaticExchangeEvaluation::evaluate(attackingPiece, capturedPiece, attackers, defenders);
	}

	static void assignLoudValues(const Position &position, const moves::Moves &moves, moves::MoveValues &moveValues, moves::Move hashOrPvMove) {
		moveValues.resize(moves.size());
		for (size_t moveIndex = 0; moveIndex < moves.size(); ++moveIndex) {
			if (hashOrPvMove == moves[moveIndex]) {
				moveValues[moveIndex] = MoveOrderingConstants::HASH_MOVE;
			} else if (moves[moveIndex].flags().isEnPassant()) {
				moveValues[moveIndex] = MoveOrderingConstants::EN_PASSANT;
			} else if (moves[moveIndex].flags().isPromotion()) {
				moveValues[moveIndex] = static_cast<int16_t>((MoveOrderingConstants::PROMOTION + moves[moveIndex].flags().get_raw_value())); // TODO: cast?
			} else if (moves[moveIndex].flags().isCapture()) {
				moveValues[moveIndex] = MoveOrderingConstants::CAPTURE + seeEvaluate(position, moves[moveIndex]);
			} else if (moves[moveIndex].flags().isCastling()) {
				moveValues[moveIndex] = MoveOrderingConstants::CASTLING;
			} else {
				moveValues[moveIndex] = MoveOrderingConstants::PAWN_NEAR_PROMOTION;
			}
		}
	}

	static void assignQuietValues(const Session &session, const moves::Moves &moves, moves::MoveValues &moveValues, size_t startIndex, int32_t ply) {
		moveValues.resize(moves.size());
		const Position &position = session.position();
		for (size_t moveIndex = startIndex; moveIndex < moves.size(); ++moveIndex) {
			if (session.m_killerHeuristic.killerMoveExists(moves[moveIndex], position.colorToMove(), ply)) {
				moveValues[moveIndex] = MoveOrderingConstants::KILLER_MOVE;
			} else {
				moveValues[moveIndex] = session.m_historyHeuristic.getMoveValue(position.colorToMove(), position.pieceTable(moves[moveIndex].from()), moves[moveIndex].to(), MoveOrderingConstants::HISTORY_HEURISTIC_MAX_SCORE);
			}
		}
	}

	static void assignQValues(const Position &position, const moves::Moves &moves, moves::MoveValues &moveValues) {
		moveValues.resize(moves.size());
		for (size_t moveIndex = 0; moveIndex < moves.size(); ++moveIndex) {
			if (moves[moveIndex].flags().isEnPassant()) {
				moveValues[moveIndex] = MoveOrderingConstants::EN_PASSANT;
			} else {
				moveValues[moveIndex] = seeEvaluate(position, moves[moveIndex]);
			}
		}
	}

	static void sortNextBestMove(moves::Moves &moves, moves::MoveValues &moveValues, size_t currentIndex) {
		assert(moves.size() == moveValues.size());

		if (moves.size() <= 1) {
			return;
		}

		int32_t max = moveValues[currentIndex];
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
