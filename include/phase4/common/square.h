#ifndef PHASE4_COMMON_SQUARE_H
#define PHASE4_COMMON_SQUARE_H

#include <phase4/common/point.h>

#include <iostream>

namespace phase4::common {

class Square {
public:
	static const Square A1;
	static const Square A2;
	static const Square A3;
	static const Square A4;
	static const Square A5;
	static const Square A6;
	static const Square A7;
	static const Square A8;

	static const Square B1;
	static const Square B2;
	static const Square B3;
	static const Square B4;
	static const Square B5;
	static const Square B6;
	static const Square B7;
	static const Square B8;

	static const Square C1;
	static const Square C2;
	static const Square C3;
	static const Square C4;
	static const Square C5;
	static const Square C6;
	static const Square C7;
	static const Square C8;

	static const Square D1;
	static const Square D2;
	static const Square D3;
	static const Square D4;
	static const Square D5;
	static const Square D6;
	static const Square D7;
	static const Square D8;

	static const Square E1;
	static const Square E2;
	static const Square E3;
	static const Square E4;
	static const Square E5;
	static const Square E6;
	static const Square E7;
	static const Square E8;

	static const Square F1;
	static const Square F2;
	static const Square F3;
	static const Square F4;
	static const Square F5;
	static const Square F6;
	static const Square F7;
	static const Square F8;

	static const Square G1;
	static const Square G2;
	static const Square G3;
	static const Square G4;
	static const Square G5;
	static const Square G6;
	static const Square G7;
	static const Square G8;

	static const Square H1;
	static const Square H2;
	static const Square H3;
	static const Square H4;
	static const Square H5;
	static const Square H6;
	static const Square H7;
	static const Square H8;

	/// @brief gets the raw internal value
	/// @return the raw internal value
	[[nodiscard]] constexpr uint8_t get_raw_value() const;

	/// @brief converts the square index to a point
	/// @return the point
	constexpr Point asPoint() const;

	constexpr operator uint64_t() const;

	constexpr Square(uint64_t value);
	constexpr Square &operator=(uint64_t value);

	constexpr Square(Square const &that);
	constexpr Square &operator=(const Square &that);

	constexpr Square(Square &&that) noexcept;
	constexpr Square &operator=(Square &&that) noexcept;

	friend std::ostream &operator<<(std::ostream &os, const Square &square);

private:
	uint8_t m_value;
};

[[nodiscard]] constexpr uint8_t Square::get_raw_value() const {
	return m_value;
}

constexpr Point Square::asPoint() const {
	const int16_t x = 7 - m_value % 8;
	const int16_t y = m_value / 8;
	return Point{ x, y };
}

constexpr Square::operator uint64_t() const {
	return m_value;
}

constexpr Square::Square(uint64_t value) :
		m_value{ static_cast<uint8_t>(value) } {
}

constexpr Square &Square::operator=(uint64_t value) {
	m_value = static_cast<uint8_t>(value);
	return *this;
}

constexpr Square Square::A1 = 0;
constexpr Square Square::A2 = 1;
constexpr Square Square::A3 = 2;
constexpr Square Square::A4 = 3;
constexpr Square Square::A5 = 4;
constexpr Square Square::A6 = 5;
constexpr Square Square::A7 = 6;
constexpr Square Square::A8 = 7;
constexpr Square Square::B1 = 8;
constexpr Square Square::B2 = 9;
constexpr Square Square::B3 = 10;
constexpr Square Square::B4 = 11;
constexpr Square Square::B5 = 12;
constexpr Square Square::B6 = 13;
constexpr Square Square::B7 = 14;
constexpr Square Square::B8 = 15;
constexpr Square Square::C1 = 16;
constexpr Square Square::C2 = 17;
constexpr Square Square::C3 = 18;
constexpr Square Square::C4 = 19;
constexpr Square Square::C5 = 20;
constexpr Square Square::C6 = 21;
constexpr Square Square::C7 = 22;
constexpr Square Square::C8 = 23;
constexpr Square Square::D1 = 24;
constexpr Square Square::D2 = 25;
constexpr Square Square::D3 = 26;
constexpr Square Square::D4 = 27;
constexpr Square Square::D5 = 28;
constexpr Square Square::D6 = 29;
constexpr Square Square::D7 = 30;
constexpr Square Square::D8 = 31;
constexpr Square Square::E1 = 32;
constexpr Square Square::E2 = 33;
constexpr Square Square::E3 = 34;
constexpr Square Square::E4 = 35;
constexpr Square Square::E5 = 36;
constexpr Square Square::E6 = 37;
constexpr Square Square::E7 = 38;
constexpr Square Square::E8 = 39;
constexpr Square Square::F1 = 40;
constexpr Square Square::F2 = 41;
constexpr Square Square::F3 = 42;
constexpr Square Square::F4 = 43;
constexpr Square Square::F5 = 44;
constexpr Square Square::F6 = 45;
constexpr Square Square::F7 = 46;
constexpr Square Square::F8 = 47;
constexpr Square Square::G1 = 48;
constexpr Square Square::G2 = 49;
constexpr Square Square::G3 = 50;
constexpr Square Square::G4 = 51;
constexpr Square Square::G5 = 52;
constexpr Square Square::G6 = 53;
constexpr Square Square::G7 = 54;
constexpr Square Square::G8 = 55;
constexpr Square Square::H1 = 56;
constexpr Square Square::H2 = 57;
constexpr Square Square::H3 = 58;
constexpr Square Square::H4 = 59;
constexpr Square Square::H5 = 60;
constexpr Square Square::H6 = 61;
constexpr Square Square::H7 = 62;
constexpr Square Square::H8 = 63;

constexpr Square::Square(const Square &that) :
		m_value{ that.m_value } {
}

constexpr Square &Square::operator=(const Square &that) {
	m_value = that.m_value;
	return *this;
}

constexpr Square::Square(Square &&that) noexcept :
		m_value{ std::move(that).m_value } {
}

constexpr Square &Square::operator=(Square &&that) noexcept {
	m_value = std::move(that).m_value;
	return *this;
}

inline std::ostream &operator<<(std::ostream &os, const Square &square) {
	constexpr char fileLabels[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
	const int16_t file = square.m_value / 8;
	const int16_t rank = square.m_value % 8;

	if (file >= 0 && file < 8 && rank >= 0 && rank < 8) {
		os << fileLabels[file] << (rank + 1);
	} else {
		os.setstate(std::ios_base::failbit);
	}

	return os;
}

} //namespace phase4::common

#endif
