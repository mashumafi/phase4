#ifndef PHASE4_ENGINE_MOVES_MAGIC_PERMUTATIONS_GENERATOR_H
#define PHASE4_ENGINE_MOVES_MAGIC_PERMUTATIONS_GENERATOR_H

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/random.h>
#include <phase4/engine/common/square.h>

namespace phase4::engine::moves::magic {

class PermutationsGenerator {
public:
	static inline constexpr common::Bitboard getPermutation(common::Bitboard mask, uint64_t permutationIndex) noexcept {
		common::Bitboard permutation = 0;
		while (mask != 0) {
			if ((permutationIndex & 1) != 0) {
				permutation |= mask.getLsb();
			}

			mask = mask.popLsb();
			permutationIndex >>= 1;
		}

		return permutation;
	}
};

} //namespace phase4::engine::moves::magic

#endif
