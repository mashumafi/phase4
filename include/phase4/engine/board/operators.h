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
};

} //namespace phase4::engine::board
#endif
