#ifndef PHASE4_ENGINE_COMMON_UTIL_H
#define PHASE4_ENGINE_COMMON_UTIL_H

#include <array>
#include <cassert>
#include <memory>
#include <memory_resource>

#if defined(__GNUC__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) x
#define unlikely(x) x
#endif

template <typename T>
struct UniquePtrDeleter {
	UniquePtrDeleter(std::pmr::memory_resource *alloc) :
			allocator(alloc) {}

	void operator()(T *ptr) const {
		if (likely(ptr)) {
			ptr->~T(); // Call the destructor
			allocator->deallocate(ptr, 1); // Deallocate memory using the custom allocator
		}
	}

private:
	std::pmr::memory_resource *allocator;
};

template <typename T>
using UniquePtr = std::unique_ptr<T, UniquePtrDeleter<T>>;

template <typename T, typename... Args>
UniquePtr<T> allocate_unique(std::pmr::memory_resource *allocator, Args &&...args) {
	UniquePtrDeleter<T> customDeleter(allocator);

	return std::unique_ptr<T, UniquePtrDeleter<T>>(new (allocator->allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...), customDeleter);
}

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
public:
	constexpr MultiArray() :
			m_data{} {
	}

	constexpr MultiArray(const std::array<T, (Dims * ... * 1)> &data) :
			m_data{ data } {
	}

	constexpr MultiArray(std::array<T, (Dims * ... * 1)> &&data) :
			m_data{ std::move(data) } {
	}

	// Function to access elements in the multi-dimensional array
	template <typename... Args>
	constexpr const T &operator()(Args... indices) const {
		return m_data[MultiArrayIndex<Dims...>::computeIndex(indices...)];
	}

private:
	std::array<T, (Dims * ... * 1)> m_data;
};

#endif
