#ifndef PHASE4_ENGINE_COMMON_PIECE_TYPE_H
#define PHASE4_ENGINE_COMMON_PIECE_TYPE_H

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
	static const PieceType END;

	/// @brief gets the raw internal value
	/// @return the raw internal value
	[[nodiscard]] constexpr uint8_t get_raw_value() const;

	constexpr PieceType(PieceType const &that);
	constexpr PieceType &operator=(const PieceType &that);

	constexpr PieceType(PieceType &&that) noexcept;
	constexpr PieceType &operator=(PieceType &&that) noexcept;

	constexpr bool operator==(const PieceType &) const;

	friend std::ostream &operator<<(std::ostream &os, const PieceType &color);

private:
	constexpr PieceType(uint64_t value);
	constexpr PieceType &operator=(uint64_t value);

	uint8_t m_piece;
};

constexpr PieceType::PieceType(uint64_t value) :
		m_piece{ static_cast<uint8_t>(value) } {
}

constexpr PieceType &PieceType::operator=(uint64_t value) {
	m_piece = static_cast<uint8_t>(value);
	return *this;
}

inline constexpr PieceType PieceType::PAWN = 0;
inline constexpr PieceType PieceType::KNIGHT = 1;
inline constexpr PieceType PieceType::BISHOP = 2;
inline constexpr PieceType PieceType::ROOK = 3;
inline constexpr PieceType PieceType::QUEEN = 4;
inline constexpr PieceType PieceType::KING = 5;
inline constexpr PieceType PieceType::END = 6;

constexpr PieceType::PieceType(const PieceType &that) :
		m_piece{ that.m_piece } {
}

constexpr PieceType &PieceType::operator=(const PieceType &that) {
	m_piece = that.m_piece;
	return *this;
}

constexpr PieceType::PieceType(PieceType &&that) noexcept :
		m_piece{ std::move(that).m_piece } {
}

constexpr PieceType &PieceType::operator=(PieceType &&that) noexcept {
	m_piece = std::move(that).m_piece;
	return *this;
}

[[nodiscard]] constexpr uint8_t PieceType::get_raw_value() const {
	return m_piece;
}

} //namespace phase4::engine::common

#endif
