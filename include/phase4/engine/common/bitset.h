#ifndef PHASE4_ENGINE_COMMON_BITSET_H
#define PHASE4_ENGINE_COMMON_BITSET_H

#include <array>
#include <bitset>
#include <cstdint>
#include <iostream>

#ifdef USE_SLOW_BITSET
#define USE_SLOW_BITSET_POP
#define USE_SLOW_BITSET_COUNT
#define USE_SLOW_BITSET_LSB
#endif

namespace phase4::engine::common {

class Bitset {
public:
	static const Bitset MAX;

	constexpr Bitset() noexcept;
	constexpr Bitset(uint64_t bits) noexcept;

	constexpr Bitset(const Bitset &that) noexcept;
	constexpr Bitset &operator=(const Bitset &that);

	constexpr Bitset(Bitset &&that) noexcept;
	constexpr Bitset &operator=(Bitset &&that) noexcept;

	/// @brief turns off all bits except the least significant bit
	/// @param bits to keep the LSB
	/// @return
	[[nodiscard]] inline constexpr Bitset getLsb() const noexcept;

	/// @brief turns off the least significant bit
	/// @param bits to turn off LSB
	/// @return
	[[nodiscard]] inline constexpr Bitset popLsb() const noexcept;

	/// @brief counts the number of 1 bits
	/// @param bits to count
	/// @return
	[[nodiscard]] inline uint8_t fastCount() const noexcept;

	/// @brief counts the number of 1 bits
	/// @param bits to count
	/// @return
	[[nodiscard]] inline constexpr uint8_t count() const noexcept;

	/// @brief counts the number of zeros to the right
	/// @param bits
	/// @return
	[[nodiscard]] inline uint8_t fastBitScan() const noexcept;

	/// @brief counts the number of zeros to the right
	/// @param bits
	/// @return
	[[nodiscard]] inline constexpr uint8_t bitScan() const noexcept;

	constexpr size_t asSize() const;

	constexpr bool operator==(Bitset bits) const noexcept;
	constexpr bool operator==(uint64_t bits) const noexcept;

	constexpr bool operator!=(Bitset bits) const noexcept;
	constexpr bool operator!=(uint64_t bits) const noexcept;

	constexpr bool operator<(Bitset bits) const noexcept;
	constexpr bool operator<(uint64_t bits) const noexcept;

	constexpr bool operator>(Bitset bits) const noexcept;
	constexpr bool operator>(uint64_t bits) const noexcept;

	constexpr Bitset operator|(Bitset bits) const noexcept;
	constexpr Bitset operator|(uint64_t bits) const noexcept;

	constexpr Bitset operator|=(Bitset bits) noexcept;
	constexpr Bitset operator|=(uint64_t bits) noexcept;

	constexpr Bitset operator&(Bitset bits) const noexcept;
	constexpr Bitset operator&(uint64_t bits) const noexcept;

	constexpr Bitset operator&=(Bitset bits) noexcept;
	constexpr Bitset operator&=(uint64_t bits) noexcept;

	constexpr Bitset operator*(Bitset bits) const noexcept;
	constexpr Bitset operator*(uint64_t bits) const noexcept;

	constexpr Bitset operator>>(Bitset bits) const noexcept;
	constexpr Bitset operator>>(uint64_t bits) const noexcept;

	constexpr Bitset operator>>=(Bitset bits) noexcept;
	constexpr Bitset operator>>=(uint64_t bits) noexcept;

	constexpr Bitset operator~() const noexcept;

	constexpr Bitset operator^(Bitset bits) const noexcept;
	constexpr Bitset operator^(uint64_t bits) const noexcept;

	constexpr Bitset operator^=(Bitset bits) noexcept;
	constexpr Bitset operator^=(uint64_t bits) noexcept;

	constexpr Bitset operator<<(Bitset bits) const noexcept;
	constexpr Bitset operator<<(uint64_t bits) const noexcept;

	constexpr Bitset operator<<=(Bitset bits) noexcept;
	constexpr Bitset operator<<=(uint64_t bits) noexcept;

	friend std::ostream &operator<<(std::ostream &os, const Bitset bits);

private:
	static constexpr int g_bitScanValues[] = {
		0, 1, 48, 2, 57, 49, 28, 3,
		61, 58, 50, 42, 38, 29, 17, 4,
		62, 55, 59, 36, 53, 51, 43, 22,
		45, 39, 33, 30, 24, 18, 12, 5,
		63, 47, 56, 27, 60, 41, 37, 16,
		54, 35, 52, 21, 44, 32, 23, 11,
		46, 26, 40, 15, 34, 20, 31, 10,
		25, 14, 19, 9, 13, 8, 7, 6
	};

