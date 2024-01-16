#ifndef PHASE4_ENGINE_MOVES_PATTERNS_FILE_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_FILE_PATTERN_GENERATOR_H

#include <phase4/engine/board/board_constants.h> // TODO: Remove circular dependency
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class FilePatternGenerator {
public:
	using Array = std::array<common::Bitset, 8>;

	static constexpr common::Bitset getPatternForField(common::Square square);

	static constexpr common::Bitset getPatternForFile(uint8_t rank);

private:
	static constexpr Array generatePatterns();

	static constexpr common::Bitset generatePatternForField(uint8_t file);

	static const Array PATTERNS;
};

constexpr common::Bitset FilePatternGenerator::generatePatternForField(uint8_t file) {
	return board::BoardConstants::FILE_H << file;
}

constexpr FilePatternGenerator::Array FilePatternGenerator::generatePatterns() {
	Array patterns{};
	for (size_t i = 0; i < patterns.size(); ++i) {
		patterns[i] = generatePatternForField(i);
	}
	return patterns;
}

inline constexpr FilePatternGenerator::Array FilePatternGenerator::PATTERNS = generatePatterns();

constexpr common::Bitset FilePatternGenerator::getPatternForField(common::Square square) {
	return PATTERNS[square % 8] & ~(1ull << square);
}

constexpr common::Bitset FilePatternGenerator::getPatternForFile(uint8_t rank) {
	return PATTERNS[rank % 8];
}

} //namespace phase4::engine::moves::patterns

#endif