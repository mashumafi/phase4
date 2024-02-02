#ifndef PHASE4_ENGINE_BOARD_TRANSPOSITION_TRANSPOSITION_TABLE_H
#define PHASE4_ENGINE_BOARD_TRANSPOSITION_TRANSPOSITION_TABLE_H

#include <phase4/engine/board/search_constants.h>
#include <phase4/engine/board/transposition/transposition_table_entry.h>

#include <phase4/engine/common/bitset.h>

#include <array>

namespace phase4::engine::board::transposition {

template <size_t MB>
class TranspositionTable {
public:
	using Table = std::array<TranspositionTableEntry, MB * 1024ul * 1024ul / sizeof(TranspositionTableEntry)>;

	inline void add(common::Bitset hash, const TranspositionTableEntry &entry) {
		m_table[hash.get_raw_value() % m_table.size()] = entry;
	}

	inline const TranspositionTableEntry &get(common::Bitset hash) const {
		return m_table[hash.get_raw_value() % m_table.size()];
	}

	inline TranspositionTableEntry &get(common::Bitset hash) {
		return m_table[hash.get_raw_value() % m_table.size()];
	}

	void clear() {
		m_table.fill(TranspositionTableEntry());
	}

	inline static int32_t regularToTranspositionTableScore(int32_t score, int32_t ply) {
		if (SearchConstants::isScoreNearCheckmate(score)) {
			if (score > 0) {
				return score + ply;
			} else {
				return score - ply;
			}
		}

		return score;
	}

	inline static int32_t transpositionTableToRegularScore(int32_t score, int32_t ply) {
		if (SearchConstants::isScoreNearCheckmate(score)) {
			if (score > 0) {
				return score - ply;
			} else {
				return score + ply;
			}
		}

		return score;
	}

private:
	Table m_table;
};

} //namespace phase4::engine::board::transposition

#endif
