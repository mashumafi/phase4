#ifndef PHASE4_ENGINE_AI_SCORE_PIECE_SQUARE_TABLES_TABLE_OPERATIONS_H
#define PHASE4_ENGINE_AI_SCORE_PIECE_SQUARE_TABLES_TABLE_OPERATIONS_H

#include <array>
#include <cstdint>

namespace phase4::engine::ai::score::piece_square_tables {

class TableOperations {
public:
	static constexpr std::array<int32_t, 64> flipVertically(const std::array<int32_t, 64> &array) {
		std::array<int32_t, 64> result{};
		for (int fieldIndex = 0; fieldIndex < 64; fieldIndex++) {
			result[fieldIndex] = array[63 - fieldIndex];
		}
		return result;
	}

	static constexpr std::array<int32_t, 64> flipHorizontally(const std::array<int32_t, 64> &array) {
		std::array<int32_t, 64> result{};
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				result[x + y * 8] = array[(7 - x) + y * 8];
			}
		}
		return result;
	}
};

} //namespace phase4::engine::ai::score::piece_square_tables
#endif
