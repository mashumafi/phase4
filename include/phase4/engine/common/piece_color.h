#ifndef PHASE4_ENGINE_COMMON_PIECE_COLOR_H
#define PHASE4_ENGINE_COMMON_PIECE_COLOR_H

#include <cstdint>
#include <iostream>

namespace phase4::engine::common {

class PieceColor {
public:
	static const PieceColor WHITE;
	static const PieceColor BLACK;
	static const PieceColor INVALID;

	/// @brief gets the raw internal value
	/// @return the raw internal value
	[[nodiscard]] constexpr int8_t get_raw_value() const;

	/// @brief inverts the color
	/// @return the inverted color
	[[nodiscard]] constexpr inline PieceColor invert() const noexcept;

	constexpr PieceColor(PieceColor const &that) noexcept = default;
	constexpr PieceColor &operator=(const PieceColor &that) noexcept = default;

	constexpr PieceColor(PieceColor &&that) noexcept = default;
	constexpr PieceColor &operator=(PieceColor &&that) noexcept = default;

	inline constexpr PieceColor operator++();

	inline constexpr bool operator==(const PieceColor &) const;
	inline constexpr bool operator!=(PieceColor piece) const;

	friend std::ostream &operator<<(std::ostream &os, PieceColor color);

private:
	constexpr PieceColor(uint64_t value);
	constexpr PieceColor &operator=(uint64_t value);

	int8_t m_value;
};

constexpr PieceColor::PieceColor(uint64_t value) :
		m_value{ static_cast<int8_t>(value) } {
}

constexpr PieceColor &PieceColor::operator=(uint64_t value) {
	m_value = static_cast<int8_t>(value);
	return *this;
}

inline constexpr PieceColor PieceColor::WHITE = 0;
inline constexpr PieceColor PieceColor::BLACK = 1;
inline constexpr PieceColor PieceColor::INVALID = 2;

[[nodiscard]] constexpr int8_t PieceColor::get_raw_value() const {
	return m_value;
}

constexpr PieceColor PieceColor::operator++() {
	return PieceColor(++m_value);
}

constexpr bool PieceColor::operator==(const PieceColor &color) const {
	return color.m_value == m_value;
}

constexpr bool PieceColor::operator!=(PieceColor color) const {
	return color.m_value != m_value;
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
