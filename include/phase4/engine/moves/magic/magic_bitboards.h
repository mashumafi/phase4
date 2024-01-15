#ifndef PHASE4_ENGINE_MOVES_MAGIC_MAGIC_BITBOARDS_H
#define PHASE4_ENGINE_MOVES_MAGIC_MAGIC_BITBOARDS_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/random.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/moves/magic/attacks_generator.h>
#include <phase4/engine/moves/magic/magic_container.h>
#include <phase4/engine/moves/magic/magic_keys.h>
#include <phase4/engine/moves/magic/magic_shifts.h>
#include <phase4/engine/moves/magic/permutation_generator.h>
#include <phase4/engine/moves/patterns/diagonal_pattern_generator.h>
#include <phase4/engine/moves/patterns/file_pattern_generator.h>
#include <phase4/engine/moves/patterns/rank_pattern_generator.h>

namespace phase4::engine::moves::magic {

class MagicBitboards {
public:
	static constexpr common::Bitset getRookMoves(common::Bitset board, common::Square square);
	static constexpr common::Bitset getBishopMoves(common::Bitset board, common::Square square);
};

constexpr common::Bitset MagicBitboards::getRookMoves(common::Bitset board, common::Square square) {
	return common::Bitset(0);
}
constexpr common::Bitset MagicBitboards::getBishopMoves(common::Bitset board, common::Square square) {
	return common::Bitset(0);
}

} //namespace phase4::engine::moves::magic

#endif
