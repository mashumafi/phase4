#ifndef PHASE4_ENGINE_MOVES_PATTERNS_RANK_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_RANK_PATTERN_GENERATOR_H

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/position_constants.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class RankPatternGenerator {
public:
	using Array = std::array<common::Bitboard, 8>;
	static constexpr common::Bitboard getPatternForField(common::Square square);

	static constexpr common::Bitboard getPatternForRank(uint8_t rank);

private:
	static constexpr Array generatePatterns();

	static constexpr common::Bitboard generatePatternForField(size_t rank);

	static const Array PATTERNS;
};

constexpr common::Bitboard RankPatternGenerator::generatePatternForField(size_t rank) {
	return (common::PositionConstants::RANK_1) << (rank * 8);
}

constexpr RankPatternGenerator::Array RankPatternGenerator::generatePatterns() {
	Array patterns{};
	for (size_t i = 0; i < patterns.size(); i++) {
		patterns[i] = generatePatternForField(i);
	}
	return patterns;
}

inline constexpr RankPatternGenerator::Array RankPatternGenerator::PATTERNS = generatePatterns();

inline constexpr common::Bitboard RankPatternGenerator::getPatternForField(common::Square square) {
	return PATTERNS[square / 8] & ~(square.asBitboard());
}

constexpr common::Bitboard RankPatternGenerator::getPatternForRank(uint8_t rank) {
	return PATTERNS[rank];
}

} //namespace phase4::engine::moves::patterns

#endif
