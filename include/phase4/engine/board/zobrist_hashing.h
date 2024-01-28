#ifndef PHASE4_ENGINE_BOARD_ZOBRIST_HASHING_H
#define PHASE4_ENGINE_BOARD_ZOBRIST_HASHING_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/castling.h>
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

	inline constexpr ZobristHashing(uint64_t hash = 0);

	[[nodiscard]] constexpr ZobristHashing movePiece(common::PieceColor color, common::PieceType piece, common::Square from, common::Square to) const noexcept;

	[[nodiscard]] constexpr ZobristHashing addOrRemovePiece(common::PieceColor color, common::PieceType piece, common::Square at) const noexcept;

	[[nodiscard]] constexpr ZobristHashing removeCastlingFlag(common::Castling currentCastling, common::Castling castlingChange) const noexcept;

	[[nodiscard]] constexpr ZobristHashing toggleEnPassant(uint8_t enPassantRank) const noexcept;

	[[nodiscard]] constexpr ZobristHashing changeSide() const noexcept;

	[[nodiscard]] constexpr ZobristHashing toggleWalls(common::Bitset walls) const noexcept;

	[[nodiscard]] constexpr common::Bitset asBitboard() const noexcept {
		return common::Bitset(m_hash);
	}

	inline constexpr bool operator==(ZobristHashing other) const;
	inline constexpr bool operator!=(ZobristHashing other) const;

	friend std::ostream &operator<<(std::ostream &os, const ZobristHashing bits);

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

inline constexpr ZobristHashing::Keys ZobristHashing::G_KEYS(common::Random(123456));

inline constexpr ZobristHashing::ZobristHashing(uint64_t hash) :
		m_hash(hash) {
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::movePiece(common::PieceColor color, common::PieceType piece, common::Square from, common::Square to) const noexcept {
	const uint64_t fromHash = G_KEYS.m_fieldHashes.at(color.get_raw_value(), piece.get_raw_value(), from);
	const uint64_t toHash = G_KEYS.m_fieldHashes.at(color.get_raw_value(), piece.get_raw_value(), to);
	return m_hash ^ fromHash ^ toHash;
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::addOrRemovePiece(common::PieceColor color, common::PieceType piece, common::Square at) const noexcept {
	return m_hash ^ G_KEYS.m_fieldHashes.at(color.get_raw_value(), piece.get_raw_value(), at);
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::removeCastlingFlag(common::Castling currentCastling, common::Castling castlingChange) const noexcept {
	if (likely((currentCastling & castlingChange) != common::Castling::NONE)) {
		return m_hash ^ G_KEYS.m_castlingHashes[common::Bitset(castlingChange.get_raw_value()).fastBitScan()];
	}

	return m_hash;
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::toggleEnPassant(uint8_t enPassantRank) const noexcept {
	return m_hash ^ G_KEYS.m_enPassantHashes[enPassantRank];
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::changeSide() const noexcept {
	return m_hash ^ G_KEYS.m_sideHash;
}

[[nodiscard]] constexpr ZobristHashing ZobristHashing::toggleWalls(common::Bitset walls) const noexcept {
	return m_hash ^ G_KEYS.m_wallHash[walls.bitScan()];
}

inline constexpr bool ZobristHashing::operator==(ZobristHashing other) const {
	return m_hash == other.m_hash;
}

inline constexpr bool ZobristHashing::operator!=(ZobristHashing other) const {
	return m_hash != other.m_hash;
}

inline std::ostream &operator<<(std::ostream &os, const ZobristHashing bits) {
	return os << std::bitset<64>(bits.m_hash);
}

} //namespace phase4::engine::board

#endif
