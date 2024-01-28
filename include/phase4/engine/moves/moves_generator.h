#ifndef PHASE4_ENGINE_MOVES_MOVES_GENERATOR_H
#define PHASE4_ENGINE_MOVES_MOVES_GENERATOR_H

#include <phase4/engine/moves/magic/magic_bitboards.h>

#include <phase4/engine/moves/patterns/box_pattern_generator.h>
#include <phase4/engine/moves/patterns/jump_pattern_generator.h>

namespace phase4::engine::moves {

class MovesGenerator {
public:
	static common::Bitset getKnightMoves(common::Square square) {
		return patterns::JumpPatternGenerator::getPattern(square);
	}

	static common::Bitset getBishopMoves(common::Bitset board, common::Square square) {
		return magic::MagicBitboards::getBishopMoves(board, square);
	}

	static common::Bitset getRookMoves(common::Bitset board, common::Square square) {
		return magic::MagicBitboards::getRookMoves(board, square);
	}

	static common::Bitset getQueenMoves(common::Bitset board, common::Square square) {
		return getBishopMoves(board, square) | getRookMoves(board, square);
	}

	static common::Bitset getKingMoves(common::Square square) {
		return patterns::BoxPatternGenerator::getPattern(square);
	}
};

} //namespace phase4::engine::moves

#endif
