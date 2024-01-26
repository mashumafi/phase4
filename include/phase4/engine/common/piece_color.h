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
	[[nodiscard]] constexpr inline PieceColor invert() const noexcept;

	constexpr PieceColor(PieceColor const &that) = default;
	constexpr PieceColor &operator=(const PieceColor &that) = default;

	constexpr PieceColor(PieceColor &&that) noexcept = default;
	constexpr PieceColor &operator=(PieceColor &&that) noexcept = default;

	constexpr bool operator==(const PieceColor &) const;

	friend std::ostream &operator<<(std::ostream &os, PieceColor color);

private:
	constexpr PieceColor(uint64_t value);
	constexpr PieceColor &operator=(uint64_t value);

	uint8_t m_value;
};

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

constexpr bool PieceColor::operator==(const PieceColor &color) const {
	return color.m_value == m_value;
}

[[nodiscard]] constexpr inline PieceColor PieceColor::invert() const noexcept {
	if (*this == WHITE) {
		return BLACK;
	} else {
		return WHITE;
	}
}

inline std::ostream &operator<<(std::ostream &os, PieceColor color) {
	switch (color.m_value) {
		case PieceColor::WHITE.m_value:
			return os << "WHITE";
		case PieceColor::BLACK.m_value:
			return os << "BLACK";
		default:
			os.setstate(std::ios_base::failbit);
			return os;
	}
}

} //namespace phase4::engine::common

#endif
