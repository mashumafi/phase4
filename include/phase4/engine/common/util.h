#ifndef PHASE4_ENGINE_COMMON_UTIL_H
#define PHASE4_ENGINE_COMMON_UTIL_H

#include <array>
#include <cassert>
#include <cstring>
#include <limits>
#include <memory>

#if defined(__GNUC__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) x
#define unlikely(x) x
#endif

#ifndef NDEBUG
#define ASSERT(x) \
	do {          \
		(void)x;  \
	} while (false)
#else
#define ASSERT(x)  \
	do {           \
		assert(x); \
	} while (false)
#endif

template <size_t... Dims>
class MultiArrayIndex {
public:
	template <typename... Indices>
	static constexpr size_t computeIndex(Indices... indices) {
		static_assert(sizeof...(Dims) == sizeof...(Indices), "Number of indices must match number of dimensions.");

		size_t index = 0;
		size_t multiplier = 1;
		std::array<size_t, sizeof...(Dims)> dimensions = { Dims... };
		std::array<size_t, sizeof...(Indices)> indexValues = { static_cast<size_t>(indices)... };

		for (int i = sizeof...(Dims) - 1; i >= 0; --i) {
			if (unlikely(indexValues[i] >= dimensions[i])) {
				return 0;
			}
			index += indexValues[i] * multiplier;
			multiplier *= dimensions[i];
		}

		return index;
	}
};

template <typename T, std::size_t... Dims>
class MultiArray {
	template <typename... Sizes>
	static constexpr std::size_t computeSize(Sizes... sizes) {
		return (sizes * ...);
	}

public:
	constexpr MultiArray() :
			m_data{} {
	}

	constexpr MultiArray(const std::array<T, computeSize(Dims...)> &data) :
			m_data{ data } {
	}

	constexpr MultiArray(std::array<T, computeSize(Dims...)> &&data) :
			m_data{ std::move(data) } {
	}

	// Function to access elements in the multi-dimensional array
	template <typename... Args>
	constexpr const T &at(Args... indices) const {
		return m_data[MultiArrayIndex<Dims...>::computeIndex(indices...)];
	}

private:
	std::array<T, computeSize(Dims...)> m_data;
};

namespace phase4::engine::common::util {

template <typename T, size_t N>
void clear(std::array<T, N> &array) {
	std::memset(static_cast<void *>(array.data()), 0, array.size() * sizeof(T));
}

inline constexpr size_t back_index(std::size_t offset) {
	return std::numeric_limits<size_t>::max() - offset + 1;
}

} //namespace phase4::engine::common::util

#endif
