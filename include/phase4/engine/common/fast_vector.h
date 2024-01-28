#ifndef PHASE4_ENGINE_COMMON_FAST_VECTOR_H
#define PHASE4_ENGINE_COMMON_FAST_VECTOR_H

#include <phase4/engine/common/util.h>

#include <cassert>
#include <type_traits>

namespace phase4::engine::common {

/// @brief Provides a fixed capacity raw array with a size
/// @tparam T element type used for the array
/// @tparam N the capacity of the array
template <typename T, std::size_t N = 512>
class FastVector {
public:
	FastVector();

	void push_back(const T &value);
	void push_back(T &&value);

	template <class... Args>
	void emplace_back(Args &&...args) {
		m_items[m_size++] = T(args...);
	}

	T &&pop_back();

	void clear();

	const T &at(std::size_t index) const;
	const T &peek() const;

	std::size_t size() const;
	bool is_empty() const;

private:
	T m_items[N];
	size_t m_size;
};

template <typename T, std::size_t N>
FastVector<T, N>::FastVector() :
		m_size(0) {
}

template <typename T, std::size_t N>
void FastVector<T, N>::push_back(const T &value) {
	assert(m_size < N);

	m_items[m_size++] = value;
}

template <typename T, std::size_t N>
void FastVector<T, N>::push_back(T &&value) {
	assert(m_size < N);

	m_items[m_size++] = std::move(value);
}

template <typename T, std::size_t N>
T &&FastVector<T, N>::pop_back() {
	assert(!is_empty());

	return std::move(m_items[--m_size]);
}

template <typename T, std::size_t N>
void FastVector<T, N>::clear() {
	m_size = 0;
}

template <typename T, std::size_t N>
const T &FastVector<T, N>::at(std::size_t index) const {
	assert(index < m_size);

	return m_items[index];
}

template <typename T, std::size_t N>
const T &FastVector<T, N>::peek() const {
	assert(!is_empty());

	return m_items[m_size - 1];
}

template <typename T, std::size_t N>
std::size_t FastVector<T, N>::size() const {
	return m_size;
}

template <typename T, std::size_t N>
bool FastVector<T, N>::is_empty() const {
	return m_size == 0;
}

} //namespace phase4::engine::common

#endif
