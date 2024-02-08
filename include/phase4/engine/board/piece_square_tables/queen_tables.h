#ifndef PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_QUEEN_TABLES_H
#define PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_QUEEN_TABLES_H

#include <phase4/engine/board/piece_square_tables/table_operations.h>

#include <phase4/engine/common/game_phase.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board::piece_square_tables {

class QueenTables {
public:
	static const int32_t O0 = -10; // Center
	static const int32_t O1 = -15; // Center
	static const int32_t O2 = -5; // Center
	static const int32_t O3 = 15; // Edge
	static const int32_t O4 = 5; // Edge
	static const int32_t O5 = -10; // Corners

	static const int32_t E0 = -5; // Center
	static const int32_t E1 = 10; // Center
	static const int32_t E2 = 10; // Center
	static const int32_t E3 = -15; // Edge
	static const int32_t E4 = -10; // Corners

	static constexpr std::array<std::array<std::array<int32_t, 64>, 2>, 2> build() {
		const std::array<int32_t, 64> opening = {
			O5, O4, O4, O3, O3, O4, O4, O5,
			O4, O2, O2, O2, O2, O2, O2, O4,
			O4, O2, O1, O1, O1, O1, O2, O4,
			O3, O2, O1, O0, O0, O1, O2, O3,
			O3, O2, O1, O0, O0, O1, O2, O3,
			O4, O2, O1, O1, O1, O1, O2, O4,
			O4, O2, O2, O2, O2, O2, O2, O4,
			O5, O4, O4, O3, O3, O4, O4, O5
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
