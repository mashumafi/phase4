#include <phase4/engine/common/safe_vector.h>

#include <doctest/doctest.h>

#include <functional>
#include <cstdint>

TEST_CASE("SafeVector constructor") {
	using namespace phase4::engine::common;

	SUBCASE("No size") {
		SafeVector<uint64_t> numbers;
		(void)numbers;
	}

	SUBCASE("Sized") {
		SafeVector<uint64_t, 256> numbers;
		(void)numbers;
	}
}

TEST_CASE("SafeVector push_back, pop_back, peek") {
	using namespace phase4::engine::common;

	SafeVector<uint64_t, 4> numbers;
	numbers.push_back(5);
	CHECK(numbers.peek() == 5);
	numbers.push_back(4);
	CHECK(numbers.peek() == 4);
	numbers.push_back(3);
	CHECK(numbers.peek() == 3);
	numbers.push_back(2);
	CHECK(numbers.peek() == 2);

	CHECK(numbers.at(1) == 4);
	CHECK(numbers.at(3) == 2);

	CHECK(numbers.peek() == 2);
	CHECK(numbers.peek(2) == 4);

	CHECK(numbers.pop_back() == 2);
	CHECK(numbers.peek() == 3);
	CHECK(numbers.pop_back() == 3);
	CHECK(numbers.peek() == 4);

	numbers.clear();

	CHECK(numbers.is_empty());
}

TEST_CASE("SafeVector pop_back") {
	using namespace phase4::engine::common;

	uint64_t &&result = std::invoke([]() {
		SafeVector<uint64_t, 4> numbers;
		numbers.push_back(5);
		numbers.push_back(4);
		numbers.push_back(3);
		numbers.push_back(2);

		return numbers.pop_back();
	});

	CHECK(result == 2);
}
