#ifndef PHASE4_ENGINE_MOVES_PATTERNS_PASSING_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_PASSING_PATTERN_GENERATOR_H

#include <phase4/engine/moves/patterns/box_pattern_generator.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class PassingPatternGenerator {
public:
	using Patterns = std::array<std::array<common::Bitset, 64>, 2>;

	static constexpr common::Bitset getPattern(common::PieceColor color, common::Square fieldIndex);

private:
	static constexpr common::Bitset getPatternForField(common::PieceColor color, common::Square fieldIndex) {
		int8_t offset = color == common::PieceColor::WHITE ? 8 : -8;
		int8_t currentField = fieldIndex + 2 * offset;
		common::Bitset result = 0ul;

		while (currentField >= 0 && currentField < 64) {
			result |= BoxPatternGenerator::getPattern(common::Square(currentField));
			currentField += offset;
		}

		return result;
	}

	static constexpr Patterns generate() {
		Patterns patterns;
		for (common::PieceColor color = common::PieceColor::WHITE; color != common::PieceColor::INVALID; ++color) {
			for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
				patterns[color.get_raw_value()][fieldIndex] = getPatternForField(color, fieldIndex);
			}
		}
		return patterns;
	}

	static const Patterns PATTERNS;
};

constexpr PassingPatternGenerator::Patterns PassingPatternGenerator::PATTERNS = generate();

constexpr common::Bitset PassingPatternGenerator::getPattern(common::PieceColor color, common::Square fieldIndex) {
	return PATTERNS[color.get_raw_value()][fieldIndex];
}

} //namespace phase4::engine::moves::patterns

#endif
