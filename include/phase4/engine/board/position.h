#ifndef PHASE4_ENGINE_BOARD_POSITION_H
#define PHASE4_ENGINE_BOARD_POSITION_H

#include <cstdint>

class Position {
public:
	//int64_t[][] Pieces;
	//int64_t[] Occupancy;
	int64_t OccupancySummary;
	int64_t EnPassant;
	//Castling Castling;
	int ColorToMove;
	int MovesCount;
	int IrreversibleMovesCount;
	int NullMoves;

	//bool[] CastlingDone;
	//int[] Material;
	//int[][] Position;

	//int[] PieceTable;
	int MaterialAtOpening;

	int64_t Walls = 0;
};
#endif
