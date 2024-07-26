#ifndef PHASE4_ENGINE_MOVES_PATTERNS_FILE_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_FILE_PATTERN_GENERATOR_H

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/position_constants.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class FilePatternGenerator {
public:
	using Array = std::array<common::Bitboard, 8>;

	static constexpr common::Bitboard getPatternForField(common::Square square) noexcept;

	static constexpr common::Bitboard getPatternForFile(uint16_t rank) noexcept;

private:
	static constexpr Array generatePatterns() noexcept;

	static constexpr common::Bitboard generatePatternForField(size_t file) noexcept;

	static const Array PATTERNS;
};

constexpr common::Bitboard FilePatternGenerator::generatePatternForField(size_t file) noexcept {
	return common::PositionConstants::FILE_H << file;
}

constexpr FilePatternGenerator::Array FilePatternGenerator::generatePatterns() noexcept {
	Array patterns{};
	for (size_t i = 0; i < patterns.size(); ++i) {
		patterns[i] = generatePatternForField(i);
	}
	return patterns;
}

inline constexpr FilePatternGenerator::Array FilePatternGenerator::PATTERNS = generatePatterns();

inline constexpr common::Bitboard FilePatternGenerator::getPatternForField(common::Square square) noexcept {
	return PATTERNS[square % 8] & ~(square.asBitboard().get_raw_value());
}

constexpr common::Bitboard FilePatternGenerator::getPatternForFile(uint16_t file) noexcept {
	assert(file < 8);
	assert(file < 64); // TODO: remove this or mod below
	return PATTERNS[file % 8];
}

} //namespace phase4::engine::moves::patterns

#endif
