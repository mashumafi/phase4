#ifndef PHASE4_ENGINE_MOVES_MAGIC_ATTACKS_GENERATOR_H
#define PHASE4_ENGINE_MOVES_MAGIC_ATTACKS_GENERATOR_H

#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/square.h>

#include <cstdint>

namespace phase4::engine::moves::magic {

class AttacksGenerator {
public:
	static uint64_t getFileRankAttacks(common::Bitset board, int fieldIndex) {
		return getAttacksForDirection(board, fieldIndex, common::FieldIndex(0, 1)) |
				getAttacksForDirection(board, fieldIndex, common::FieldIndex(0, -1)) |
				getAttacksForDirection(board, fieldIndex, common::FieldIndex(1, 0)) |
				getAttacksForDirection(board, fieldIndex, common::FieldIndex(-1, 0));
	}

	static uint64_t getDiagonalAttacks(common::Bitset board, int fieldIndex) {
		return getAttacksForDirection(board, fieldIndex, common::FieldIndex(1, 1)) |
				getAttacksForDirection(board, fieldIndex, common::FieldIndex(1, -1)) |
				getAttacksForDirection(board, fieldIndex, common::FieldIndex(-1, 1)) |
				getAttacksForDirection(board, fieldIndex, common::FieldIndex(-1, -1));
	}

private:
	static uint64_t getAttacksForDirection(common::Bitset board, int fieldIndex, const common::FieldIndex &shift) {
		common::FieldIndex current = common::Square(fieldIndex).asFieldIndex() + shift;
		uint64_t attacks = 0;

		while (current.isValid()) {
			attacks |= 1ull << common::Square(current);
			if ((common::Square(current).asBitboard() & board) != 0) {
				break;
			}

			current = current + shift;
		}

		return attacks;
	}
};

} //namespace phase4::engine::moves::magic
#endif
