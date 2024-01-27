#ifndef PHASE4_ENGINE_COMMON_SQUARE_H
#define PHASE4_ENGINE_COMMON_SQUARE_H

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/util.h>

#include <iostream>

namespace phase4::engine::common {

class Square {
public:
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
	[[nodiscard]] constexpr uint8_t get_raw_value() const;

	/// @brief converts the square index to a point
	/// @return the point
	constexpr FieldIndex asFieldIndex() const;

	inline constexpr Bitset asBitboard() const;

	constexpr operator uint64_t() const;

	constexpr Square();

	constexpr explicit Square(uint64_t value);
	constexpr Square &operator=(uint64_t value);

	explicit constexpr Square(const FieldIndex &fieldIndex);

	constexpr Square(Square const &that) = default;
	constexpr Square &operator=(const Square &that) = default;

	constexpr Square(Square &&that) noexcept = default;
	constexpr Square &operator=(Square &&that) noexcept = default;

	explicit constexpr Square(std::string_view move);

	constexpr bool isValid() const;

	inline constexpr bool operator!=(Square piece) const;
	inline constexpr Square operator++();

	friend std::ostream &operator<<(std::ostream &os, const Square &square);

private:
	uint8_t m_value;
};

[[nodiscard]] constexpr uint8_t Square::get_raw_value() const {
	return m_value;
}

constexpr FieldIndex Square::asFieldIndex() const {
	const int16_t x = 7 - m_value % 8;
	const int16_t y = m_value / 8;
	return FieldIndex{ x, y };
}

inline constexpr Bitset Square::asBitboard() const {
	return Bitset(1ull << m_value);
}

constexpr Square::operator uint64_t() const {
	return m_value;
}

constexpr Square::Square() :
		m_value(64) {
}

constexpr Square::Square(uint64_t value) :
		m_value{ static_cast<uint8_t>(value) } {
}

constexpr Square &Square::operator=(uint64_t value) {
	m_value = static_cast<uint8_t>(value);
	return *this;
}

constexpr Square::Square(const FieldIndex &fieldIndex) :
		Square(7 - fieldIndex.x + fieldIndex.y * 8) {
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

constexpr Square::Square(std::string_view square) :
		Square(FieldIndex(square)) {
}

constexpr bool Square::isValid() const {
	return m_value < Square::INVALID.m_value;
}

inline constexpr bool Square::operator!=(Square piece) const {
	return m_value != piece.m_value;
}

inline constexpr Square Square::operator++() {
	return Square(++m_value);
}

inline std::ostream &operator<<(std::ostream &os, const Square &square) {
	constexpr char fileLabels[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
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
