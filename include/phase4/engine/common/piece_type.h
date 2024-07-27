#ifndef PHASE4_ENGINE_COMMON_PIECE_TYPE_H
#define PHASE4_ENGINE_COMMON_PIECE_TYPE_H

#include <phase4/engine/common/piece_color.h>

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

namespace phase4::engine::common {

class PieceType {
public:
	static const PieceType PAWN;
	static const PieceType KNIGHT;
	static const PieceType BISHOP;
	static const PieceType ROOK;
	static const PieceType QUEEN;
	static const PieceType KING;
	static const PieceType INVALID;

	/// @brief gets the raw internal value
	/// @return the raw internal value
	[[nodiscard]] constexpr uint8_t get_raw_value() const;

	inline constexpr PieceType();

	inline constexpr PieceType(PieceType const &that) = default;
	inline constexpr PieceType &operator=(const PieceType &that) = default;

	inline constexpr PieceType(PieceType &&that) noexcept = default;
	inline constexpr PieceType &operator=(PieceType &&that) noexcept = default;

	inline constexpr bool operator==(const PieceType &that) const;
	inline constexpr bool operator!=(const PieceType &that) const;

	inline constexpr PieceType operator++();

	inline constexpr char toSymbol() const;
	inline constexpr wchar_t toSymbol(const PieceColor &color) const;

	friend std::ostream &operator<<(std::ostream &os, PieceType color);

private:
	inline constexpr PieceType(uint64_t value);
	inline constexpr PieceType &operator=(uint64_t value);

	uint8_t m_piece;
};

inline constexpr PieceType::PieceType(uint64_t value) :
		m_piece{ static_cast<uint8_t>(value) } {
}

inline constexpr PieceType &PieceType::operator=(uint64_t value) {
	m_piece = static_cast<uint8_t>(value);
	return *this;
}

inline constexpr PieceType::PieceType() :
		PieceType(6) {
}

inline constexpr PieceType PieceType::PAWN = 0;
inline constexpr PieceType PieceType::KNIGHT = 1;
inline constexpr PieceType PieceType::BISHOP = 2;
inline constexpr PieceType PieceType::ROOK = 3;
inline constexpr PieceType PieceType::QUEEN = 4;
inline constexpr PieceType PieceType::KING = 5;
inline constexpr PieceType PieceType::INVALID = 6;

[[nodiscard]] inline constexpr uint8_t PieceType::get_raw_value() const {
	return m_piece;
}

inline constexpr bool PieceType::operator==(const PieceType &that) const {
	return m_piece == that.m_piece;
}

inline constexpr bool PieceType::operator!=(const PieceType &that) const {
	return m_piece != that.m_piece;
}

inline constexpr PieceType PieceType::operator++() {
	return m_piece++;
}

inline constexpr char PieceType::toSymbol() const {
	constexpr std::array<char, 6> symbols{
		'p',
		'n',
		'b',
		'r',
		'q',
		'k',
	};

	assert(m_piece != PieceType::INVALID.m_piece);

	return symbols[m_piece];
}

inline constexpr wchar_t PieceType::toSymbol(const PieceColor &color) const {
	constexpr std::array<wchar_t, 12> symbols{
		L'♟',
		L'♞',
		L'♝',
		L'♜',
		L'♛',
		L'♚',
		L'♙',
		L'♘',
		L'♗',
		L'♖',
		L'♕',
		L'♔',
	};

	assert(m_piece != PieceType::INVALID.m_piece);
	assert(color != PieceColor::INVALID);
	return symbols[m_piece + 6 * color.get_raw_value()];
}

inline std::ostream &operator<<(std::ostream &os, PieceType color) {
	switch (color.m_piece) {
		case PieceType::PAWN.m_piece:
			return os << "PAWN";
		case PieceType::KNIGHT.m_piece:
			return os << "KNIGHT";
		case PieceType::BISHOP.m_piece:
			return os << "BISHOP";
		case PieceType::ROOK.m_piece:
			return os << "ROOK";
		case PieceType::QUEEN.m_piece:
			return os << "QUEEN";
		case PieceType::KING.m_piece:
			return os << "KING";
		default:
			os.setstate(std::ios_base::failbit);
			return os;
	}
}

} //namespace phase4::engine::common

#endif
