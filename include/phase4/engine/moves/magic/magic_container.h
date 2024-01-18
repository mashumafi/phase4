#ifndef PHASE4_ENGINE_MOVES_MAGIC_MAGIC_CONTAINER_H
#define PHASE4_ENGINE_MOVES_MAGIC_MAGIC_CONTAINER_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/moves/magic/magic_shifts.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::magic {

template <size_t N>
struct MagicContainer {
	common::Bitset mask;
	common::Bitset magicNumber;
	std::array<common::Bitset, N> attacks;
	int32_t shift;
};

} //namespace phase4::engine::moves::magic

#endif
