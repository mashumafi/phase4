#ifndef PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_TABLE_OPERATIONS_H
#define PHASE4_ENGINE_BOARD_PIECE_SQUARE_TABLES_TABLE_OPERATIONS_H

#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board::piece_square_tables {

class TableOperations {
public:
	static constexpr std::array<int32_t, 64> flipVertically(const std::array<int32_t, 64> &array) {
		std::array<int32_t, 64> result{};
		for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
			result[fieldIndex] = array[63 - fieldIndex];
		}
		return result;
	}

	static constexpr std::array<int32_t, 64> flipHorizontally(const std::array<int32_t, 64> &array) {
		std::array<int32_t, 64> result{};
		for (int32_t y = 0; y < 8; y++) {
			for (int32_t x = 0; x < 8; x++) {
				result[x + y * 8] = array[(7 - x) + y * 8];
			}
		}
		return result;
	}
};

} //namespace phase4::engine::board::piece_square_tables
#endif
