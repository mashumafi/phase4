#ifndef PHASE4_ENGINE_MOVES_PATTERNS_DIAGONAL_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_DIAGONAL_PATTERN_GENERATOR_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class DiagonalPatternGenerator {
public:
	static common::Bitset getPattern(int fieldIndex) {
		return PATTERNS[fieldIndex];
	}

private:
	static const std::array<common::Bitset, 64> PATTERNS;

	static constexpr uint64_t getPatternForField(common::Square square, common::FieldIndex shift) {
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

	static constexpr std::array<common::Bitset, 64> generatePatterns() {
		std::array<common::Bitset, 64> patterns{};
		for (int i = 0; i < 64; ++i) {
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
};

inline constexpr std::array<common::Bitset, 64> DiagonalPatternGenerator::PATTERNS = DiagonalPatternGenerator::generatePatterns();

} //namespace phase4::engine::moves::patterns

#endif
