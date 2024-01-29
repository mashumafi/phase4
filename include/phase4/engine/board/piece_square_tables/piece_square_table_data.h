#ifndef PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_PIECE_SQUARE_TABLE_DATA_H
#define PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_PIECE_SQUARE_TABLE_DATA_H

#include <phase4/engine/board/piece_square_tables/bishop_tables.h>
#include <phase4/engine/board/piece_square_tables/king_tables.h>
#include <phase4/engine/board/piece_square_tables/knight_tables.h>
#include <phase4/engine/board/piece_square_tables/pawn_tables.h>
#include <phase4/engine/board/piece_square_tables/queen_tables.h>
#include <phase4/engine/board/piece_square_tables/rook_tables.h>

#include <array>

namespace phase4::engine::board::piece_square_tables {

class PieceSquareTablesData {
public:
	static const std::array<std::array<std::array<std::array<int32_t, 64>, 2>, 2>, 6> VALUES;
};

inline constexpr std::array<std::array<std::array<std::array<int32_t, 64>, 2>, 2>, 6> PieceSquareTablesData::VALUES = {
	PawnTables::build(),
	KnightTables::build(),
	BishopTables::build(),
	QueenTables::build(),
	RookTables::build(),
	KingTables::build()
};

} //namespace phase4::engine::board::piece_square_tables
#endif
