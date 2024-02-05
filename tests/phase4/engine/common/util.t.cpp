#include <phase4/engine/common/util.h>

#include <doctest/doctest.h>

#include <array>

TEST_CASE("MultiArrayIndex index") {
	SUBCASE("2 dimensions") {
		constexpr size_t index = MultiArrayIndex<3, 4>::computeIndex(2, 2);
		CHECK(index == 10);
		CHECK(MultiArrayIndex<3, 4>::computeIndex(1, 1) == (1 * 4) + (1));
		CHECK(MultiArrayIndex<3, 4>::computeIndex(1, 0) == (1 * 4) + (0));
		CHECK(MultiArrayIndex<3, 4>::computeIndex(0, 1) == (0 * 4) + (1));
	}

	SUBCASE("3 dimensions") {
		constexpr size_t index = MultiArrayIndex<4, 5, 6>::computeIndex(3, 2, 1);
		CHECK(index == 103);
		CHECK(MultiArrayIndex<3, 4, 5>::computeIndex(1, 1, 1) == (1 * 4 * 5) + (1 * 5) + (1));
		CHECK(MultiArrayIndex<3, 4, 5>::computeIndex(1, 1, 0) == (1 * 4 * 5) + (1 * 5) + (0));
		CHECK(MultiArrayIndex<3, 4, 5>::computeIndex(1, 0, 1) == (1 * 4 * 5) + (0 * 5) + (1));
		CHECK(MultiArrayIndex<3, 4, 5>::computeIndex(0, 1, 1) == (0 * 4 * 5) + (1 * 5) + (1));
	}
}

TEST_CASE("util::clear") {
	using namespace phase4::engine::common;

	std::array<std::array<uint64_t, 3>, 3> num = {
		std::array<uint64_t, 3>{ 1, 2, 3 },
		std::array<uint64_t, 3>{ 4, 5, 6 },
		std::array<uint64_t, 3>{ 7, 8, 9 },
	};

	util::clear(num[1]);

	CHECK(num == std::array<std::array<uint64_t, 3>, 3>{ std::array<uint64_t, 3>{ 1, 2, 3 }, std::array<uint64_t, 3>{ 0, 0, 0 }, std::array<uint64_t, 3>{ 7, 8, 9 } });

	util::clear(num);

	CHECK(num == std::array<std::array<uint64_t, 3>, 3>{ std::array<uint64_t, 3>{ 0, 0, 0 }, std::array<uint64_t, 3>{ 0, 0, 0 }, std::array<uint64_t, 3>{ 0, 0, 0 } });
}