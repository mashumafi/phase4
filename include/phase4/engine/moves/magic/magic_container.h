#ifndef PHASE4_ENGINE_MOVES_MAGIC_MAGIC_CONTAINER_H
#define PHASE4_ENGINE_MOVES_MAGIC_MAGIC_CONTAINER_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/moves/magic/magic_shifts.h>

#include <array>
#include <cstdint>

namespace phase4::engine::moves::magic {

class MagicContainer {
public:
	common::Bitset Mask;
	common::Bitset MagicNumber;
	std::array<common::Bitset, 1ull << MagicShifts::MAX_SHIFT> Attacks;
	int32_t Shift;
};

} //namespace phase4::engine::moves::magic

#endif
