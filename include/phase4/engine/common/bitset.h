#ifndef PHASE4_ENGINE_COMMON_BITSET_H
#define PHASE4_ENGINE_COMMON_BITSET_H

#include <bitset>
#include <cstdint>

namespace phase4::engine::common {

class Bitset {
public:
	constexpr Bitset(size_t bits) noexcept;

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
	[[nodiscard]] inline constexpr uint8_t count() const noexcept;

	/// @brief counts the number of zeros to the right
	/// @param bits
	/// @return
	[[nodiscard]] inline constexpr uint8_t bitScan() const noexcept;

	inline constexpr bool operator==(Bitset bits) const noexcept;
	inline constexpr bool operator==(uint64_t bits) const noexcept;

	inline constexpr bool operator<(Bitset bits) const noexcept;
	inline constexpr bool operator<(uint64_t bits) const noexcept;

	inline constexpr bool operator>(Bitset bits) const noexcept;
	inline constexpr bool operator>(uint64_t bits) const noexcept;

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

	size_t m_bits;
};

inline constexpr Bitset::Bitset(size_t bits) noexcept :
		m_bits(bits) {
}

[[nodiscard]] inline constexpr Bitset Bitset::getLsb() const noexcept {
	return (m_bits & -m_bits);
}

[[nodiscard]] inline constexpr Bitset Bitset::popLsb() const noexcept {
	return (m_bits & (m_bits - 1));
}

[[nodiscard]] inline constexpr uint8_t Bitset::count() const noexcept {
	uint64_t count = 0;
	Bitset bits = m_bits;
	while (bits > 0) {
		bits = bits.popLsb();
		count++;
	}
	return count;
}

[[nodiscard]] inline constexpr uint8_t Bitset::bitScan() const noexcept {
	const int64_t ibits = static_cast<int64_t>(m_bits);
	return g_bitScanValues[(static_cast<uint64_t>((ibits & -ibits) * 0x03f79d71b4cb0a89)) >> 58];
}

inline constexpr bool Bitset::operator==(uint64_t bits) const noexcept {
	return m_bits == bits;
}

inline constexpr bool Bitset::operator==(Bitset bits) const noexcept {
	return m_bits == bits.m_bits;
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

std::ostream &operator<<(std::ostream &os, const Bitset bits) {
	return os << std::bitset<64>(bits.m_bits);
}

} //namespace phase4::engine::common

#endif