	static const std::array<uint8_t, 1 << 16> g_popCount;

	uint64_t m_bits;
};

constexpr std::array<uint8_t, 1 << 16> populateBitCounts() {
	std::array<uint8_t, 1 << 16> bitCounts{};
	for (size_t i = 0; i < bitCounts.size(); ++i) {
		bitCounts[i] = i % 8;
	}
	return bitCounts;
}

inline constexpr std::array<uint8_t, 1 << 16> Bitset::g_popCount = populateBitCounts();

[[nodiscard]] inline constexpr Bitset Bitset::getLsb() const noexcept {
	return (m_bits & -m_bits);
}

inline constexpr Bitset::Bitset(uint64_t bits) noexcept :
		m_bits(bits) {
}

inline constexpr Bitset::Bitset(const Bitset &that) noexcept :
		m_bits(that.m_bits) {
}

inline constexpr Bitset::Bitset() noexcept :
		Bitset(0) {
}

inline constexpr Bitset &Bitset::operator=(const Bitset &that) {
	m_bits = that.m_bits;
	return *this;
}

inline constexpr Bitset::Bitset(Bitset &&that) noexcept :
		m_bits(std::move(that).m_bits) {
}

inline constexpr Bitset &Bitset::operator=(Bitset &&that) noexcept {
	m_bits = std::move(that).m_bits;
	return *this;
}

inline constexpr Bitset Bitset::MAX(0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111);

[[nodiscard]] inline constexpr Bitset Bitset::popLsb() const noexcept {
#if defined(USE_SLOW_BITSET_POP)
	return m_bits & (m_bits - 1);
#elif defined(__GNUC__) || defined(__clang__)
	// GCC or Clang
	return m_bits & (m_bits - 1);
#elif defined(_MSC_VER)
	// Microsoft Visual C++
	return m_bits & (m_bits - 1);
#else
	static_assert(false, "Define USE_SLOW_BITSET_POP to use internal implementation.")
#endif
}

[[nodiscard]] inline uint8_t Bitset::fastCount() const noexcept {
#if defined(USE_SLOW_BITSET_COUNT)
	uint8_t count = 0;
	Bitset bits = m_bits;
	while (bits > 0) {
		bits = bits.popLsb();
		count++;
	}
	return count;
#elif defined(__GNUC__) || defined(__clang__)
	// GCC or Clang
	return static_cast<std::size_t>(__builtin_popcountll(m_bits));
#elif defined(_MSC_VER)
	return static_cast<std::size_t>(__popcnt64(m_bits));
#else
	static_assert(false, "Define USE_SLOW_BITSET_COUNT to use internal implementation.")
#endif
}

[[nodiscard]] inline constexpr uint8_t Bitset::count() const noexcept {
#if defined(__GNUC__) || defined(__clang__)
	// GCC or Clang
	return static_cast<std::size_t>(__builtin_popcountll(m_bits));
#elif defined(_MSC_VER) && 0 // Skip intrinsic for Microsoft Visual C++ due to not being constexpr
#else
	// Fallback implementation for other compilers or platforms
	uint8_t count = 0;
	Bitset bits = m_bits;
	while (bits > 0) {
		bits = bits.popLsb();
		count++;
	}
	return count;
#endif
}

[[nodiscard]] inline uint8_t Bitset::fastBitScan() const noexcept {
#if defined(USE_SLOW_BITSET_LSB)
	const int64_t ibits = static_cast<int64_t>(m_bits);
	return g_bitScanValues[(static_cast<uint64_t>((ibits & -ibits) * 0x03f79d71b4cb0a89)) >> 58];
#elif defined(__GNUC__) || defined(__clang__)
	// GCC or Clang
	return __builtin_ctzll(m_bits);
#elif _MSC_VER
	// Microsoft Visual C++
	unsigned long index;
	_BitScanForward64(&index, m_bits); // returns if the number was zero
	return index;
#else
	static_assert(false, "Define USE_SLOW_BITSET_LSB to use internal implementation.")
#endif
}

[[nodiscard]] inline constexpr uint8_t Bitset::bitScan() const noexcept {
#if defined(__GNUC__) || defined(__clang__)
	// GCC or Clang
	return __builtin_ctzll(m_bits);
#elif _MSC_VER && 0 // Skip intrinsic for Microsoft Visual C++ due to not being constexpr
#else
	// This is slow but guarantees a constexpr
	return 1ull << count().asSize();
#endif
}

constexpr size_t Bitset::asSize() const {
	return m_bits;
}

inline constexpr bool Bitset::operator==(uint64_t bits) const noexcept {
	return m_bits == bits;
}

inline constexpr bool Bitset::operator==(Bitset bits) const noexcept {
	return m_bits == bits.m_bits;
}

inline constexpr bool Bitset::operator!=(uint64_t bits) const noexcept {
	return m_bits != bits;
}

inline constexpr bool Bitset::operator!=(Bitset bits) const noexcept {
	return m_bits != bits.m_bits;
}

inline constexpr bool Bitset::operator<(uint64_t bits) const noexcept {
	return m_bits < bits;
}

inline constexpr bool Bitset::operator<(Bitset bits) const noexcept {
	return m_bits < bits.m_bits;
}

inline constexpr bool Bitset::operator>(uint64_t bits) const noexcept {
	return m_bits > bits;
}

inline constexpr bool Bitset::operator>(Bitset bits) const noexcept {
	return m_bits > bits.m_bits;
}

inline constexpr Bitset Bitset::operator|(Bitset bits) const noexcept {
	return Bitset(m_bits | bits.m_bits);
}

inline constexpr Bitset Bitset::operator|(uint64_t bits) const noexcept {
	return Bitset(m_bits | bits);
}

inline constexpr Bitset Bitset::operator|=(Bitset bits) noexcept {
	m_bits |= bits.m_bits;
	return *this;
}

inline constexpr Bitset Bitset::operator|=(uint64_t bits) noexcept {
	m_bits |= bits;
	return *this;
}

inline constexpr Bitset Bitset::operator&(Bitset bits) const noexcept {
	return Bitset(m_bits & bits.m_bits);
}

inline constexpr Bitset Bitset::operator&(uint64_t bits) const noexcept {
	return Bitset(m_bits & bits);
}

inline constexpr Bitset Bitset::operator&=(Bitset bits) noexcept {
	m_bits &= bits.m_bits;
	return *this;
}

inline constexpr Bitset Bitset::operator&=(uint64_t bits) noexcept {
	m_bits &= bits;
	return *this;
}

constexpr Bitset Bitset::operator*(Bitset bits) const noexcept {
	return Bitset(m_bits * bits.m_bits);
}

constexpr Bitset Bitset::operator*(uint64_t bits) const noexcept {
	return Bitset(m_bits * bits);
}

constexpr Bitset Bitset::operator>>(Bitset bits) const noexcept {
	return Bitset(m_bits >> bits.m_bits);
}

constexpr Bitset Bitset::operator>>(uint64_t bits) const noexcept {
	return Bitset(m_bits >> bits);
}

inline constexpr Bitset Bitset::operator>>=(Bitset bits) noexcept {
	m_bits >>= bits.m_bits;
	return *this;
}

inline constexpr Bitset Bitset::operator>>=(uint64_t bits) noexcept {
	m_bits >>= bits;
	return *this;
}

constexpr Bitset Bitset::operator~() const noexcept {
	return Bitset(~m_bits);
}

constexpr Bitset Bitset::operator^(Bitset bits) const noexcept {
	return Bitset(m_bits ^ bits.m_bits);
}

constexpr Bitset Bitset::operator^(uint64_t bits) const noexcept {
	return Bitset(m_bits ^ bits);
}

inline constexpr Bitset Bitset::operator^=(Bitset bits) noexcept {
	m_bits ^= bits.m_bits;
	return *this;
}

inline constexpr Bitset Bitset::operator^=(uint64_t bits) noexcept {
	m_bits ^= bits;
	return *this;
}

constexpr Bitset Bitset::operator<<(Bitset bits) const noexcept {
	return Bitset(m_bits << bits.m_bits);
}

constexpr Bitset Bitset::operator<<(uint64_t bits) const noexcept {
	return Bitset(m_bits << bits);
}

inline constexpr Bitset Bitset::operator<<=(Bitset bits) noexcept {
	m_bits <<= bits.m_bits;
	return *this;
}

inline constexpr Bitset Bitset::operator<<=(uint64_t bits) noexcept {
	m_bits <<= bits;
	return *this;
}

inline std::ostream &operator<<(std::ostream &os, const Bitset bits) {
	return os << std::bitset<64>(bits.m_bits);
}

} //namespace phase4::engine::common

#endif
