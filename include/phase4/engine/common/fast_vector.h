#include <phase4/engine/common/util.h>

#include <array>
#include <cassert>
#include <memory>
#include <memory_resource>

namespace phase4::engine::common {

/// @brief Provides a fixed capacity std::array with a size
/// @tparam T element type used for the array
/// @tparam SIZE the capacity of the array
template <typename T, std::size_t SIZE = 1024>
class FastVector {
public:
	/// @brief
	/// @param upstream the upstream std::memory_resource if allocation is requried
	FastVector(std::pmr::memory_resource *upstream = std::pmr::get_default_resource());

	void push_back(const T &value);
	void push_back(T &&value);

	T &&pop_back();

	void clear();

	const T &at(std::size_t index) const;
	const T &peek() const;

	std::size_t size() const;
	bool is_empty() const;

private:
	using Buffer = std::array<T, SIZE>;

	UniquePtr<Buffer> m_array;
	std::size_t m_size = 0;
};

template <typename T, std::size_t SIZE>
FastVector<T, SIZE>::FastVector(std::pmr::memory_resource *upstream) :
		m_array(allocate_unique<Buffer>(upstream)) {
}

template <typename T, std::size_t SIZE>
void FastVector<T, SIZE>::push_back(const T &value) {
	assert(m_size < SIZE);

	m_array[m_size++] = value;
}

template <typename T, std::size_t SIZE>
void FastVector<T, SIZE>::push_back(T &&value) {
	assert(m_size < SIZE);

	(*m_array)[m_size++] = std::move(value);
}

template <typename T, std::size_t SIZE>
T &&FastVector<T, SIZE>::pop_back() {
	assert(!is_empty());

	return std::move((*m_array)[--m_size]);
}

template <typename T, std::size_t SIZE>
void FastVector<T, SIZE>::clear() {
	m_size = 0;
}

template <typename T, std::size_t SIZE>
const T &FastVector<T, SIZE>::at(std::size_t index) const {
	assert(index < m_size);

	return (*m_array)[index];
}

template <typename T, std::size_t SIZE>
const T &FastVector<T, SIZE>::peek() const {
	assert(!is_empty());

	return (*m_array)[m_size - 1];
}

template <typename T, std::size_t SIZE>
std::size_t FastVector<T, SIZE>::size() const {
	return m_size;
}

template <typename T, std::size_t SIZE>
bool FastVector<T, SIZE>::is_empty() const {
	return m_size == 0;
}

} //namespace phase4::engine::common
