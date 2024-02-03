#ifndef PHASE4_ENGINE_COMMON_MATH_H
#define PHASE4_ENGINE_COMMON_MATH_H

#include <climits>
#include <cstddef>
#include <cstdint>

// [Bit Twiddling Hacks](https://graphics.stanford.edu/~seander/bithacks.html)

namespace phase4::engine::common {

class Math {
public:
	static inline constexpr int16_t abs_int16(int16_t x);
	static inline constexpr int16_t min_int16(int16_t x, int16_t y);
	static inline constexpr int16_t max_int16(int16_t x, int16_t y);

	static inline constexpr uint16_t min_uint16(uint16_t x, uint16_t y);
	static inline constexpr uint16_t max_uint16(uint16_t x, uint16_t y);

	static inline constexpr int32_t min_int32(int32_t x, int32_t y);
	static inline constexpr int32_t max_int32(int32_t x, int32_t y);

	static inline constexpr uint32_t min_uint32(uint32_t x, uint32_t y);
	static inline constexpr uint32_t max_uint32(uint32_t x, uint32_t y);

	static inline constexpr std::size_t min_size(std::size_t x, std::size_t y);
	static inline constexpr std::size_t max_size(std::size_t x, std::size_t y);
};

inline constexpr int16_t Math::abs_int16(int16_t x) {
	const int16_t mask = x >> (sizeof(int16_t) * CHAR_BIT - 1);
	return (x + mask) ^ mask;
}

inline constexpr int16_t Math::min_int16(int16_t x, int16_t y) {
	return y ^ ((x ^ y) & -(x < y));
}

inline constexpr int16_t Math::max_int16(int16_t x, int16_t y) {
	return x ^ ((x ^ y) & -(x < y));
}

inline constexpr uint16_t Math::min_uint16(uint16_t x, uint16_t y) {
	return y ^ ((x ^ y) & -(x < y));
}

inline constexpr uint16_t Math::max_uint16(uint16_t x, uint16_t y) {
	return x ^ ((x ^ y) & -(x < y));
}

inline constexpr int32_t Math::min_int32(int32_t x, int32_t y) {
	return y ^ ((x ^ y) & -(x < y));
}

inline constexpr int32_t Math::max_int32(int32_t x, int32_t y) {
	return x ^ ((x ^ y) & -(x < y));
}

inline constexpr uint32_t Math::min_uint32(uint32_t x, uint32_t y) {
	return y ^ ((x ^ y) & -(x < y));
}

inline constexpr uint32_t Math::max_uint32(uint32_t x, uint32_t y) {
	return x ^ ((x ^ y) & -(x < y));
}

inline constexpr std::size_t Math::min_size(std::size_t x, std::size_t y) {
	return y ^ ((x ^ y) & -(x < y));
}

inline constexpr std::size_t Math::max_size(std::size_t x, std::size_t y) {
	return x ^ ((x ^ y) & -(x < y));
}

} //namespace phase4::engine::common

#endif
