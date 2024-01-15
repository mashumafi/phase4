#include <climits>
#include <cstdint>

// [Bit Twiddling Hacks](https://graphics.stanford.edu/~seander/bithacks.html)

namespace phase4::engine::common {

class Math {
public:
	static constexpr int16_t abs_int16(int16_t x);
    static constexpr int16_t min_int16(int16_t x, int16_t y);
    static constexpr int16_t max_int16(int16_t x, int16_t y);
};

constexpr int16_t Math::abs_int16(int16_t x) {
	int16_t mask = x >> (sizeof(int16_t) * CHAR_BIT - 1);
	return (x + mask) ^ mask;
}

constexpr int16_t Math::min_int16(int16_t x, int16_t y) {
    return y ^ ((x ^ y) & -(x < y));
}

constexpr int16_t Math::max_int16(int16_t x, int16_t y) {
    return x ^ ((x ^ y) & -(x < y));
}

} //namespace phase4::engine::common
