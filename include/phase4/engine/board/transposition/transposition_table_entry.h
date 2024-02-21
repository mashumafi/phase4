#ifndef PHASE4_ENGINE_BOARD_TRANSPOSITION_TRANSPOSITION_TABLE_ENTRY_H
#define PHASE4_ENGINE_BOARD_TRANSPOSITION_TRANSPOSITION_TABLE_ENTRY_H

#include <phase4/engine/board/transposition/transposition_table_entry_flags.h>

#include <phase4/engine/moves/move.h>

#include <phase4/engine/common/bitboard.h>

#include <cstdint>

namespace phase4::engine::board::transposition {

class TranspositionTableEntry {
public:
	inline TranspositionTableEntry() :
			m_key(0),
			m_score(0),
			m_bestMove(),
			m_depth(0),
			m_ageFlagsField(0) {
	}

	inline TranspositionTableEntry(common::Bitboard hash, int16_t score, moves::Move bestMove, uint8_t depth, TranspositionTableEntryFlags flags, uint8_t age) {
		m_key = static_cast<uint16_t>(hash.get_raw_value() >> 48);
		m_score = score;
		m_bestMove = bestMove;
		m_depth = depth;

		m_ageFlagsField = static_cast<uint8_t>(flags);
		m_ageFlagsField |= static_cast<uint8_t>(age << 3);
	}

	inline uint16_t key() const {
		return m_key;
	}

	inline int16_t score() const {
		return m_score;
	}

	inline int16_t &score() {
		return m_score;
	}

	inline moves::Move bestMove() const {
		return m_bestMove;
	}

	inline uint8_t depth() const {
		return m_depth;
	}

	inline uint8_t age() const {
		return m_ageFlagsField >> 3;
	}

	inline TranspositionTableEntryFlags flags() const {
		return static_cast<TranspositionTableEntryFlags>(m_ageFlagsField & 7);
	}

	inline bool isKeyValid(common::Bitboard hash) const {
		return m_key == static_cast<uint16_t>(hash.get_raw_value() >> 48);
	}

private:
	uint16_t m_key;
	int16_t m_score;
	moves::Move m_bestMove;
	uint8_t m_depth;
	uint8_t m_ageFlagsField;
};

} //namespace phase4::engine::board::transposition

#endif
