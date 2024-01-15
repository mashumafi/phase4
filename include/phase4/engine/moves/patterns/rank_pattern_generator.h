#ifndef PHASE4_ENGINE_MOVES_PATTERNS_RANK_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_RANK_PATTERN_GENERATOR_H

#include <phase4/engine/board/board_constants.h> // TODO: Remove circular dependency
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class RankPatternGenerator {
public:
	static constexpr common::Bitset getPatternForField(common::Square square);

	static constexpr common::Bitset getPatternForRank(uint8_t rank);

private:
	static constexpr std::array<common::Bitset, 8> generatePatterns();

	static constexpr common::Bitset generatePatternForField(uint8_t rank);

	static const std::array<common::Bitset, 8> PATTERNS;
};

constexpr common::Bitset RankPatternGenerator::generatePatternForField(uint8_t rank) {
	return (board::BoardConstants::RANK_1) << (rank * 8);
}

constexpr std::array<common::Bitset, 8> RankPatternGenerator::generatePatterns() {
	std::array<common::Bitset, 8> patterns{};
	for (int i = 0; i < patterns.size(); i++) {
		patterns[i] = generatePatternForField(i);
	}
	return patterns;
}

inline constexpr std::array<common::Bitset, 8> RankPatternGenerator::PATTERNS = generatePatterns();

constexpr common::Bitset RankPatternGenerator::getPatternForField(common::Square square) {
	return PATTERNS[square / 8] & ~(1ull << square);
}

constexpr common::Bitset RankPatternGenerator::getPatternForRank(uint8_t rank) {
	return PATTERNS[rank];
}

} //namespace phase4::engine::moves::patterns

#endif
