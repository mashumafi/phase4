#ifndef PHASE4_ENGINE_COMMON_BITBOARD_H
#define PHASE4_ENGINE_COMMON_BITBOARD_H

#include <phase4/engine/common/util.h>

#include <array>
#include <bitset>
#include <cstdint>
#include <iostream>

#ifdef USE_SLOW_BITSET
#define USE_SLOW_BITSET_COUNT
#define USE_SLOW_BITSET_LSB
#endif

namespace phase4::engine::common {

class Bitboard {
public:
	static const Bitboard MAX;

	inline constexpr Bitboard() noexcept;
	inline constexpr Bitboard(uint64_t bitset) noexcept;

	inline constexpr Bitboard(const Bitboard &that) noexcept = default;
	inline constexpr Bitboard &operator=(const Bitboard &that) = default;

	inline constexpr Bitboard(Bitboard &&that) noexcept = default;
	inline constexpr Bitboard &operator=(Bitboard &&that) noexcept = default;

	/// @brief turns off all bitset except the least significant bit
	/// @param bitset to keep the LSB
	/// @return
	[[nodiscard]] inline constexpr Bitboard getLsb() const noexcept;

	/// @brief turns off the least significant bit
	/// @param bitset to turn off LSB
	/// @return
	[[nodiscard]] inline constexpr Bitboard popLsb() const noexcept;

	/// @brief counts the number of 1 bitset
	/// @param bitset to count
	/// @return
	[[nodiscard]] inline uint8_t fastCount() const noexcept;

	/// @brief counts the number of 1 bitset
	/// @param bitset to count
	/// @return
	[[nodiscard]] inline constexpr uint8_t count() const noexcept;

	/// @brief counts the number of zeros to the right
	/// @param bitset
	/// @return
	[[nodiscard]] inline uint8_t fastBitScan() const noexcept;

	/// @brief counts the number of zeros to the right
	/// @param bitset
	/// @return
	[[nodiscard]] inline constexpr uint8_t bitScan() const noexcept;

	inline constexpr uint64_t get_raw_value() const;

	inline constexpr bool operator==(Bitboard bitset) const noexcept;
	inline constexpr bool operator==(uint64_t bitset) const noexcept;

	inline constexpr bool operator!=(Bitboard bitset) const noexcept;
	inline constexpr bool operator!=(uint64_t bitset) const noexcept;

	inline constexpr bool operator<(Bitboard bitset) const noexcept;
	inline constexpr bool operator<(uint64_t bitset) const noexcept;

	inline constexpr bool operator>(Bitboard bitset) const noexcept;
	inline constexpr bool operator>(uint64_t bitset) const noexcept;

	inline constexpr Bitboard operator|(Bitboard bitset) const noexcept;
	inline constexpr Bitboard operator|(uint64_t bitset) const noexcept;

	inline constexpr Bitboard operator|=(Bitboard bitset) noexcept;
	inline constexpr Bitboard operator|=(uint64_t bitset) noexcept;

	inline constexpr Bitboard operator&(Bitboard bitset) const noexcept;
	inline constexpr Bitboard operator&(uint64_t bitset) const noexcept;

	inline constexpr Bitboard operator&=(Bitboard bitset) noexcept;
	inline constexpr Bitboard operator&=(uint64_t bitset) noexcept;

	inline constexpr Bitboard operator*(Bitboard bitset) const noexcept;
	inline constexpr Bitboard operator*(uint64_t bitset) const noexcept;

	inline constexpr Bitboard operator>>(Bitboard bitset) const noexcept;
	inline constexpr Bitboard operator>>(uint64_t bitset) const noexcept;

	inline constexpr Bitboard operator>>=(Bitboard bitset) noexcept;
	inline constexpr Bitboard operator>>=(uint64_t bitset) noexcept;

	inline constexpr Bitboard operator~() const noexcept;

	inline constexpr Bitboard operator^(Bitboard bitset) const noexcept;
	inline constexpr Bitboard operator^(uint64_t bitset) const noexcept;

	inline constexpr Bitboard operator^=(Bitboard bitset) noexcept;
	inline constexpr Bitboard operator^=(uint64_t bitset) noexcept;

	inline constexpr Bitboard operator<<(Bitboard bitset) const noexcept;
	inline constexpr Bitboard operator<<(uint64_t bitset) const noexcept;

	inline constexpr Bitboard operator<<=(Bitboard bitset) noexcept;
	inline constexpr Bitboard operator<<=(uint64_t bitset) noexcept;

	friend std::ostream &operator<<(std::ostream &os, const Bitboard bitset);

private:
#ifdef USE_SLOW_BITSET_LSB
	static constexpr int32_t g_bitScanValues[] = {
		0, 1, 48, 2, 57, 49, 28, 3,
		61, 58, 50, 42, 38, 29, 17, 4,
		62, 55, 59, 36, 53, 51, 43, 22,
		45, 39, 33, 30, 24, 18, 12, 5,
		63, 47, 56, 27, 60, 41, 37, 16,
		54, 35, 52, 21, 44, 32, 23, 11,
		46, 26, 40, 15, 34, 20, 31, 10,
		25, 14, 19, 9, 13, 8, 7, 6
	};
#endif

#ifdef USE_SLOW_BITSET_COUNT
	static const std::array<uint8_t, 1 << 16> g_popCount;

