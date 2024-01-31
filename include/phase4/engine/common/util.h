#ifndef PHASE4_ENGINE_COMMON_UTIL_H
#define PHASE4_ENGINE_COMMON_UTIL_H

#include <array>
#include <cassert>
#include <memory>

#if defined(__GNUC__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) x
#define unlikely(x) x
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

		for (size_t i = sizeof...(Dims) - 1; i >= 0; --i) {
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

#if NDEBUG // NOTE: faster builds on debug
#define FAST_INLINE_CONSTEXPR inline constexpr
#else
#define FAST_INLINE_CONSTEXPR inline const
#endif

#if NDEBUG // NOTE: faster builds on debug
#define FAST_STATIC_CONSTEXPR static constexpr
#else
#define FAST_STATIC_CONSTEXPR static
#endif

#if NDEBUG // NOTE: faster builds on debug
#define FAST_CONSTEXPR constexpr
#else
#define FAST_CONSTEXPR inline
#endif

#endif
