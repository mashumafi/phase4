#include <phase4/engine/common/util.h>

#include <doctest/doctest.h>

TEST_CASE("MultiArrayIndex index") {
	SUBCASE("2 dimensions") {
		constexpr size_t index = MultiArrayIndex<3, 4>::computeIndex(3, 2);
		CHECK(MultiArrayIndex<3, 4>::computeIndex(1, 1) == (1 * 4) + (1));
		CHECK(MultiArrayIndex<3, 4>::computeIndex(1, 0) == (1 * 4) + (0));
		CHECK(MultiArrayIndex<3, 4>::computeIndex(0, 1) == (0 * 4) + (1));
	}

	SUBCASE("3 dimensions") {
		constexpr size_t index = MultiArrayIndex<3, 4, 5>::computeIndex(3, 2, 1);
		CHECK(MultiArrayIndex<3, 4, 5>::computeIndex(1, 1, 1) == (1 * 4 * 5) + (1 * 5) + (1));
		CHECK(MultiArrayIndex<3, 4, 5>::computeIndex(1, 1, 0) == (1 * 4 * 5) + (1 * 5) + (0));
		CHECK(MultiArrayIndex<3, 4, 5>::computeIndex(1, 0, 1) == (1 * 4 * 5) + (0 * 5) + (1));
		CHECK(MultiArrayIndex<3, 4, 5>::computeIndex(0, 1, 1) == (0 * 4 * 5) + (1 * 5) + (1));
	}
}
