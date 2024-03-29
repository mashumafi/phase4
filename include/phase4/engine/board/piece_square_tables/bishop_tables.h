#ifndef PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_BISHOP_TABLES_H
#define PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_BISHOP_TABLES_H

#include <phase4/engine/board/piece_square_tables/table_operations.h>

#include <phase4/engine/common/game_phase.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board::piece_square_tables {

class BishopTables {
public:
	static constexpr int32_t O0 = 15; // Center
	static constexpr int32_t O1 = 10; // Center
	static constexpr int32_t O2 = 5; // Center
	static constexpr int32_t O3 = 0; // Original fields
	static constexpr int32_t O4 = -5; // Edge
	static constexpr int32_t O5 = -15; // Corners

	static constexpr int32_t E0 = 15; // Center
	static constexpr int32_t E1 = 15; // Center
	static constexpr int32_t E2 = 15; // Center
	static constexpr int32_t E3 = -10; // Edge
	static constexpr int32_t E4 = -15; // Corners

	static constexpr std::array<std::array<std::array<int32_t, 64>, 2>, 2> build() {
		constexpr std::array opening = {
			O5, O4, O4, O4, O4, O4, O4, O5,
			O4, O2, O2, O2, O2, O2, O2, O4,
			O4, O2, O2, O1, O1, O2, O2, O4,
			O4, O2, O1, O1, O1, O1, O2, O4,
			O4, O1, O1, O0, O0, O1, O1, O4,
			O4, O1, O0, O0, O0, O0, O1, O4,
			O4, O0, O2, O1, O1, O2, O0, O4,
			O5, O4, O3, O4, O4, O3, O4, O5
		};

		constexpr std::array ending = {
			E4, E3, E3, E3, E3, E3, E3, E4,
			E3, E2, E2, E2, E2, E2, E2, E3,
			E3, E2, E1, E1, E1, E1, E2, E3,
			E3, E2, E1, E0, E0, E1, E2, E3,
			E3, E2, E1, E0, E0, E1, E2, E3,
			E3, E2, E1, E1, E1, E1, E2, E3,
			E3, E2, E2, E2, E2, E2, E2, E3,
			E4, E3, E3, E3, E3, E3, E3, E4
		};

		constexpr std::array white = {
			TableOperations::flipVertically(opening),
			TableOperations::flipVertically(ending)
		};

		constexpr std::array black = {
			TableOperations::flipHorizontally(opening),
			TableOperations::flipHorizontally(ending)
		};

		return { white, black };
	}
};

} //namespace phase4::engine::board::piece_square_tables

#endif
