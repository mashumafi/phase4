#ifndef PHASE4_ENGINE_BOARD_TRANSPOSITION_EVALUATION_HASH_TABLE_ENTRY_H
#define PHASE4_ENGINE_BOARD_TRANSPOSITION_EVALUATION_HASH_TABLE_ENTRY_H

#include <phase4/engine/board/transposition/evaluation_hash_table_entry.h>

#include <phase4/engine/common/bitboard.h>

#include <cstdint>

namespace phase4::engine::board::transposition {

class EvaluationHashTableEntry {
public:
	EvaluationHashTableEntry() :
			m_key(0), m_score(0) {
	}

	EvaluationHashTableEntry(common::Bitboard hash, int16_t score) {
		m_key = static_cast<uint16_t>(hash.get_raw_value() >> 48);
		m_score = score;
	}

	uint16_t key() const {
		return m_key;
	}

	int16_t score() const {
		return m_score;
	}

	bool isKeyValid(common::Bitboard hash) const {
		return m_key == hash.get_raw_value() >> 48;
	}

private:
	uint16_t m_key;
	int16_t m_score;
};

} //namespace phase4::engine::board::transposition

#endif
