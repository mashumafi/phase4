#ifndef PHASE4_ENGINE_COMMON_SAFE_VECTOR_H
#define PHASE4_ENGINE_COMMON_SAFE_VECTOR_H

#include <array>
#include <cassert>
#include <vector>

namespace phase4::engine::common {

/// @brief Provides a fixed size std::vector that can reallocate memory if needed
/// @tparam T element type used for the vector
/// @tparam SIZE the capacity of the vector
template <typename T, std::size_t SIZE = 1024>
class SafeVector {
public:
	/// @brief
	/// @param upstream the upstream std::memory_resource if allocation is requried
	SafeVector();

	void push_back(const T &value);
	void push_back(T &&value);

	T &&pop_back();

	void clear();

	const T &at(std::size_t index) const;
	const T &peek() const;

	std::size_t size() const;
	bool is_empty() const;

private:
	std::vector<T> m_vector;
};

template <typename T, std::size_t SIZE>
SafeVector<T, SIZE>::SafeVector() {
	m_vector.reserve(SIZE);
}

template <typename T, std::size_t SIZE>
void SafeVector<T, SIZE>::push_back(const T &value) {
	m_vector.push_back(value);
}

template <typename T, std::size_t SIZE>
void SafeVector<T, SIZE>::push_back(T &&value) {
	m_vector.push_back(std::move(value));
}

template <typename T, std::size_t SIZE>
T &&SafeVector<T, SIZE>::pop_back() {
	assert(!m_vector.empty());

	T &&back = std::move(m_vector.back());
	m_vector.pop_back();
	return std::move(back);
}

template <typename T, std::size_t SIZE>
void SafeVector<T, SIZE>::clear() {
	m_vector.clear();
}

template <typename T, std::size_t SIZE>
const T &SafeVector<T, SIZE>::at(std::size_t index) const {
	return m_vector.at(index);
}

template <typename T, std::size_t SIZE>
const T &SafeVector<T, SIZE>::peek() const {
	assert(!m_vector.empty());

	return m_vector[size() - 1];
}

template <typename T, std::size_t SIZE>
std::size_t SafeVector<T, SIZE>::size() const {
	return m_vector.size();
}

template <typename T, std::size_t SIZE>
bool SafeVector<T, SIZE>::is_empty() const {
	return m_vector.empty();
}

} //namespace phase4::engine::common

#endif
