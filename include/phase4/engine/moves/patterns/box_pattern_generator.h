#ifndef PHASE4_ENGINE_MOVES_PATTERNS_BOX_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_BOX_PATTERN_GENERATOR_H

#include <phase4/engine/board/board_constants.h>
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class BoxPatternGenerator {
public:
	static constexpr common::Bitset getPattern(common::Square square);

private:
	static const std::array<uint64_t, 64> PATTERNS;

	static constexpr uint64_t getPatternForField(int fieldIndex) {
		uint64_t field = 1ull << fieldIndex;
		return ((field & ~board::BoardConstants::FILE_A) << 1) |
				((field & ~board::BoardConstants::FILE_H) << 7) |
				(field << 8) |
				((field & ~board::BoardConstants::FILE_A) << 9) |
				((field & ~board::BoardConstants::FILE_H) >> 1) |
				((field & ~board::BoardConstants::FILE_A) >> 7) |
				(field >> 8) |
				((field & ~board::BoardConstants::FILE_H) >> 9);
	}

	static constexpr std::array<uint64_t, 64> generatePatterns() {
		std::array<uint64_t, 64> patterns = {};
		for (int i = 0; i < 64; ++i) {
			patterns[i] = getPatternForField(i);
		}
		return patterns;
	}
};

constexpr std::array<uint64_t, 64> BoxPatternGenerator::PATTERNS = BoxPatternGenerator::generatePatterns();

constexpr common::Bitset BoxPatternGenerator::getPattern(common::Square square) {
	return PATTERNS[square];
}

} //namespace phase4::engine::moves::patterns

#endif
