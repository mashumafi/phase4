#ifndef PHASE4_ENGINE_MOVES_PATTERNS_BOX_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_BOX_PATTERN_GENERATOR_H

#include <phase4/engine/board/board_constants.h> // TODO: Remove circular dependency
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class BoxPatternGenerator {
public:
	using Array = std::array<common::Bitset, 64>;

	static constexpr common::Bitset getPattern(common::Square square);

private:
	static constexpr common::Bitset getPatternForField(common::Square square);

	static constexpr Array generatePatterns();

	static const Array PATTERNS;
};

constexpr common::Bitset BoxPatternGenerator::getPatternForField(common::Square square) {
	const uint64_t field = 1ull << square;
	return ((field & ~board::BoardConstants::FILE_A) << 1) |
			((field & ~board::BoardConstants::FILE_H) << 7) |
			(field << 8) |
			((field & ~board::BoardConstants::FILE_A) << 9) |
			((field & ~board::BoardConstants::FILE_H) >> 1) |
			((field & ~board::BoardConstants::FILE_A) >> 7) |
			(field >> 8) |
			((field & ~board::BoardConstants::FILE_H) >> 9);
}

constexpr BoxPatternGenerator::Array BoxPatternGenerator::generatePatterns() {
	Array patterns{};
	for (int i = 0; i < 64; ++i) {
		patterns[i] = getPatternForField(common::Square(i));
	}
	return patterns;
}

inline constexpr BoxPatternGenerator::Array BoxPatternGenerator::PATTERNS = BoxPatternGenerator::generatePatterns();

constexpr common::Bitset BoxPatternGenerator::getPattern(common::Square square) {
	return PATTERNS[square];
}

} //namespace phase4::engine::moves::patterns

#endif