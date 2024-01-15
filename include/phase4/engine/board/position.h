#ifndef PHASE4_ENGINE_BOARD_POSITION_H
#define PHASE4_ENGINE_BOARD_POSITION_H

#include <phase4/engine/common/castling.h>

#include <cstdint>

namespace phase4::engine::board {

class Position {
public:
	int64_t Pieces[2][6];
	int64_t Occupancy[2];
	int64_t OccupancySummary;
	int64_t EnPassant;
	common::Castling Castling;
	int ColorToMove;
	int MovesCount;
	int IrreversibleMovesCount;
	int NullMoves;

	bool CastlingDone[2];
	int Material[2];
	//int Position[2][2];

	int PieceTable[64];
	int MaterialAtOpening;

	int64_t Walls = 0;
};

} //namespace phase4::engine::board

#endif