	static constexpr std::array<uint8_t, 1 << 16> populateBitCounts();
#endif

	uint64_t m_bitset;
};

inline constexpr Bitboard::Bitboard(uint64_t bitset) noexcept :
		m_bitset(bitset) {
}

inline constexpr Bitboard::Bitboard() noexcept :
		Bitboard(0) {
}

inline constexpr Bitboard Bitboard::MAX(0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111);

[[nodiscard]] inline constexpr Bitboard Bitboard::getLsb() const noexcept {
	assert(m_bitset != 0);
	return m_bitset & (~m_bitset + 1);
}

[[nodiscard]] inline constexpr Bitboard Bitboard::popLsb() const noexcept {
	assert(m_bitset != 0);
	return m_bitset & (m_bitset - 1);
}

[[nodiscard]] inline uint8_t Bitboard::fastCount() const noexcept {
#if defined(USE_SLOW_BITSET_COUNT)
	union {
		uint64_t bb;
		uint16_t u[4];
	} v = { m_bitset };
	return g_popCount[v.u[0]] + g_popCount[v.u[1]] + g_popCount[v.u[2]] + g_popCount[v.u[3]];
#elif defined(__GNUC__) || defined(__clang__)
	// GCC or Clang
	return __builtin_popcountll(m_bitset);
#elif defined(_MSC_VER)
#ifdef _WIN64
	return static_cast<uint8_t>(__popcnt64(m_bitset));
#else
	return static_cast<uint8_t>(__popcnt(static_cast<uint32_t>(m_bitset)) + __popcnt(static_cast<uint32_t>(m_bitset >> 32)));
#endif
#else
	static_assert(false, "Define USE_SLOW_BITSET_COUNT to use internal implementation.")
#endif
}

[[nodiscard]] inline constexpr uint8_t Bitboard::count() const noexcept {
#if defined(__GNUC__) || defined(__clang__)
	// GCC or Clang
	return static_cast<uint8_t>(__builtin_popcountll(m_bitset));
#elif defined(_MSC_VER) && 0 // Skip intrinsic for Microsoft Visual C++ due to not being constexpr
#else
	// Fallback implementation for other compilers or platforms
	uint8_t count = 0;
	Bitboard bitset = m_bitset;
	while (bitset > 0) {
		bitset = bitset.popLsb();
		count++;
	}
	return count;
#endif
}

#ifdef USE_SLOW_BITSET_COUNT
constexpr std::array<uint8_t, 1 << 16> Bitboard::populateBitCounts() {
	std::array<uint8_t, 1 << 16> bitCounts{};
	for (size_t i = 0; i < bitCounts.size(); ++i) {
		bitCounts[i] = Bitboard(i).count();
	}
	return bitCounts;
}

inline constexpr std::array<uint8_t, 1 << 16> Bitboard::g_popCount = populateBitCounts();
#endif

[[nodiscard]] inline uint8_t Bitboard::fastBitScan() const noexcept {
	assert(m_bitset != 0);
#if defined(USE_SLOW_BITSET_LSB)
	const int64_t ibits = static_cast<int64_t>(m_bitset);
	return g_bitScanValues[(static_cast<uint64_t>((ibits & -ibits) * 0x03f79d71b4cb0a89)) >> 58];
#elif defined(__GNUC__) || defined(__clang__)
	// GCC or Clang
	return __builtin_ctzll(m_bitset);
#elif _MSC_VER
	// Microsoft Visual C++
	unsigned long idx;
#ifdef _WIN64
	_BitScanForward64(&idx, m_bitset);
	return static_cast<uint8_t>(idx);

#else // WIN32
	if (m_bitset & 0xffffffff) {
		_BitScanForward(&idx, static_cast<uint32_t>(m_bitset));
		return static_cast<uint8_t>(idx);
	} else {
		_BitScanForward(&idx, static_cast<uint32_t>(m_bitset >> 32));
		return static_cast<uint8_t>(idx) + 32;
	}
#endif
#else
	static_assert(false, "Define USE_SLOW_BITSET_LSB to use internal implementation.")
#endif
}

[[nodiscard]] inline constexpr uint8_t Bitboard::bitScan() const noexcept {
#if defined(__GNUC__) || defined(__clang__)
	// GCC or Clang
	return __builtin_ctzll(m_bitset);
#elif _MSC_VER && 0 // Skip intrinsic for Microsoft Visual C++ due to not being constexpr
#else
	// This is slow but guarantees a constexpr
	uint8_t index = 0;
	uint64_t bitset = m_bitset;
	while (bitset > 0) {
		if (bitset & 1)
			break;

		bitset >>= 1;
		index++;
	}
	return index;
#endif
}

inline constexpr uint64_t Bitboard::get_raw_value() const {
	return m_bitset;
}

inline constexpr bool Bitboard::operator==(uint64_t bitset) const noexcept {
	return m_bitset == bitset;
}

inline constexpr bool Bitboard::operator==(Bitboard bitset) const noexcept {
	return m_bitset == bitset.m_bitset;
}

inline constexpr bool Bitboard::operator!=(uint64_t bitset) const noexcept {
	return m_bitset != bitset;
}

inline constexpr bool Bitboard::operator!=(Bitboard bitset) const noexcept {
	return m_bitset != bitset.m_bitset;
}

inline constexpr bool Bitboard::operator<(uint64_t bitset) const noexcept {
	return m_bitset < bitset;
}

inline constexpr bool Bitboard::operator<(Bitboard bitset) const noexcept {
	return m_bitset < bitset.m_bitset;
}

inline constexpr bool Bitboard::operator>(uint64_t bitset) const noexcept {
	return m_bitset > bitset;
}

inline constexpr bool Bitboard::operator>(Bitboard bitset) const noexcept {
	return m_bitset > bitset.m_bitset;
}

inline constexpr Bitboard Bitboard::operator|(Bitboard bitset) const noexcept {
	return Bitboard(m_bitset | bitset.m_bitset);
}

inline constexpr Bitboard Bitboard::operator|(uint64_t bitset) const noexcept {
	return Bitboard(m_bitset | bitset);
}

inline constexpr Bitboard Bitboard::operator|=(Bitboard bitset) noexcept {
	m_bitset |= bitset.m_bitset;
	return *this;
}

inline constexpr Bitboard Bitboard::operator|=(uint64_t bitset) noexcept {
	m_bitset |= bitset;
	return *this;
}

inline constexpr Bitboard Bitboard::operator&(Bitboard bitset) const noexcept {
	return Bitboard(m_bitset & bitset.m_bitset);
}

inline constexpr Bitboard Bitboard::operator&(uint64_t bitset) const noexcept {
	return Bitboard(m_bitset & bitset);
}

inline constexpr Bitboard Bitboard::operator&=(Bitboard bitset) noexcept {
	m_bitset &= bitset.m_bitset;
	return *this;
}

inline constexpr Bitboard Bitboard::operator&=(uint64_t bitset) noexcept {
	m_bitset &= bitset;
	return *this;
}

inline constexpr Bitboard Bitboard::operator*(Bitboard bitset) const noexcept {
	return Bitboard(m_bitset * bitset.m_bitset);
}

inline constexpr Bitboard Bitboard::operator*(uint64_t bitset) const noexcept {
	return Bitboard(m_bitset * bitset);
}

inline constexpr Bitboard Bitboard::operator>>(Bitboard bitset) const noexcept {
	return Bitboard(m_bitset >> bitset.m_bitset);
}

inline constexpr Bitboard Bitboard::operator>>(uint64_t bitset) const noexcept {
	return Bitboard(m_bitset >> bitset);
}

inline constexpr Bitboard Bitboard::operator>>=(Bitboard bitset) noexcept {
	m_bitset >>= bitset.m_bitset;
	return *this;
}

inline constexpr Bitboard Bitboard::operator>>=(uint64_t bitset) noexcept {
	m_bitset >>= bitset;
	return *this;
}

inline constexpr Bitboard Bitboard::operator~() const noexcept {
	return Bitboard(~m_bitset);
}

inline constexpr Bitboard Bitboard::operator^(Bitboard bitset) const noexcept {
	return Bitboard(m_bitset ^ bitset.m_bitset);
}

inline constexpr Bitboard Bitboard::operator^(uint64_t bitset) const noexcept {
	return Bitboard(m_bitset ^ bitset);
}

inline constexpr Bitboard Bitboard::operator^=(Bitboard bitset) noexcept {
	m_bitset ^= bitset.m_bitset;
	return *this;
}

inline constexpr Bitboard Bitboard::operator^=(uint64_t bitset) noexcept {
	m_bitset ^= bitset;
	return *this;
}

inline constexpr Bitboard Bitboard::operator<<(Bitboard bitset) const noexcept {
	return Bitboard(m_bitset << bitset.m_bitset);
}

inline constexpr Bitboard Bitboard::operator<<(uint64_t bitset) const noexcept {
	return Bitboard(m_bitset << bitset);
}

inline constexpr Bitboard Bitboard::operator<<=(Bitboard bitset) noexcept {
	m_bitset <<= bitset.m_bitset;
	return *this;
}

inline constexpr Bitboard Bitboard::operator<<=(uint64_t bitset) noexcept {
	m_bitset <<= bitset;
	return *this;
}

inline std::ostream &operator<<(std::ostream &os, const Bitboard bitset) {
	return os << std::bitset<64>(bitset.m_bitset);
}

} //namespace phase4::engine::common

#endif
