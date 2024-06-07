#ifndef PHASE4_ENGINE_COMMON_SQUARE_H
#define PHASE4_ENGINE_COMMON_SQUARE_H

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/util.h>

#include <iostream>
#include <optional>

namespace phase4::engine::common {

class Square {
public:
	using Direction = Square (Square::*)(int8_t) const;

	static const Square BEGIN;

	static const Square H1;
	static const Square G1;
	static const Square F1;
	static const Square E1;
	static const Square D1;
	static const Square C1;
	static const Square B1;
	static const Square A1;

	static const Square H2;
	static const Square G2;
	static const Square F2;
	static const Square E2;
	static const Square D2;
	static const Square C2;
	static const Square B2;
	static const Square A2;

	static const Square H3;
	static const Square G3;
	static const Square F3;
	static const Square E3;
	static const Square D3;
	static const Square C3;
	static const Square B3;
	static const Square A3;

	static const Square H4;
	static const Square G4;
	static const Square F4;
	static const Square E4;
	static const Square D4;
	static const Square C4;
	static const Square B4;
	static const Square A4;

	static const Square H5;
	static const Square G5;
	static const Square F5;
	static const Square E5;
	static const Square D5;
	static const Square C5;
	static const Square B5;
	static const Square A5;

	static const Square H6;
	static const Square G6;
	static const Square F6;
	static const Square E6;
	static const Square D6;
	static const Square C6;
	static const Square B6;
	static const Square A6;

	static const Square H7;
	static const Square G7;
	static const Square F7;
	static const Square E7;
	static const Square D7;
	static const Square C7;
	static const Square B7;
	static const Square A7;

	static const Square H8;
	static const Square G8;
	static const Square F8;
	static const Square E8;
	static const Square D8;
	static const Square C8;
	static const Square B8;
	static const Square A8;

	static const Square INVALID;

	/// @brief gets the raw internal value
	/// @return the raw internal value
	[[nodiscard]] inline constexpr int8_t get_raw_value() const;

	/// @brief converts the square index to a point
	/// @return the point
	constexpr inline FieldIndex asFieldIndex() const noexcept;

	inline constexpr Bitboard asBitboard() const noexcept;

	inline constexpr operator uint64_t() const;

	inline constexpr Square();
	inline static constexpr std::optional<Square> parse(std::string_view square);

	inline constexpr explicit Square(uint64_t value);
	inline constexpr Square &operator=(uint64_t value);

	inline explicit constexpr Square(const FieldIndex &fieldIndex);
	inline explicit Square(const Bitboard &bits);

	inline constexpr Square(Square const &that) = default;
	inline constexpr Square &operator=(const Square &that) = default;

	inline constexpr Square(Square &&that) noexcept = default;
	inline constexpr Square &operator=(Square &&that) noexcept = default;

	inline explicit constexpr Square(std::string_view move);

	inline constexpr bool isValid() const noexcept;

	inline constexpr Square forward(PieceColor color, int8_t scalar) const noexcept;
	inline constexpr Square north(int8_t scalar) const noexcept;
	inline constexpr Square east(int8_t scalar) const noexcept;
	inline constexpr Square south(int8_t scalar) const noexcept;
	inline constexpr Square west(int8_t scalar) const noexcept;

	inline constexpr Square flipped() const noexcept;

	inline constexpr Square middle(Square that) const noexcept;

	inline constexpr bool operator!=(Square piece) const;
	inline constexpr Square operator++();

