#ifndef PHASE4_ENGINE_MOVES_PATTERNS_DIAGONAL_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_DIAGONAL_PATTERN_GENERATOR_H

#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class DiagonalPatternGenerator {
public:
	static uint64_t getPattern(int fieldIndex) {
		return PATTERNS[fieldIndex];
	}

private:
	static const std::array<uint64_t, 64> PATTERNS;

	static constexpr uint64_t GetPatternForField(int fieldIndex, common::FieldIndex shift) {
		uint64_t attacks = 0;
		common::FieldIndex currentPosition = common::Square(fieldIndex).asFieldIndex();

		currentPosition = currentPosition + shift;
		while (currentPosition.isValid()) {
			uint64_t positionBitIndex = common::Square(currentPosition);

			attacks |= 1ull << positionBitIndex;
			currentPosition = currentPosition + shift;
		}

		return attacks;
	}

	static constexpr std::array<uint64_t, 64> generatePatterns() {
		std::array<uint64_t, 64> patterns{};
		for (int i = 0; i < 64; ++i) {
			common::FieldIndex rightTopShift(-1, 1);
			common::FieldIndex leftTopShift(1, 1);
			common::FieldIndex rightBottomShift(-1, -1);
			common::FieldIndex leftBottomShift(1, -1);

			uint64_t rightTopPattern = GetPatternForField(i, rightTopShift);
			uint64_t leftTopPattern = GetPatternForField(i, leftTopShift);
			uint64_t rightBottomPattern = GetPatternForField(i, rightBottomShift);
			uint64_t leftBottomPattern = GetPatternForField(i, leftBottomShift);

			patterns[i] = rightTopPattern | leftTopPattern | rightBottomPattern | leftBottomPattern;
		}
		return patterns;
	}
};

constexpr std::array<uint64_t, 64> DiagonalPatternGenerator::PATTERNS = DiagonalPatternGenerator::generatePatterns();

} //namespace phase4::engine::moves::patterns

#endif
