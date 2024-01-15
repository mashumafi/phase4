#ifndef PHASE4_ENGINE_BOARD_ZOBRIST_HASHING_H
#define PHASE4_ENGINE_BOARD_ZOBRIST_HASHING_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/random.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/common/util.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board {

class ZobristHashing {
public:
	struct Keys {
		static constexpr int BOARD_SIZE = 64;
		static constexpr int NUM_COLORS = 2;
		static constexpr int NUM_PIECES = 6;
		static constexpr int NUM_CASTLING_OPTIONS = 4;
		static constexpr int NUM_EN_PASSANT_OPTIONS = 8;
		static constexpr int NUM_FIELD_HASHES = NUM_COLORS * NUM_PIECES * BOARD_SIZE;

		template <size_t Size>
		static constexpr std::array<uint64_t, Size> populateHashArray(common::Random &random) {
			return populateRow(random, std::make_index_sequence<Size>{});
		}

		// Recursive constexpr function to populate the array
		template <std::size_t SQUARE>
		static constexpr uint64_t populateElement(common::Random &random) {
			return random.next();
		}

		// Recursive constexpr function to fill a row of the array
		template <std::size_t... SQUARE>
		static constexpr std::array<uint64_t, sizeof...(SQUARE)> populateRow(common::Random &random, std::index_sequence<SQUARE...>) {
			return { populateElement<SQUARE>(random)... };
		}

		constexpr Keys(common::Random random);

		uint64_t m_sideHash;
		std::array<uint64_t, NUM_CASTLING_OPTIONS> m_castlingHashes;
		std::array<uint64_t, NUM_EN_PASSANT_OPTIONS> m_enPassantHashes;
		MultiArray<uint64_t, NUM_COLORS, NUM_PIECES, BOARD_SIZE> m_fieldHashes;
		std::array<uint64_t, BOARD_SIZE> m_wallHash;
	};

	constexpr ZobristHashing(uint64_t hash = 0);

	[[nodiscard]] constexpr ZobristHashing movePiece(int color, int piece, uint8_t from, uint8_t to);

	[[nodiscard]] constexpr ZobristHashing addOrRemovePiece(int color, int piece, uint8_t at);

	[[nodiscard]] constexpr ZobristHashing removeCastlingFlag(uint8_t currentCastling, uint8_t castlingChange);

	[[nodiscard]] constexpr ZobristHashing toggleEnPassant(int enPassantRank);

	[[nodiscard]] constexpr ZobristHashing changeSide();

	[[nodiscard]] constexpr ZobristHashing toggleWalls(uint64_t walls);

	constexpr bool operator==(ZobristHashing other) const;
	constexpr bool operator!=(ZobristHashing other) const;

private:
	uint64_t m_hash;

	static const Keys G_KEYS;
};

constexpr ZobristHashing::Keys::Keys(common::Random random) :
		m_sideHash(random.next()),
		m_castlingHashes(populateHashArray<NUM_CASTLING_OPTIONS>(random)),
		m_enPassantHashes(populateHashArray<NUM_EN_PASSANT_OPTIONS>(random)),
		m_fieldHashes(populateHashArray<NUM_FIELD_HASHES>(random)),
		m_wallHash(populateHashArray<BOARD_SIZE>(random)) {
}

constexpr ZobristHashing::Keys ZobristHashing::G_KEYS(common::Random(123456));

constexpr ZobristHashing::ZobristHashing(uint64_t hash) :
		m_hash(hash) {
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::movePiece(int color, int piece, uint8_t from, uint8_t to) {
	const uint64_t fromHash = G_KEYS.m_fieldHashes(static_cast<size_t>(color), static_cast<size_t>(piece), static_cast<size_t>(from));
	const uint64_t toHash = G_KEYS.m_fieldHashes(static_cast<size_t>(color), static_cast<size_t>(piece), static_cast<size_t>(to));
	return m_hash ^ fromHash ^ toHash;
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::addOrRemovePiece(int color, int piece, uint8_t at) {
	return m_hash ^ G_KEYS.m_fieldHashes(static_cast<size_t>(color), static_cast<size_t>(piece), static_cast<size_t>(at));
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::removeCastlingFlag(uint8_t currentCastling, uint8_t castlingChange) {
	if (likely((currentCastling & castlingChange) != 0)) {
		return m_hash ^ G_KEYS.m_castlingHashes[common::Bitset(castlingChange).bitScan()];
	}

	return m_hash;
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::toggleEnPassant(int enPassantRank) {
	return m_hash ^ G_KEYS.m_enPassantHashes[enPassantRank];
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::changeSide() {
	return m_hash ^ G_KEYS.m_sideHash;
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::toggleWalls(uint64_t walls) {
	return m_hash ^ G_KEYS.m_wallHash[common::Bitset(walls).bitScan()];
}

inline constexpr bool ZobristHashing::operator==(ZobristHashing other) const {
	return m_hash == other.m_hash;
}

inline constexpr bool ZobristHashing::operator!=(ZobristHashing other) const {
	return m_hash != other.m_hash;
}

} //namespace phase4::engine::board

#endif
