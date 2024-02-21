#ifndef PHASE4_ENGINE_BOARD_TRANSPOSITION_PAWN_HASH_TABLE_ENTRY_H
#define PHASE4_ENGINE_BOARD_TRANSPOSITION_PAWN_HASH_TABLE_ENTRY_H

#include <phase4/engine/common/bitboard.h>

#include <cstdint>

namespace phase4::engine::board::transposition {

class PawnHashTableEntry {
public:
	PawnHashTableEntry() :
			m_data(0) {
	}

	PawnHashTableEntry(common::Bitboard hash, int16_t openingScore, int16_t endingScore) {
		const uint32_t hashPart = (uint32_t)((hash.get_raw_value() >> 52) << 20);
		const uint32_t endingScorePart = (uint32_t)((endingScore & 0x3FF) << 10);
		const uint32_t openingScorePart = (uint32_t)(openingScore & 0x3FF);

		m_data = hashPart | endingScorePart | openingScorePart;
	}

	uint16_t key() const {
		return (uint16_t)(m_data >> 20);
	}

	int16_t endingScore() const {
		const int16_t result = (int16_t)((m_data >> 10) & 0x3FF);
		return result < 512 ? result : (int16_t)(result - 1024);
	}

	int16_t openingScore() const {
		const int16_t result = (int16_t)(m_data & 0x3FF);
		return result < 512 ? result : (int16_t)(result - 1024);
	}

	bool isKeyValid(common::Bitboard hash) const {
		return key() == hash.get_raw_value() >> 52;
	}

private:
	uint32_t m_data;
};

} //namespace phase4::engine::board::transposition

#endif
