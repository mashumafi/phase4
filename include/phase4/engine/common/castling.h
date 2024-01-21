#ifndef PHASE4_ENGINE_COMMON_CASTLING_H
#define PHASE4_ENGINE_COMMON_CASTLING_H

#include <phase4/engine/common/util.h>

#include <cstdint>
#include <iostream>

namespace phase4::engine::common {

class Castling {
public:
	static const Castling NONE;
	static const Castling WHITE_SHORT;
	static const Castling WHITE_LONG;
	static const Castling BLACK_SHORT;
	static const Castling BLACK_LONG;
	static const Castling WHITE_CASTLING;
	static const Castling BLACK_CASTLING;
	static const Castling EVERYTHING;

	/// @brief gets the raw internal value
	/// @return the raw internal value
	[[nodiscard]] constexpr uint8_t get_raw_value() const;

	constexpr Castling();

	constexpr Castling(Castling const &that);
	constexpr Castling &operator=(const Castling &that);

	constexpr Castling(Castling &&that) noexcept;
	constexpr Castling &operator=(Castling &&that) noexcept;

	constexpr Castling operator|(Castling that) const;

	constexpr Castling operator&(Castling that) const;

	constexpr Castling operator~() const;

	constexpr bool operator==(Castling that) const;

	constexpr bool operator!=(Castling that) const;

	constexpr Castling operator&=(Castling that);

	friend std::ostream &operator<<(std::ostream &os, Castling castling);

private:
	constexpr Castling(uint64_t value);
	constexpr Castling &operator=(uint64_t value);

	uint8_t m_value;
};

constexpr Castling::Castling(uint64_t value) :
		m_value{ static_cast<uint8_t>(value) } {
}

constexpr Castling &Castling::operator=(uint64_t value) {
	m_value = static_cast<uint8_t>(value);
	return *this;
}

constexpr Castling::Castling() :
		Castling(0) {
}

constexpr Castling Castling::operator|(Castling that) const {
	return Castling(m_value | that.m_value);
}

constexpr Castling Castling::operator&(Castling that) const {
	return Castling(m_value & that.m_value);
}

constexpr Castling Castling::operator~() const {
	return Castling(~m_value);
}

constexpr Castling::Castling(const Castling &that) :
		m_value{ that.m_value } {
}

constexpr Castling &Castling::operator=(const Castling &that) {
	m_value = that.m_value;
	return *this;
}

constexpr Castling::Castling(Castling &&that) noexcept :
		m_value{ std::move(that).m_value } {
}

constexpr Castling &Castling::operator=(Castling &&that) noexcept {
	m_value = std::move(that).m_value;
	return *this;
}

inline constexpr Castling Castling::NONE;
inline constexpr Castling Castling::WHITE_SHORT = 1;
inline constexpr Castling Castling::WHITE_LONG = 2;
inline constexpr Castling Castling::BLACK_SHORT = 4;
inline constexpr Castling Castling::BLACK_LONG = 8;
inline constexpr Castling Castling::WHITE_CASTLING = Castling::WHITE_SHORT | Castling::WHITE_LONG;
inline constexpr Castling Castling::BLACK_CASTLING = Castling::BLACK_SHORT | Castling::BLACK_LONG;
inline constexpr Castling Castling::EVERYTHING = Castling::WHITE_CASTLING | Castling::BLACK_CASTLING;

[[nodiscard]] constexpr uint8_t Castling::get_raw_value() const {
	return m_value;
}

constexpr bool Castling::operator==(Castling that) const {
	return m_value == that.m_value;
}

constexpr bool Castling::operator!=(Castling that) const {
	return m_value != that.m_value;
}

constexpr Castling Castling::operator&=(Castling that) {
	return m_value &= that.m_value;
}

inline std::ostream &operator<<(std::ostream &os, Castling castling) {
	switch (castling.get_raw_value()) {
		case Castling::NONE.get_raw_value():
			return os << "NONE";
		case Castling::WHITE_SHORT.get_raw_value():
			return os << "WHITE_SHORT";
		case Castling::WHITE_LONG.get_raw_value():
			return os << "WHITE_LONG";
		case Castling::BLACK_SHORT.get_raw_value():
			return os << "BLACK_SHORT";
		case Castling::BLACK_LONG.get_raw_value():
			return os << "BLACK_LONG";
		case Castling::WHITE_CASTLING.get_raw_value():
			return os << "WHITE_CASTLING";
		case Castling::BLACK_CASTLING.get_raw_value():
			return os << "BLACK_CASTLING";
		case Castling::EVERYTHING.get_raw_value():
			return os << "EVERYTHING";
		default:
			if (unlikely(castling.get_raw_value() > Castling::EVERYTHING.get_raw_value())) {
				os.setstate(std::ios_base::failbit);
			} else {
				bool isFirstFlag = true;

				for (uint8_t flag = 1; flag <= Castling::EVERYTHING.get_raw_value(); flag <<= 1) {
					if (castling.get_raw_value() & flag) {
						if (!isFirstFlag) {
							os << " | ";
						}
						os << Castling(flag);
						isFirstFlag = false;
					}
				}
			}
			return os;
	}
}

} //namespace phase4::engine::common

#endif