	friend std::ostream &operator<<(std::ostream &os, const Square &square);

private:
	int8_t m_value;
};

[[nodiscard]] inline constexpr int8_t Square::get_raw_value() const {
	return m_value;
}

inline constexpr FieldIndex Square::asFieldIndex() const noexcept {
	const int16_t x = 7 - m_value % 8;
	const int16_t y = m_value / 8;
	return FieldIndex{ x, y };
}

inline constexpr Bitboard Square::asBitboard() const noexcept {
	return Bitboard(1ull << m_value);
}

inline constexpr Square::operator uint64_t() const {
	return m_value;
}

inline constexpr Square::Square() :
		m_value(64) {
}

inline constexpr std::optional<Square> Square::parse(std::string_view square) {
	if (unlikely(square.size() != 2)) {
		return {};
	}

	if (unlikely(square[0] < 'a' || 'h' < square[0])) {
		return {};
	}

	if (unlikely(square[1] < '1' || '8' < square[1])) {
		return {};
	}

	return Square(square);
}

inline constexpr Square::Square(uint64_t value) :
		m_value{ static_cast<int8_t>(value) } {
}

inline constexpr Square &Square::operator=(uint64_t value) {
	m_value = static_cast<int8_t>(value);
	return *this;
}

inline constexpr Square::Square(const FieldIndex &fieldIndex) :
		Square(7 - fieldIndex.x + fieldIndex.y * 8) {
}

inline Square::Square(const Bitboard &bits) :
		Square(bits.fastBitScan()) {
}

inline constexpr Square Square::BEGIN(0);
inline constexpr Square Square::H1(0);
inline constexpr Square Square::G1(1);
inline constexpr Square Square::F1(2);
inline constexpr Square Square::E1(3);
inline constexpr Square Square::D1(4);
inline constexpr Square Square::C1(5);
inline constexpr Square Square::B1(6);
inline constexpr Square Square::A1(7);
inline constexpr Square Square::H2(8);
inline constexpr Square Square::G2(9);
inline constexpr Square Square::F2(10);
inline constexpr Square Square::E2(11);
inline constexpr Square Square::D2(12);
inline constexpr Square Square::C2(13);
inline constexpr Square Square::B2(14);
inline constexpr Square Square::A2(15);
inline constexpr Square Square::H3(16);
inline constexpr Square Square::G3(17);
inline constexpr Square Square::F3(18);
inline constexpr Square Square::E3(19);
inline constexpr Square Square::D3(20);
inline constexpr Square Square::C3(21);
inline constexpr Square Square::B3(22);
inline constexpr Square Square::A3(23);
inline constexpr Square Square::H4(24);
inline constexpr Square Square::G4(25);
inline constexpr Square Square::F4(26);
inline constexpr Square Square::E4(27);
inline constexpr Square Square::D4(28);
inline constexpr Square Square::C4(29);
inline constexpr Square Square::B4(30);
inline constexpr Square Square::A4(31);
inline constexpr Square Square::H5(32);
inline constexpr Square Square::G5(33);
inline constexpr Square Square::F5(34);
inline constexpr Square Square::E5(35);
inline constexpr Square Square::D5(36);
inline constexpr Square Square::C5(37);
inline constexpr Square Square::B5(38);
inline constexpr Square Square::A5(39);
inline constexpr Square Square::H6(40);
inline constexpr Square Square::G6(41);
inline constexpr Square Square::F6(42);
inline constexpr Square Square::E6(43);
inline constexpr Square Square::D6(44);
inline constexpr Square Square::C6(45);
inline constexpr Square Square::B6(46);
inline constexpr Square Square::A6(47);
inline constexpr Square Square::H7(48);
inline constexpr Square Square::G7(49);
inline constexpr Square Square::F7(50);
inline constexpr Square Square::E7(51);
inline constexpr Square Square::D7(52);
inline constexpr Square Square::C7(53);
inline constexpr Square Square::B7(54);
inline constexpr Square Square::A7(55);
inline constexpr Square Square::H8(56);
inline constexpr Square Square::G8(57);
inline constexpr Square Square::F8(58);
inline constexpr Square Square::E8(59);
inline constexpr Square Square::D8(60);
inline constexpr Square Square::C8(61);
inline constexpr Square Square::B8(62);
inline constexpr Square Square::A8(63);
inline constexpr Square Square::INVALID(64);

inline constexpr Square::Square(std::string_view square) :
		Square(FieldIndex(square)) {
}

inline constexpr bool Square::isValid() const noexcept {
	return 0 <= m_value && m_value < Square::INVALID.m_value;
}

inline constexpr Square Square::forward(PieceColor color, int8_t scalar) const noexcept {
	return color == PieceColor::WHITE ? north(scalar) : south(scalar);
}

inline constexpr Square Square::north(int8_t scalar) const noexcept {
	return Square(m_value + 8 * scalar);
}

inline constexpr Square Square::east(int8_t scalar) const noexcept {
	return Square(m_value - scalar);
}

inline constexpr Square Square::south(int8_t scalar) const noexcept {
	return Square(m_value - 8 * scalar);
}

inline constexpr Square Square::west(int8_t scalar) const noexcept {
	return Square(m_value + scalar);
}

inline constexpr Square Square::flipped() const noexcept {
	return Square(63 - m_value);
}

inline constexpr Square Square::middle(Square that) const noexcept {
	return Square((m_value + that.m_value) / 2);
}

inline constexpr bool Square::operator!=(Square piece) const {
	return m_value != piece.m_value;
}

inline constexpr Square Square::operator++() {
	return Square(++m_value);
}

inline std::ostream &operator<<(std::ostream &os, const Square &square) {
	constexpr char fileLabels[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
	const FieldIndex field = square.asFieldIndex();

	if (unlikely(field.x >= 0 && field.x < 8 && field.y >= 0 && field.y < 8)) {
		os << fileLabels[field.x] << (field.y + 1);
	} else {
		os.setstate(std::ios_base::failbit);
	}

	return os;
}

} //namespace phase4::engine::common

#endif
