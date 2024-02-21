#ifndef PHASE4_ENGINE_MOVES_PATTERNS_OUTER_FILES_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_OUTER_FILES_PATTERN_GENERATOR_H

#include <phase4/engine/common/position_constants.h>

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class OuterFilesPatternGenerator {
public:
	using Patterns = std::array<common::Bitboard, 8>;

	static constexpr common::Bitboard getPatternForFile(size_t file);

private:
	static constexpr common::Bitboard generatePatternForField(size_t fieldIndex) {
		common::Bitboard result = 0ul;
		if (fieldIndex + 1 < 8) {
			result |= common::PositionConstants::FILE_H << (fieldIndex + 1);
		}

		if (fieldIndex - 1 < 8) {
			result |= common::PositionConstants::FILE_H << (fieldIndex - 1);
		}

		return result;
	}

	static constexpr Patterns generate() {
		Patterns patterns;
		for (size_t i = 0; i < patterns.size(); i++) {
			patterns[i] = generatePatternForField(i);
		}
		return patterns;
	}

	static const Patterns PATTERNS;
};

inline constexpr OuterFilesPatternGenerator::Patterns OuterFilesPatternGenerator::PATTERNS = generate();

constexpr common::Bitboard OuterFilesPatternGenerator::getPatternForFile(size_t file) {
	return PATTERNS[file];
}

} //namespace phase4::engine::moves::patterns

#endif
