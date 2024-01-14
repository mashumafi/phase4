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
		if (ptr) {
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
