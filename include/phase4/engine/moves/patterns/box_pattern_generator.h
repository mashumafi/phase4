#ifndef PHASE4_ENGINE_MOVES_PATTERNS_BOX_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_BOX_PATTERN_GENERATOR_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/square.h>

#include <cstdint>

namespace phase4::engine::moves::patterns {

class BoxPatternGenerator {
public:
	static constexpr common::Bitset getPattern(common::Square square);
};

constexpr common::Bitset BoxPatternGenerator::getPattern(common::Square square) {
	return common::Bitset(0);
}

} //namespace phase4::engine::moves::patterns

#endif
