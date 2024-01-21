#ifndef PHASE4_ENGINE_BOARD_OPERATORS_H
#define PHASE4_ENGINE_BOARD_OPERATORS_H

#include <phase4/engine/board/operators/knight_operator.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/moves/move.h>

namespace phase4::engine::board {

class Operators {
public:
	static void GetLoudMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		operators::KnightOperator::GetLoudMoves(position, moves, evasionMask);
	}

	static void GetQuietMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		operators::KnightOperator::GetQuietMoves(position, moves, evasionMask);
	}

	bool IsMoveLegal(const Position &position, moves::Move move) {
		// Check if that color has a piece at the `move.from()` square
		if (((move.from().asBitboard()) & position.Occupancy[position.ColorToMove.get_raw_value()]) == 0) {
			return false;
		}

		common::PieceType fromPiece = position.PieceTable[move.from().get_raw_value()];
		switch (fromPiece.get_raw_value()) {
			case common::PieceType::KNIGHT.get_raw_value():
				return operators::KnightOperator::IsMoveLegal(position, move);
		}

		return false;
	}
};

} //namespace phase4::engine::board
#endif
