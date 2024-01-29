#ifndef PHASE4_ENGINE_BOARD_TRANSPOSITION_TRANSPOSITION_TABLE_H
#define PHASE4_ENGINE_BOARD_TRANSPOSITION_TRANSPOSITION_TABLE_H

#include <phase4/engine/board/transposition/transposition_table_entry.h>

#include <phase4/engine/common/bitset.h>

#include <array>

namespace phase4::engine::board::transposition {

template <size_t MB>
class TranspositionTable {
public:
	using Table = std::array<TranspositionTableEntry, MB * 1024ul * 1024ul / sizeof(TranspositionTableEntry)>;

	void add(common::Bitset hash, TranspositionTableEntry entry) {
		m_table[hash.asSize() % m_table.size()] = entry;
	}

	TranspositionTableEntry get(common::Bitset hash) {
		return m_table[hash.asSize() % m_table.size()];
	}

	void clear() {
		static constexpr Table blank = {};
		m_table = blank;
	}

	int regularToTranpositionTableScore(int score, int ply) const {
        (void)ply;
		/*if (IterativeDeepening.IsScoreNearCheckmate(score)) {
			if (score > 0) {
				return score + ply;
			} else {
				return score - ply;
			}
		}*/

		return score;
	}

	int transpositionTableToRegularScore(int score, int ply) const {
        (void)ply;
		/*if (IterativeDeepening.IsScoreNearCheckmate(score)) {
			if (score > 0) {
				return score - ply;
			} else {
				return score + ply;
			}
		}*/

		return score;
	}

private:
	Table m_table;
};

} //namespace phase4::engine::board::transposition

#endif
