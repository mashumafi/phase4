#ifndef PHASE4_ENGINE_BOARD_TRANSPOSITION_EVALUATION_HASH_TABLE_H
#define PHASE4_ENGINE_BOARD_TRANSPOSITION_EVALUATION_HASH_TABLE_H

#include <phase4/engine/board/transposition/evaluation_hash_table_entry.h>

#include <phase4/engine/common/util.h>

#include <array>

namespace phase4::engine::board::transposition {

template <size_t MB>
class EvaluationHashTable {
public:
	static constexpr size_t SIZE = MB * 1024ull * 1024ull / sizeof(EvaluationHashTableEntry);
	using Table = std::array<EvaluationHashTableEntry, SIZE>;

	void add(common::Bitboard hash, int16_t score) {
		m_table[hash.get_raw_value() % m_table.size()] = EvaluationHashTableEntry(hash, score);
	}

	const EvaluationHashTableEntry &get(common::Bitboard hash) const {
		return m_table[hash.get_raw_value() % m_table.size()];
	}

	void clear() {
		common::util::clear(m_table);
	}

private:
	Table m_table;
};

} //namespace phase4::engine::board::transposition

#endif
