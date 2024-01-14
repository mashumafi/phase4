#ifndef PHASE4_ENGINE_COMMON_PIECE_COLOR_H
#define PHASE4_ENGINE_COMMON_PIECE_COLOR_H

#include <cstdint>
#include <iostream>

namespace phase4::engine::common {

class PieceColor {
public:
	static const PieceColor WHITE;
	static const PieceColor BLACK;

	/// @brief gets the raw internal value
	/// @return the raw internal value
	[[nodiscard]] constexpr uint8_t get_raw_value() const;

	/// @brief inverts the color
	/// @return the inverted color
	[[nodiscard]] constexpr inline PieceColor invert() const;

	constexpr PieceColor(PieceColor const &that);
	constexpr PieceColor &operator=(const PieceColor &that);

	constexpr PieceColor(PieceColor &&that) noexcept;
	constexpr PieceColor &operator=(PieceColor &&that) noexcept;

	constexpr bool operator==(const PieceColor &) const;

	friend std::ostream &operator<<(std::ostream &os, const PieceColor &color);

private:
	constexpr PieceColor(uint64_t value);
	constexpr PieceColor &operator=(uint64_t value);

	uint8_t m_value;
};

[[nodiscard]] constexpr inline PieceColor PieceColor::invert() const {
	if ((*this) == WHITE) {
		return BLACK;
	} else {
		return WHITE;
	}
}

constexpr PieceColor::PieceColor(uint64_t value) :
		m_value{ static_cast<uint8_t>(value) } {
}

constexpr PieceColor &PieceColor::operator=(uint64_t value) {
	m_value = static_cast<uint8_t>(value);
	return *this;
}

inline constexpr PieceColor PieceColor::WHITE = 0;
inline constexpr PieceColor PieceColor::BLACK = 1;

[[nodiscard]] constexpr uint8_t PieceColor::get_raw_value() const {
	return m_value;
}

constexpr PieceColor::PieceColor(const PieceColor &that) :
		m_value{ that.m_value } {
}

constexpr PieceColor &PieceColor::operator=(const PieceColor &that) {
	m_value = that.m_value;
	return *this;
}

constexpr PieceColor::PieceColor(PieceColor &&that) noexcept :
		m_value{ std::move(that).m_value } {
}

constexpr PieceColor &PieceColor::operator=(PieceColor &&that) noexcept {
	m_value = std::move(that).m_value;
	return *this;
}

constexpr bool PieceColor::operator==(const PieceColor &color) const {
	return color.m_value == m_value;
}

std::ostream &operator<<(std::ostream &os, const PieceColor &color) {
	switch (color.m_value) {
		case PieceColor::WHITE.m_value:
			os << "WHITE";
			break;
		case PieceColor::BLACK.m_value:
			os << "BLACK";
			break;
		default:
			os.setstate(std::ios_base::failbit);
	}
	return os;
}

} //namespace phase4::engine::common

#endif
