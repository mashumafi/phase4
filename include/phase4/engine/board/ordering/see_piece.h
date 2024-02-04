#ifndef PHASE4_ENGINE_BOARD_ORDERING_SEE_PIECE_H
#define PHASE4_ENGINE_BOARD_ORDERING_SEE_PIECE_H

#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/moves_generator.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/square.h>

#include <cstdint>

namespace phase4::engine::board::ordering {

class SeePiece {
public:
	static const SeePiece PAWN;
	static const SeePiece KNIGHT1;
	static const SeePiece KNIGHT2;
	static const SeePiece BISHOP;
	static const SeePiece ROOK1;
	static const SeePiece ROOK2;
	static const SeePiece QUEEN;
	static const SeePiece KING;
	static const SeePiece INVALID;

	[[nodiscard]] constexpr uint8_t get_raw_value() const {
		return m_value;
	}

	static uint8_t getAttackingPiecesWithColor(const board::Position &position, common::PieceColor color, common::Square fieldIndex) {
		uint8_t result = 0;

		const common::Bitset jumpAttacks = moves::MovesGenerator::getKnightMoves(fieldIndex);
		const common::Bitset attackingKnights = jumpAttacks & position.colorPieceMask(color, common::PieceType::KNIGHT);
		const uint8_t attackingKnightsCount = attackingKnights.count();
		if (attackingKnightsCount != 0) {
			result |= (uint8_t)((attackingKnightsCount == 1 ? 1 : 3) << SeePiece::KNIGHT1.m_value);
		}

		const common::Bitset diagonalAttacks = moves::MovesGenerator::getBishopMoves(position.m_occupancySummary, fieldIndex) & position.occupancy(color);
		const common::Bitset attackingBishops = diagonalAttacks & position.colorPieceMask(color, common::PieceType::BISHOP);
		if (attackingBishops != 0) {
			result |= 1 << SeePiece::BISHOP.m_value;
		}

		const common::Bitset occupancyWithoutFileRankPieces = position.m_occupancySummary & ~position.colorPieceMask(color, common::PieceType::ROOK) & ~position.colorPieceMask(color, common::PieceType::QUEEN);
		const common::Bitset fileRankAttacks = moves::MovesGenerator::getRookMoves(occupancyWithoutFileRankPieces, fieldIndex) & position.occupancy(color);
		const common::Bitset attackingRooks = fileRankAttacks & position.colorPieceMask(color, common::PieceType::ROOK);
		uint8_t attackingRooksCount = attackingRooks.count();
		if (attackingRooksCount != 0) {
			result |= (uint8_t)((attackingRooksCount == 1 ? 1 : 3) << SeePiece::ROOK1.m_value);
		}

		const common::Bitset attackingQueens = (fileRankAttacks | diagonalAttacks) & position.colorPieceMask(color, common::PieceType::QUEEN);
		if (attackingQueens != 0) {
			result |= 1 << SeePiece::QUEEN.m_value;
		}

		const common::Bitset boxAttacks = moves::MovesGenerator::getKingMoves(fieldIndex);
		const common::Bitset attackingKings = boxAttacks & position.colorPieceMask(color, common::PieceType::KING);
		if (attackingKings != 0) {
			result |= 1 << SeePiece::KING.m_value;
		}

		const common::Bitset field = fieldIndex.asBitboard();
		const common::Bitset potentialPawns = boxAttacks & position.colorPieceMask(color, common::PieceType::PAWN);
		const common::Bitset attackingPawns = color == common::PieceColor::WHITE ? field & ((potentialPawns << 7) | (potentialPawns << 9)) : field & ((potentialPawns >> 7) | (potentialPawns >> 9));

		if (attackingPawns != 0) {
			result |= 1 << SeePiece::PAWN.m_value;
		}

		return result;
	}

private:
	constexpr SeePiece(uint64_t value);
	constexpr SeePiece &operator=(uint64_t value);

	uint8_t m_value;
};

constexpr SeePiece::SeePiece(uint64_t value) :
		m_value(static_cast<uint8_t>(value)) {
}

constexpr SeePiece &SeePiece::operator=(uint64_t value) {
	m_value = static_cast<uint8_t>(value);
	return *this;
}

inline constexpr SeePiece SeePiece::PAWN = 0;
inline constexpr SeePiece SeePiece::KNIGHT1 = 1;
inline constexpr SeePiece SeePiece::KNIGHT2 = 2;
inline constexpr SeePiece SeePiece::BISHOP = 3;
inline constexpr SeePiece SeePiece::ROOK1 = 4;
inline constexpr SeePiece SeePiece::ROOK2 = 5;
inline constexpr SeePiece SeePiece::QUEEN = 6;
inline constexpr SeePiece SeePiece::KING = 7;
inline constexpr SeePiece SeePiece::INVALID = 8;

} //namespace phase4::engine::board::ordering

#endif
