#ifndef PHASE4_ENGINE_AI_SCORE_PIECE_SQUARE_TABLES_KING_TABLES_H
#define PHASE4_ENGINE_AI_SCORE_PIECE_SQUARE_TABLES_KING_TABLES_H

#include <phase4/engine/ai/score/piece_square_tables/table_operations.h>

#include <phase4/engine/common/game_phase.h>

#include <array>
#include <cstdint>

namespace phase4::engine::ai::score::piece_square_tables {

class KingTables {
public:
	static constexpr int32_t O0 = -50; // 8th, 7th, 6th rank
	static constexpr int32_t O1 = -30; // 5th, 4th rank
	static constexpr int32_t O2 = -30; // 3rd rank
	static constexpr int32_t O3 = -15; // 2nd, 1st rank
	static constexpr int32_t O4 = 0; // King original fields
	static constexpr int32_t O5 = 20; // Fields after castling

	static constexpr int32_t E0 = 20; // Center
	static constexpr int32_t E1 = 15; // Center
	static constexpr int32_t E2 = -5; // Center
	static constexpr int32_t E3 = -20; // Edge

	static constexpr std::array<std::array<std::array<int32_t, 64>, 2>, 2> build() {
		std::array<std::array<int32_t, 64>, 2> pattern = {
			// Opening
			{
					{ O0, O0, O0, O0, O0, O0, O0, O0,
							O0, O0, O0, O0, O0, O0, O0, O0,
							O0, O0, O0, O0, O0, O0, O0, O0,
							O1, O1, O1, O1, O1, O1, O1, O1,
							O1, O1, O1, O1, O1, O1, O1, O1,
							O2, O2, O2, O2, O2, O2, O2, O2,
							O3, O3, O3, O3, O3, O3, O3, O3,
							O3, O3, O5, O4, O4, O3, O5, O3 },
					// Ending
					{
							E3, E3, E3, E3, E3, E3, E3, E3,
							E3, E2, E2, E2, E2, E2, E2, E3,
							E3, E2, E1, E1, E1, E1, E2, E3,
							E3, E2, E1, E0, E0, E1, E2, E3,
							E3, E2, E1, E0, E0, E1, E2, E3,
							E3, E2, E1, E1, E1, E1, E2, E3,
							E3, E2, E2, E2, E2, E2, E2, E3,
							E3, E3, E3, E3, E3, E3, E3, E3 } }
		};

		return std::array<std::array<std::array<int32_t, 64>, 2>, 2>{
			// White
			std::array<std::array<int32_t, 64>, 2>{
					TableOperations::flipVertically(pattern[common::GamePhase::OPENING]),
					TableOperations::flipVertically(pattern[common::GamePhase::ENDING]) },

			// Black
			std::array<std::array<int32_t, 64>, 2>{
					TableOperations::flipHorizontally(pattern[common::GamePhase::OPENING]),
					TableOperations::flipHorizontally(pattern[common::GamePhase::ENDING]) }
		};
	}
};

} //namespace phase4::engine::ai::score::piece_square_tables
#endif
