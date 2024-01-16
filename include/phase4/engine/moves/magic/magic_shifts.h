#ifndef PHASE4_ENGINE_MOVES_MAGIC_MAGIC_SHIFTS_H
#define PHASE4_ENGINE_MOVES_MAGIC_MAGIC_SHIFTS_H

#include <phase4/engine/common/random.h>

#include <array>

namespace phase4::engine::moves::magic {

class MagicShifts {
public:
	using Array = std::array<int32_t, 64>;

	static constexpr size_t MAX_SHIFT = 12;

	using Permutations = std::array<std::array<common::Bitset, 64>, 1 << MagicShifts::MAX_SHIFT>;
	using Attacks = std::array<std::array<common::Bitset, 64>, 1 << MagicShifts::MAX_SHIFT>;

	static constexpr Array RookShifts{
		12, 11, 11, 11, 11, 11, 11, 12,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		12, 11, 11, 11, 11, 11, 11, 12
	};

	static constexpr Array BishopShifts{
		6, 5, 5, 5, 5, 5, 5, 6,
		5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 7, 7, 7, 7, 5, 5,
		5, 5, 7, 9, 9, 7, 5, 5,
		5, 5, 7, 9, 9, 7, 5, 5,
		5, 5, 7, 7, 7, 7, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5,
		6, 5, 5, 5, 5, 5, 5, 6
	};
};

} //namespace phase4::engine::moves::magic

#endif
