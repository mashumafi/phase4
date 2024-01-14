#ifndef PHASE4_ENGINE_MOVES_MOVE_FLAGS_H
#define PHASE4_ENGINE_MOVES_MOVE_FLAGS_H

#include <phase4/engine/common/piece_type.h>

#include <cstdint>
#include <iostream>

namespace phase4::engine::moves {

class MoveFlags {
public:
	enum class Fields : uint8_t {
		SPECIAL_0 = 1,
		SPECIAL_1 = 2,
		CAPTURE = 4,
		PROMOTION = 8,
	};

	static const MoveFlags QUIET;
	static const MoveFlags DOUBLE_PUSH;

	static const MoveFlags KING_CASTLE;
	static const MoveFlags QUEEN_CASTLE;
	static const MoveFlags CAPTURE;
	static const MoveFlags EN_PASSANT;

	static const MoveFlags KNIGHT_PROMOTION;
	static const MoveFlags BISHOP_PROMOTION;
	static const MoveFlags ROOK_PROMOTION;
	static const MoveFlags QUEEN_PROMOTION;

	static const MoveFlags KNIGHT_PROMOTION_CAPTURE;
	static const MoveFlags BISHOP_PROMOTION_CAPTURE;
	static const MoveFlags ROOK_PROMOTION_CAPTURE;
	static const MoveFlags QUEEN_PROMOTION_CAPTURE;

	/// @brief gets the raw internal value
	/// @return the raw internal value
	[[nodiscard]] constexpr uint8_t get_raw_value() const;

	[[nodiscard]] constexpr char getPromotionSymbol() const;
	[[nodiscard]] constexpr common::PieceType getPromotionPiece() const;

	[[nodiscard]] bool isQuiet() const {
		return m_flags == MoveFlags::QUIET.m_flags || m_flags == MoveFlags::DOUBLE_PUSH.m_flags;
	}

	[[nodiscard]] bool isSinglePush() const {
		return m_flags == MoveFlags::QUIET.m_flags;
	}

	[[nodiscard]] bool isDoublePush() const {
		return m_flags == MoveFlags::DOUBLE_PUSH.m_flags;
	}

	[[nodiscard]] bool isEnPassant() const {
		return m_flags == MoveFlags::EN_PASSANT.m_flags;
	}

	[[nodiscard]] constexpr bool isCapture() const {
		return (m_flags & static_cast<uint8_t>(Fields::CAPTURE)) != 0;
	}

	[[nodiscard]] bool isCastling() const {
		return m_flags == MoveFlags::KING_CASTLE.m_flags || m_flags == MoveFlags::QUEEN_CASTLE.m_flags;
	}

	[[nodiscard]] bool isKingCastling() const {
		return m_flags == MoveFlags::KING_CASTLE.m_flags;
	}

	[[nodiscard]] bool isQueenCastling() const {
		return m_flags == MoveFlags::QUEEN_CASTLE.m_flags;
	}

	[[nodiscard]] constexpr bool isPromotion() const {
		return (m_flags & static_cast<uint8_t>(Fields::PROMOTION)) != 0;
	}

	[[nodiscard]] static constexpr MoveFlags getPromotionSymbolFlags(char promotionSymbol) {
		switch (promotionSymbol) {
			case 'q':
				return QUEEN_PROMOTION;
			case 'r':
				return ROOK_PROMOTION;
			case 'b':
				return BISHOP_PROMOTION;
			case 'n':
				return KNIGHT_PROMOTION;
		}

		return MoveFlags(0);
	}

	constexpr MoveFlags(MoveFlags const &that);
	constexpr MoveFlags &operator=(const MoveFlags &that);

	constexpr MoveFlags(MoveFlags &&that) noexcept;
	constexpr MoveFlags &operator=(MoveFlags &&that) noexcept;

	constexpr bool operator==(const MoveFlags &) const;

	friend std::ostream &operator<<(std::ostream &os, const MoveFlags &color);

private:
	constexpr MoveFlags(uint64_t value);
	constexpr MoveFlags &operator=(uint64_t value);

	friend class Move;

