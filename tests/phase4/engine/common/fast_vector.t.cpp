#include <phase4/engine/common/fast_vector.h>

#include <functional>

#include <doctest/doctest.h>

TEST_CASE("FastVector constructor") {
	using namespace phase4::engine::common;

	SUBCASE("No size") {
		FastVector<uint64_t> numbers;
	}

	SUBCASE("Sized") {
		FastVector<uint64_t, 256> numbers;
	}
}

TEST_CASE("FastVector push_back, pop_back, peek") {
	using namespace phase4::engine::common;

	FastVector<uint64_t, 4> numbers;
	numbers.push_back(5);
	CHECK(numbers.peek() == 5);
	numbers.push_back(4);
	CHECK(numbers.peek() == 4);
	numbers.push_back(3);
	CHECK(numbers.peek() == 3);
	numbers.push_back(2);
	CHECK(numbers.peek() == 2);

	CHECK(numbers.pop_back() == 2);
	CHECK(numbers.peek() == 3);
	CHECK(numbers.pop_back() == 3);
	CHECK(numbers.peek() == 4);

	numbers.clear();

	CHECK(numbers.is_empty());
}

TEST_CASE("FastVector pop_back") {
	using namespace phase4::engine::common;

	uint64_t &&result = std::invoke([]() {
		std::pmr::monotonic_buffer_resource memory_resource;
		FastVector<uint64_t, 4> numbers(&memory_resource);
		numbers.push_back(5);
		numbers.push_back(4);
		numbers.push_back(3);
		numbers.push_back(2);

		return numbers.pop_back();
	});

	CHECK(result == 2);
}
