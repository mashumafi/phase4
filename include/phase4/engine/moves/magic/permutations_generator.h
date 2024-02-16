#ifndef PHASE4_ENGINE_MOVES_MAGIC_PERMUTATIONS_GENERATOR_H
#define PHASE4_ENGINE_MOVES_MAGIC_PERMUTATIONS_GENERATOR_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/random.h>
#include <phase4/engine/common/square.h>

namespace phase4::engine::moves::magic {

class PermutationsGenerator {
public:
	static constexpr common::Bitset getPermutation(common::Bitset mask, uint64_t permutationIndex) {
		common::Bitset permutation = 0;
		while (mask != 0) {
			if ((permutationIndex & 1) != 0) {
				const common::Square lsbIndex(mask.bitScan());
				permutation = permutation | lsbIndex.asBitboard();
			}

			mask = mask.popLsb();
			permutationIndex >>= 1;
		}

		return permutation;
	}
};

} //namespace phase4::engine::moves::magic

#endif
