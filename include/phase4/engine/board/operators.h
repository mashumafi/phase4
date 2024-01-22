#ifndef PHASE4_ENGINE_BOARD_OPERATORS_H
#define PHASE4_ENGINE_BOARD_OPERATORS_H

#include <phase4/engine/board/operators/bishop_operator.h>
#include <phase4/engine/board/operators/king_operator.h>
#include <phase4/engine/board/operators/knight_operator.h>
#include <phase4/engine/board/operators/pawn_operator.h>
#include <phase4/engine/board/operators/queen_operator.h>
#include <phase4/engine/board/operators/rook_operator.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/moves/move.h>

namespace phase4::engine::board {

class Operators {
public:
	static void getLoudMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		operators::PawnOperator::getLoudMoves(position, moves, evasionMask);
		operators::KnightOperator::getLoudMoves(position, moves, evasionMask);
		operators::BishopOperator::getLoudMoves(position, moves, evasionMask);
		operators::RookOperator::getLoudMoves(position, moves, evasionMask);
		operators::QueenOperator::getLoudMoves(position, moves, evasionMask);
		operators::KingOperator::getLoudMoves(position, moves);
	}

	static void getQuietMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		operators::PawnOperator::getQuietMoves(position, moves, evasionMask);
		operators::KnightOperator::getQuietMoves(position, moves, evasionMask);
		operators::BishopOperator::getQuietMoves(position, moves, evasionMask);
		operators::RookOperator::getQuietMoves(position, moves, evasionMask);
		operators::QueenOperator::getQuietMoves(position, moves, evasionMask);
		operators::KingOperator::getQuietMoves(position, moves);
	}

	static void getAllMoves(const Position &position, moves::Moves &moves) {
		getLoudMoves(position, moves, common::Bitset::MAX);
		getQuietMoves(position, moves, common::Bitset::MAX);
	}

	void getAvailableCaptureMoves(const Position &position, moves::Moves& moves) {
		operators::PawnOperator::getAvailableCaptureMoves(position, moves);
		operators::KnightOperator::getAvailableCaptureMoves(position, moves);
		operators::BishopOperator::getAvailableCaptureMoves(position, moves);
		operators::RookOperator::getAvailableCaptureMoves(position, moves);
		operators::QueenOperator::getAvailableCaptureMoves(position, moves);
		operators::KingOperator::getAvailableCaptureMoves(position, moves);
	}

	static bool isMoveLegal(const Position &position, moves::Move move) {
		// Check if that color has a piece at the `move.from()` square
		if (((move.from().asBitboard()) & position.Occupancy[position.ColorToMove.get_raw_value()]) == 0) {
			return false;
		}

		common::PieceType fromPiece = position.PieceTable[move.from().get_raw_value()];
		switch (fromPiece.get_raw_value()) {
			case common::PieceType::PAWN.get_raw_value():
				return operators::PawnOperator::isMoveLegal(position, move);
			case common::PieceType::KNIGHT.get_raw_value():
				return operators::KnightOperator::isMoveLegal(position, move);
			case common::PieceType::BISHOP.get_raw_value():
				return operators::BishopOperator::isMoveLegal(position, move);
			case common::PieceType::ROOK.get_raw_value():
				return operators::RookOperator::isMoveLegal(position, move);
			case common::PieceType::QUEEN.get_raw_value():
				return operators::QueenOperator::isMoveLegal(position, move);
			case common::PieceType::KING.get_raw_value():
				return operators::KingOperator::isMoveLegal(position, move);
		}

		return false;
	}
};

} //namespace phase4::engine::board
#endif
