#ifndef PHASE4_ENGINE_MOVES_PATTERNS_JUMP_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_JUMP_PATTERN_GENERATOR_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/square.h>

#include <cstdint>

namespace phase4::engine::moves::patterns {

class JumpPatternGenerator {
public:
	static constexpr common::Bitset getPattern(common::Square square);
};

constexpr common::Bitset JumpPatternGenerator::getPattern(common::Square square) {
	return common::Bitset(0);
}

} //namespace phase4::engine::moves::patterns

#endif
