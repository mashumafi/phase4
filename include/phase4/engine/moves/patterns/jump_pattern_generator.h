#ifndef PHASE4_ENGINE_MOVES_PATTERNS_JUMP_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_JUMP_PATTERN_GENERATOR_H

#include <phase4/engine/board/board_constants.h>
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class JumpPatternGenerator {
public:
	using Array = std::array<common::Bitset, 64>;

	static constexpr common::Bitset getPattern(common::Square square);

private:
	static constexpr common::Bitset generatePatternForField(common::Square square);

	static constexpr Array generatePatterns();

	static const Array PATTERNS;
};

constexpr common::Bitset JumpPatternGenerator::generatePatternForField(common::Square square) {
	uint64_t field = 1ull << square;
	return ((field & ~board::BoardConstants::FILE_A) << 17) |
			((field & ~board::BoardConstants::FILE_H) << 15) |
			((field & ~board::BoardConstants::FILE_A & ~board::BoardConstants::FILE_B) << 10) |
			((field & ~board::BoardConstants::FILE_G & ~board::BoardConstants::FILE_H) << 6) |
			((field & ~board::BoardConstants::FILE_A & ~board::BoardConstants::FILE_B) >> 6) |
			((field & ~board::BoardConstants::FILE_G & ~board::BoardConstants::FILE_H) >> 10) |
			((field & ~board::BoardConstants::FILE_A) >> 15) |
			((field & ~board::BoardConstants::FILE_H) >> 17);
}

constexpr JumpPatternGenerator::Array JumpPatternGenerator::generatePatterns() {
	Array patterns{};
	for (size_t i = 0; i < patterns.size(); ++i) {
		patterns[i] = generatePatternForField(common::Square(i));
	}
	return patterns;
}

inline constexpr JumpPatternGenerator::Array JumpPatternGenerator::PATTERNS = generatePatterns();

constexpr common::Bitset JumpPatternGenerator::getPattern(common::Square square) {
	return PATTERNS[square];
}

} //namespace phase4::engine::moves::patterns

#endif
