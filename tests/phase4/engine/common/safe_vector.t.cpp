#include <phase4/engine/common/safe_vector.h>

#include <functional>

#include <doctest/doctest.h>

TEST_CASE("SafeVector constructor") {
	using namespace phase4::engine::common;

	SUBCASE("No size") {
		SafeVector<uint64_t> numbers;
	}

	SUBCASE("Sized") {
		SafeVector<uint64_t, 256> numbers;
	}
}

TEST_CASE("SafeVector push_back") {
	using namespace phase4::engine::common;

	SafeVector<uint64_t, 4> numbers;
	numbers.push_back(5);
	numbers.push_back(4);
	numbers.push_back(3);
	numbers.push_back(2);

	CHECK(numbers.pop_back() == 2);
	CHECK(numbers.pop_back() == 3);
	CHECK(numbers.pop_back() == 4);
	CHECK(numbers.pop_back() == 5);
	CHECK(numbers.is_empty());
}

TEST_CASE("SafeVector pop_back") {
	using namespace phase4::engine::common;

	uint64_t &&result = std::invoke([]() {
		std::pmr::monotonic_buffer_resource memory_resource;
		SafeVector<uint64_t, 4> numbers(&memory_resource);
		numbers.push_back(5);
		numbers.push_back(4);
		numbers.push_back(3);
		numbers.push_back(2);

		return numbers.pop_back();
	});

	CHECK(result == 2);
}