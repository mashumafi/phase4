#ifndef PHASE4_ENGINE_TRANSPOSITION_HASH_TABLES_H
#define PHASE4_ENGINE_TRANSPOSITION_HASH_TABLES_H

#include <phase4/engine/transposition/hash_table_constants.h>
#include <phase4/engine/transposition/pawn_hash_table.h>

#include <phase4/engine/common/math.h>

namespace phase4::engine::transposition {

template <size_t MB = HashTableConstants::DEFAULT_HASH_TABLES_SIZE>
class HashTables {
public:
	PawnHashTable<common::Math::max_size(1, MB / HashTableConstants::PAWN_HASH_TABLE_SIZE_DIVIDER)> m_pawnHashTable;
};

} //namespace phase4::engine::transposition

#endif
