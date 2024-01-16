#ifndef PHASE4_ENGINE_MOVES_MAGIC_PERMUTATIONS_GENERATOR_H
#define PHASE4_ENGINE_MOVES_MAGIC_PERMUTATIONS_GENERATOR_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/random.h>
#include <phase4/engine/common/square.h>

namespace phase4::engine::moves::magic {

class PermutationsGenerator {
public:
	static constexpr common::Bitset getPermutation(common::Bitset mask, int permutationIndex) {
		common::Bitset permutation = 0;
		while (mask != 0) {
			const common::Bitset lsb = mask.getLsb();
			mask = mask.popLsb();

			if ((permutationIndex & 1) != 0) {
				const common::Square lsbIndex(lsb.bitScan());
				permutation = permutation | lsbIndex.asBitboard();
			}

			permutationIndex >>= 1;
		}

		return permutation;
	}
};

} //namespace phase4::engine::moves::magic

#endif
