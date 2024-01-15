#ifndef PHASE4_ENGINE_MOVES_MAGIC_PERMUTATION_GENERATOR_H
#define PHASE4_ENGINE_MOVES_MAGIC_PERMUTATION_GENERATOR_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/random.h>

namespace phase4::engine::moves::magic {

class PermutationGenerator {
public:
	static constexpr uint64_t getPermutation(common::Bitset mask, int permutationIndex) {
		uint64_t permutation = 0;
		while (mask != 0) {
			const common::Bitset lsb = mask.getLsb();
			mask = mask.popLsb();

			if ((permutationIndex & 1) != 0) {
				const uint8_t lsbIndex = lsb.bitScan();
				permutation |= 1ul << lsbIndex;
			}

			permutationIndex >>= 1;
		}

		return permutation;
	}
};

} //namespace phase4::engine::moves::magic

#endif
