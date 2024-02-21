#ifndef PHASE4_ENGINE_MOVES_PATTERNS_CHAIN_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_CHAIN_PATTERN_GENERATOR_H

#include <phase4/engine/moves/patterns/box_pattern_generator.h>
#include <phase4/engine/moves/patterns/diagonal_pattern_generator.h>

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class ChainPatternGenerator {
public:
	using Array = std::array<common::Bitboard, 64>;

	static constexpr common::Bitboard getPattern(common::Square fieldIndex);

private:
	static constexpr common::Bitboard getPatternForField(common::Square fieldIndex) {
		return DiagonalPatternGenerator::getPattern(fieldIndex) & BoxPatternGenerator::getPattern(fieldIndex);
	}

	static constexpr Array populate() {
		Array patterns;
		for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
			patterns[fieldIndex] = getPatternForField(fieldIndex);
		}
		return patterns;
	}

	static const Array PATTERNS;
};

inline constexpr ChainPatternGenerator::Array ChainPatternGenerator::PATTERNS = populate();

constexpr common::Bitboard ChainPatternGenerator::getPattern(common::Square fieldIndex) {
	return PATTERNS[fieldIndex];
}

} //namespace phase4::engine::moves::patterns

#endif
