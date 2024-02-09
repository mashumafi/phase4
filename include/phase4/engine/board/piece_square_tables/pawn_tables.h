#ifndef PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_PAWN_TABLES_H
#define PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_PAWN_TABLES_H

#include <phase4/engine/board/piece_square_tables/table_operations.h>

#include <phase4/engine/common/game_phase.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board::piece_square_tables {

class PawnTables {
public:
	static constexpr int32_t O0 = 5; // 7th rank
	static constexpr int32_t O1 = 15; // 6th rank
	static constexpr int32_t O2 = 5; // 5th rank
	static constexpr int32_t O3 = -15; // 4th rank with penalty
	static constexpr int32_t O4 = 15; // 4th rank with reward
	static constexpr int32_t O5 = -5; // 3th, 2th rank with penalty
	static constexpr int32_t O6 = 0; // 3th, 2th rank with reward

	static constexpr int32_t E0 = 100; // 7th rank
	static constexpr int32_t E1 = 90; // 6th rank
	static constexpr int32_t E2 = 50; // 5th rank
	static constexpr int32_t E3 = 20; // 4th rank
	static constexpr int32_t E4 = 0; // 3th, 2th rank

	static constexpr std::array<std::array<std::array<int32_t, 64>, 2>, 2> build() {
		constexpr std::array<int32_t, 64> opening = {
			0, 0, 0, 0, 0, 0, 0, 0,
			O0, O0, O0, O0, O0, O0, O0, O0,
			O1, O1, O1, O1, O1, O1, O1, O1,
			O2, O2, O2, O2, O2, O2, O2, O2,
			O3, O3, O3, O4, O4, O3, O3, O3,
			O6, O6, O5, O6, O6, O5, O6, O6,
			O6, O6, O6, O5, O5, O6, O6, O6,
			0, 0, 0, 0, 0, 0, 0, 0
		};

		constexpr std::array<int32_t, 64> ending = {
			0, 0, 0, 0, 0, 0, 0, 0,
			E0, E0, E0, E0, E0, E0, E0, E0,
			E1, E1, E1, E1, E1, E1, E1, E1,
			E2, E2, E2, E2, E2, E2, E2, E2,
			E3, E3, E3, E3, E3, E3, E3, E3,
			E4, E4, E4, E4, E4, E4, E4, E4,
			E4, E4, E4, E4, E4, E4, E4, E4,
			0, 0, 0, 0, 0, 0, 0, 0
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
