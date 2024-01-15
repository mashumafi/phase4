#ifndef PHASE4_ENGINE_MOVES_MAGIC_MAGIC_CONTAINER_H
#define PHASE4_ENGINE_MOVES_MAGIC_MAGIC_CONTAINER_H

#include <array>
#include <cstdint>

namespace phase4::engine::moves::magic {

class MagicContainer {
public:
	uint64_t Mask;
	uint64_t MagicNumber;
	std::array<uint64_t, 64> Attacks;
	int32_t Shift;
};

} //namespace phase4::engine::moves::magic

#endif
