#ifndef PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_KNIGHT_TABLES_H
#define PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_KNIGHT_TABLES_H

#include <phase4/engine/board/piece_square_tables/table_operations.h>

#include <phase4/engine/common/game_phase.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board::piece_square_tables {

class KnightTables {
public:
	static const int32_t O0 = 15; // Center
	static const int32_t O1 = 15; // Center
	static const int32_t O2 = 15; // Center
	static const int32_t O3 = 0; // Original fields
	static const int32_t O4 = -10; // Edge
	static const int32_t O5 = -15; // Corners

	static const int32_t E0 = 10; // Center
	static const int32_t E1 = 5; // Center
	static const int32_t E2 = 0; // Center
	static const int32_t E3 = -10; // Edge
	static const int32_t E4 = -15; // Corners

	static constexpr std::array<std::array<std::array<int32_t, 64>, 2>, 2> build() {
		const std::array<int32_t, 64> opening = {
			O5, O4, O4, O4, O4, O4, O4, O5,
			O4, O2, O2, O2, O2, O2, O2, O4,
			O4, O2, O1, O1, O1, O1, O2, O4,
			O4, O1, O1, O0, O0, O1, O1, O4,
			O4, O2, O0, O0, O0, O0, O2, O4,
			O4, O1, O0, O0, O0, O0, O1, O4,
			O4, O1, O2, O2, O2, O2, O1, O4,
			O5, O4, O3, O4, O4, O3, O4, O5
		};

		const std::array<int32_t, 64> ending = {
			E4, E3, E3, E3, E3, E3, E3, E4,
			E3, E2, E2, E2, E2, E2, E2, E3,
			E3, E2, E1, E1, E1, E1, E2, E3,
			E3, E2, E1, E0, E0, E1, E2, E3,
			E3, E2, E1, E0, E0, E1, E2, E3,
			E3, E2, E1, E1, E1, E1, E2, E3,
			E3, E2, E2, E2, E2, E2, E2, E3,
			E4, E3, E3, E3, E3, E3, E3, E4
		};

		return std::array<std::array<std::array<int32_t, 64>, 2>, 2>{
			// White
			std::array<std::array<int32_t, 64>, 2>{
					TableOperations::flipVertically(opening),
					TableOperations::flipVertically(ending) },
			// Black
			std::array<std::array<int32_t, 64>, 2>{
					TableOperations::flipHorizontally(opening),
					TableOperations::flipHorizontally(ending) }
		};
	}
};

} //namespace phase4::engine::board::piece_square_tables
#endif
