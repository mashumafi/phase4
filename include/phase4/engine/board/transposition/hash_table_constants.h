#ifndef PHASE4_ENGINE_BOARD_TRANSPOSITION_HASH_TABLE_CONSTANTS_H
#define PHASE4_ENGINE_BOARD_TRANSPOSITION_HASH_TABLE_CONSTANTS_H

#include <cstdint>

namespace phase4::engine::board::transposition {

class HashTableConstants {
public:
	static constexpr size_t DEFAULT_HASH_TABLES_SIZE = 32;

	/*
	 * For every 128 megabytes of memory, there are:
	 *  - 105 megabytes of transposition table
	 *  - 21 megabytes of evaluation hash table
	 *  - 2 megabyte of pawn hash table
	 */

	static constexpr size_t PAWN_HASH_TABLE_SIZE_DIVIDER = 64;

	static constexpr size_t EVALUATION_HASH_TABLE_SIZE_DIVIDER = 6;
};

} //namespace phase4::engine::board::transposition

#endif
