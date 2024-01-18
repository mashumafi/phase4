#ifndef PHASE4_ENGINE_MOVES_MAGIC_MAGIC_SHIFTS_H
#define PHASE4_ENGINE_MOVES_MAGIC_MAGIC_SHIFTS_H

#include <phase4/engine/common/random.h>

#include <array>

namespace phase4::engine::moves::magic {

class MagicShifts {
public:
	using Array = std::array<int32_t, 64>;

	static constexpr size_t MAX_BISHOP_SHIFT = 9;
	static constexpr size_t MAX_ROOK_SHIFT = 12;

	static constexpr Array ROOK_SHIFTS{
		12, 11, 11, 11, 11, 11, 11, 12,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		12, 11, 11, 11, 11, 11, 11, 12
	};

	static constexpr Array BISHOP_SHIFTS{
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
