#ifndef PHASE4_ENGINE_COMMON_SAFE_VECTOR_H
#define PHASE4_ENGINE_COMMON_SAFE_VECTOR_H

#include <array>
#include <iostream>

namespace phase4::engine::common {

/// @brief Provides a fixed capacity std::array with a size
/// @tparam T element type used for the array
/// @tparam N the capacity of the array
template <typename T, std::size_t N = 512>
class SafeVector {
public:
	inline constexpr SafeVector();
	inline constexpr SafeVector(std::initializer_list<T> items);

	inline constexpr void push_back(const T &value) noexcept;
	inline constexpr void push_back(T &&value) noexcept;

	template <class... Args>
	inline constexpr void emplace_back(Args &&...args) noexcept {
		m_items[m_size++] = T(args...);
	}

	inline constexpr T &&pop_back() noexcept;

	inline constexpr void clear() noexcept;

	inline constexpr const T &at(std::size_t index) const noexcept;
	inline constexpr const T &peek() const noexcept;
	inline constexpr const T &peek(std::size_t index) const noexcept;

	inline constexpr std::size_t size() const noexcept;
	inline constexpr bool is_empty() const noexcept;

private:
	std::array<T, N> m_items = {};
	size_t m_size;
};

template <typename T, std::size_t N>
inline constexpr SafeVector<T, N>::SafeVector() :
		m_size(0) {
}

template <typename T, std::size_t N>
inline constexpr SafeVector<T, N>::SafeVector(std::initializer_list<T> items) :
		m_items(items),
		m_size(items.size()) {
}

template <typename T, std::size_t N>
inline constexpr void SafeVector<T, N>::push_back(const T &value) noexcept {
	m_items[m_size++] = value;
}

template <typename T, std::size_t N>
inline constexpr void SafeVector<T, N>::push_back(T &&value) noexcept {
	m_items[m_size++] = std::move(value);
}

template <typename T, std::size_t N>
inline constexpr T &&SafeVector<T, N>::pop_back() noexcept {
	return std::move(m_items[--m_size]);
}

template <typename T, std::size_t N>
inline constexpr void SafeVector<T, N>::clear() noexcept {
	m_size = 0;
}

template <typename T, std::size_t N>
inline constexpr const T &SafeVector<T, N>::at(std::size_t index) const noexcept {
	return m_items[index];
}

template <typename T, std::size_t N>
inline constexpr const T &SafeVector<T, N>::peek() const noexcept {
	return m_items[m_size - 1];
}

template <typename T, std::size_t N>
inline constexpr const T &SafeVector<T, N>::peek(std::size_t index) const noexcept {
	return m_items[m_size - 1 - index];
}

template <typename T, std::size_t N>
inline constexpr std::size_t SafeVector<T, N>::size() const noexcept {
	return m_size;
}

template <typename T, std::size_t N>
inline constexpr bool SafeVector<T, N>::is_empty() const noexcept {
	return m_size == 0;
}

template <typename T, std::size_t N>
inline std::ostream &operator<<(std::ostream &os, const SafeVector<T, N> &vector) {
	for (size_t i = 0; i < vector.size(); ++i) {
		if (i > 0) {
			os << " ";
		}
		os << vector[i];
	}
	return os;
}

} //namespace phase4::engine::common

#endif
