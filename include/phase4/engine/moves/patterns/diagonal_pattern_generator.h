#ifndef PHASE4_ENGINE_MOVES_PATTERNS_DIAGONAL_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_DIAGONAL_PATTERN_GENERATOR_H

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class DiagonalPatternGenerator {
public:
	using Array = std::array<common::Bitboard, 64>;

	static constexpr common::Bitboard getPattern(common::Square square);

private:
	static constexpr uint64_t getPatternForField(common::Square square, common::FieldIndex shift);

	static constexpr Array generatePatterns();

	static const Array PATTERNS;
};

constexpr uint64_t DiagonalPatternGenerator::getPatternForField(common::Square square, common::FieldIndex shift) {
	uint64_t attacks = 0;
	common::FieldIndex currentPosition = square.asFieldIndex();

	currentPosition = currentPosition + shift;
	while (currentPosition.isValid()) {
		const uint64_t positionBitIndex = common::Square(currentPosition);

		attacks |= 1ull << positionBitIndex;
		currentPosition = currentPosition + shift;
	}

	return attacks;
}

constexpr DiagonalPatternGenerator::Array DiagonalPatternGenerator::generatePatterns() {
	Array patterns{};
	for (common::Square i = common::Square::BEGIN; i != common::Square::INVALID; ++i) {
		const common::FieldIndex rightTopShift(-1, 1);
		const common::FieldIndex leftTopShift(1, 1);
		const common::FieldIndex rightBottomShift(-1, -1);
		const common::FieldIndex leftBottomShift(1, -1);

		const common::Square square(i);

		const uint64_t rightTopPattern = getPatternForField(square, rightTopShift);
		const uint64_t leftTopPattern = getPatternForField(square, leftTopShift);
		const uint64_t rightBottomPattern = getPatternForField(square, rightBottomShift);
		const uint64_t leftBottomPattern = getPatternForField(square, leftBottomShift);

		patterns[i] = rightTopPattern | leftTopPattern | rightBottomPattern | leftBottomPattern;
	}
	return patterns;
}

inline constexpr DiagonalPatternGenerator::Array DiagonalPatternGenerator::PATTERNS = DiagonalPatternGenerator::generatePatterns();

inline constexpr common::Bitboard DiagonalPatternGenerator::getPattern(common::Square square) {
	return PATTERNS[square];
}

} //namespace phase4::engine::moves::patterns

#endif
