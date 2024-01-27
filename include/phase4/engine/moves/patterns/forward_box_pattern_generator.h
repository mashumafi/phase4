#ifndef PHASE4_ENGINE_MOVES_PATTERNS_FORWARD_BOX_PATTERN_GENERATOR_H
#define PHASE4_ENGINE_MOVES_PATTERNS_FORWARD_BOX_PATTERN_GENERATOR_H

#include <phase4/engine/moves/patterns/box_pattern_generator.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::patterns {

class ForwardBoxPatternGenerator {
public:
	using Array = std::array<std::array<common::Bitset, 64>, 2>;

	static constexpr common::Bitset getPattern(common::PieceColor color, common::Square fieldIndex) {
		return PATTERNS[color.get_raw_value()][fieldIndex];
	}

private:
	static constexpr common::Bitset getPatternForField(common::PieceColor color, common::Square fieldIndex) {
		int16_t offset = color == common::PieceColor::WHITE ? 8 : -8;
		common::Bitset mask = BoxPatternGenerator::getPattern(fieldIndex);
		common::Square fieldIndexWithOffset(fieldIndex + offset);

		if (fieldIndexWithOffset.isValid()) {
			mask |= BoxPatternGenerator::getPattern(fieldIndexWithOffset);
			mask &= ~(fieldIndex.asBitboard());
		}

		return mask;
	}

	static constexpr Array generate() {
		Array patterns;
		for (common::PieceColor color = common::PieceColor::WHITE; color != common::PieceColor::INVALID; ++color) {
			for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
				patterns[color.get_raw_value()][fieldIndex] = getPatternForField(color, fieldIndex);
			}
		}
		return patterns;
	}

	static const Array PATTERNS;
};

constexpr ForwardBoxPatternGenerator::Array ForwardBoxPatternGenerator::PATTERNS = generate();

} //namespace phase4::engine::moves::patterns
#endif
