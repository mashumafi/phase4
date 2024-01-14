#include <array>
#include <cassert>
#include <memory_resource>
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
	SafeVector(std::pmr::memory_resource *upstream = std::pmr::null_memory_resource());

	void push_back(const T &value);
	void push_back(T &&value);

	T &&pop_back();

	std::size_t size() const;
	bool is_empty() const;

private:
	std::array<T, SIZE> m_buffer;
	std::pmr::monotonic_buffer_resource m_pool;
	std::pmr::vector<T> m_vector;
};

template <typename T, std::size_t SIZE>
SafeVector<T, SIZE>::SafeVector(std::pmr::memory_resource *upstream) :
		m_pool(m_buffer.data(), sizeof(m_buffer), upstream),
		m_vector(&m_pool) {
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
std::size_t SafeVector<T, SIZE>::size() const {
	return m_vector.size();
}

template <typename T, std::size_t SIZE>
bool SafeVector<T, SIZE>::is_empty() const {
	return m_vector.empty();
}

} //namespace phase4::engine::common