	uint8_t m_flags;
};

constexpr MoveFlags::MoveFlags(uint64_t value) :
		m_flags{ static_cast<uint8_t>(value) } {
}

constexpr MoveFlags &MoveFlags::operator=(uint64_t value) {
	m_flags = static_cast<uint8_t>(value);
	return *this;
}

inline constexpr MoveFlags MoveFlags::QUIET = 0;
inline constexpr MoveFlags MoveFlags::DOUBLE_PUSH = 1;

inline constexpr MoveFlags MoveFlags::KING_CASTLE = 2;
inline constexpr MoveFlags MoveFlags::QUEEN_CASTLE = 3;
inline constexpr MoveFlags MoveFlags::CAPTURE = 4;
inline constexpr MoveFlags MoveFlags::EN_PASSANT = 5;

inline constexpr MoveFlags MoveFlags::KNIGHT_PROMOTION = 8;
inline constexpr MoveFlags MoveFlags::BISHOP_PROMOTION = 9;
inline constexpr MoveFlags MoveFlags::ROOK_PROMOTION = 10;
inline constexpr MoveFlags MoveFlags::QUEEN_PROMOTION = 11;

inline constexpr MoveFlags MoveFlags::KNIGHT_PROMOTION_CAPTURE = 12;
inline constexpr MoveFlags MoveFlags::BISHOP_PROMOTION_CAPTURE = 13;
inline constexpr MoveFlags MoveFlags::ROOK_PROMOTION_CAPTURE = 14;
inline constexpr MoveFlags MoveFlags::QUEEN_PROMOTION_CAPTURE = 15;

constexpr MoveFlags::MoveFlags(const MoveFlags &that) :
		m_flags{ that.m_flags } {
}

constexpr MoveFlags &MoveFlags::operator=(const MoveFlags &that) {
	m_flags = that.m_flags;
	return *this;
}

constexpr MoveFlags::MoveFlags(MoveFlags &&that) noexcept :
		m_flags{ std::move(that).m_flags } {
}

constexpr MoveFlags &MoveFlags::operator=(MoveFlags &&that) noexcept {
	m_flags = std::move(that).m_flags;
	return *this;
}

[[nodiscard]] constexpr uint8_t MoveFlags::get_raw_value() const {
	return m_flags;
}

[[nodiscard]] constexpr char MoveFlags::getPromotionSymbol() const {
	switch (m_flags) {
		case MoveFlags::QUEEN_PROMOTION.m_flags:
		case MoveFlags::QUEEN_PROMOTION_CAPTURE.m_flags:
			return 'q';
		case MoveFlags::ROOK_PROMOTION.m_flags:
		case MoveFlags::ROOK_PROMOTION_CAPTURE.m_flags:
			return 'r';
		case MoveFlags::BISHOP_PROMOTION.m_flags:
		case MoveFlags::BISHOP_PROMOTION_CAPTURE.m_flags:
			return 'b';
		case MoveFlags::KNIGHT_PROMOTION.m_flags:
		case MoveFlags::KNIGHT_PROMOTION_CAPTURE.m_flags:
			return 'n';
	}

	return ' ';
}

[[nodiscard]] constexpr common::PieceType MoveFlags::getPromotionPiece() const {
	switch (m_flags) {
		case MoveFlags::QUEEN_PROMOTION.m_flags:
		case MoveFlags::QUEEN_PROMOTION_CAPTURE.m_flags:
			return common::PieceType::QUEEN;
		case MoveFlags::ROOK_PROMOTION.m_flags:
		case MoveFlags::ROOK_PROMOTION_CAPTURE.m_flags:
			return common::PieceType::ROOK;
		case MoveFlags::BISHOP_PROMOTION.m_flags:
		case MoveFlags::BISHOP_PROMOTION_CAPTURE.m_flags:
			return common::PieceType::BISHOP;
		case MoveFlags::KNIGHT_PROMOTION.m_flags:
		case MoveFlags::KNIGHT_PROMOTION_CAPTURE.m_flags:
			return common::PieceType::KNIGHT;
	}

	return common::PieceType::END;
}

constexpr bool MoveFlags::operator==(const MoveFlags &flags) const {
	return flags.m_flags == m_flags;
}

inline std::ostream &operator<<(std::ostream &os, const MoveFlags &flags) {
	switch (flags.m_flags) {
		case MoveFlags::QUIET.m_flags:
			os << "QUIET";
			break;
		case MoveFlags::DOUBLE_PUSH.m_flags:
			os << "DOUBLE_PUSH";
			break;
		case MoveFlags::KING_CASTLE.m_flags:
			os << "KING_CASTLE";
			break;
		case MoveFlags::QUEEN_CASTLE.m_flags:
			os << "QUEEN_CASTLE";
			break;
		case MoveFlags::CAPTURE.m_flags:
			os << "CAPTURE";
			break;
		case MoveFlags::EN_PASSANT.m_flags:
			os << "EN_PASSANT";
			break;
		case MoveFlags::KNIGHT_PROMOTION.m_flags:
			os << "KNIGHT_PROMOTION";
			break;
		case MoveFlags::BISHOP_PROMOTION.m_flags:
			os << "BISHOP_PROMOTION";
			break;
		case MoveFlags::ROOK_PROMOTION.m_flags:
			os << "ROOK_PROMOTION";
			break;
		case MoveFlags::QUEEN_PROMOTION.m_flags:
			os << "QUEEN_PROMOTION";
			break;
		case MoveFlags::KNIGHT_PROMOTION_CAPTURE.m_flags:
			os << "KNIGHT_PROMOTION_CAPTURE";
			break;
		case MoveFlags::BISHOP_PROMOTION_CAPTURE.m_flags:
			os << "BISHOP_PROMOTION_CAPTURE";
			break;
		case MoveFlags::ROOK_PROMOTION_CAPTURE.m_flags:
			os << "ROOK_PROMOTION_CAPTURE";
			break;
		case MoveFlags::QUEEN_PROMOTION_CAPTURE.m_flags:
			os << "QUEEN_PROMOTION_CAPTURE";
			break;
		default:
			os.setstate(std::ios_base::failbit);
			break;
	}
	return os;
}

} //namespace phase4::engine::moves

#endif
