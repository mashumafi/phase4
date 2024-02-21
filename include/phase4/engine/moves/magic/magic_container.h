#ifndef PHASE4_ENGINE_MOVES_MAGIC_MAGIC_CONTAINER_H
#define PHASE4_ENGINE_MOVES_MAGIC_MAGIC_CONTAINER_H

#include <phase4/engine/moves/magic/magic_shifts.h>

#include <phase4/engine/common/bitboard.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::magic {

template <size_t N>
struct MagicContainer {
	common::Bitboard mask;
	common::Bitboard magicNumber;
	std::array<common::Bitboard, N> attacks;
	int32_t shift;
};

} //namespace phase4::engine::moves::magic

#endif
