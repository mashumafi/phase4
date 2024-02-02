#ifndef PHASE4_ENGINE_BOARD_TRANSPOSITION_PAWN_HASH_TABLE_H
#define PHASE4_ENGINE_BOARD_TRANSPOSITION_PAWN_HASH_TABLE_H

#include <phase4/engine/board/transposition/pawn_hash_table_entry.h>

#include <phase4/engine/common/bitset.h>

#include <array>

namespace phase4::engine::board::transposition {

template <size_t MB>
class PawnHashTable {
public:
	using Table = std::array<PawnHashTableEntry, MB * 1024ull * 1024ull / sizeof(PawnHashTableEntry)>;

	void add(common::Bitset hash, int16_t openingScore, int16_t endingScore) {
		m_table[hash.get_raw_value() % m_table.size()] = PawnHashTableEntry(hash, openingScore, endingScore);
	}

	PawnHashTableEntry get(common::Bitset hash) const {
		return m_table[hash.get_raw_value() % m_table.size()];
	}

	void clear() {
		m_table.fill(PawnHashTableEntry());
	}

private:
	Table m_table;
};

} //namespace phase4::engine::board::transposition

#endif
