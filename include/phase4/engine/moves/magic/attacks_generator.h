#ifndef PHASE4_ENGINE_MOVES_MAGIC_ATTACKS_GENERATOR_H
#define PHASE4_ENGINE_MOVES_MAGIC_ATTACKS_GENERATOR_H

#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/square.h>

#include <cstdint>

namespace phase4::engine::moves::magic {

class AttacksGenerator {
public:
	static constexpr common::Bitboard getFileRankAttacks(common::Bitboard board, common::Square square) {
		return getAttacksForDirection(board, square, common::FieldIndex(0, 1)) |
				getAttacksForDirection(board, square, common::FieldIndex(0, -1)) |
				getAttacksForDirection(board, square, common::FieldIndex(1, 0)) |
				getAttacksForDirection(board, square, common::FieldIndex(-1, 0));
	}

	static constexpr common::Bitboard getDiagonalAttacks(common::Bitboard board, common::Square square) {
		return getAttacksForDirection(board, square, common::FieldIndex(1, 1)) |
				getAttacksForDirection(board, square, common::FieldIndex(1, -1)) |
				getAttacksForDirection(board, square, common::FieldIndex(-1, 1)) |
				getAttacksForDirection(board, square, common::FieldIndex(-1, -1));
	}

private:
	static constexpr common::Bitboard getAttacksForDirection(common::Bitboard board, common::Square square, const common::FieldIndex &shift) {
		common::FieldIndex current = square.asFieldIndex() + shift;
		common::Bitboard attacks = 0;

		while (current.isValid()) {
			attacks = attacks | common::Square(current).asBitboard();
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
