#ifndef PHASE4_ENGINE_BOARD_TRANSPOSITION_TRANSPOSITION_TABLE_ENTRY_FLAGS_H
#define PHASE4_ENGINE_BOARD_TRANSPOSITION_TRANSPOSITION_TABLE_ENTRY_FLAGS_H

#include <cstdint>

namespace phase4::engine::board::transposition {

enum TranspositionTableEntryFlags : uint8_t {
	INVALID = 0,
	EXACT_SCORE = 1,
	BETA_SCORE = 2,
	ALPHA_SCORE = 4
};

}

#endif
