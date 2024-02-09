#include <phase4/engine/board/piece_square_tables/table_operations.h>

#include <doctest/doctest.h>

#include <array>

TEST_CASE("TableOperations flipVertically") {
	using namespace phase4::engine::board::piece_square_tables;

	const std::array<int32_t, 64> original = {
		10, 11, 12, 13, 14, 15, 16, 17,
		20, 21, 22, 23, 24, 25, 26, 27,
		30, 31, 32, 33, 34, 35, 36, 37,
		40, 41, 42, 43, 44, 45, 46, 47,
		50, 51, 52, 53, 54, 55, 56, 57,
		60, 61, 62, 63, 64, 65, 66, 67,
		70, 71, 72, 73, 74, 75, 76, 77,
		80, 81, 82, 83, 84, 85, 86, 87
	};
	const std::array<int32_t, 64> expected = {
		87, 86, 85, 84, 83, 82, 81, 80,
		77, 76, 75, 74, 73, 72, 71, 70,
		67, 66, 65, 64, 63, 62, 61, 60,
		57, 56, 55, 54, 53, 52, 51, 50,
		47, 46, 45, 44, 43, 42, 41, 40,
		37, 36, 35, 34, 33, 32, 31, 30,
		27, 26, 25, 24, 23, 22, 21, 20,
		17, 16, 15, 14, 13, 12, 11, 10
	};

	CHECK(expected == TableOperations::flipVertically(original));
}

TEST_CASE("TableOperations flipHorizontally") {
	using namespace phase4::engine::board::piece_square_tables;

	const std::array<int32_t, 64> original = {
		10, 11, 12, 13, 14, 15, 16, 17,
		20, 21, 22, 23, 24, 25, 26, 27,
		30, 31, 32, 33, 34, 35, 36, 37,
		40, 41, 42, 43, 44, 45, 46, 47,
		50, 51, 52, 53, 54, 55, 56, 57,
		60, 61, 62, 63, 64, 65, 66, 67,
		70, 71, 72, 73, 74, 75, 76, 77,
		80, 81, 82, 83, 84, 85, 86, 87
	};
	const std::array<int32_t, 64> expected = {
		17, 16, 15, 14, 13, 12, 11, 10,
		27, 26, 25, 24, 23, 22, 21, 20,
		37, 36, 35, 34, 33, 32, 31, 30,
		47, 46, 45, 44, 43, 42, 41, 40,
		57, 56, 55, 54, 53, 52, 51, 50,
		67, 66, 65, 64, 63, 62, 61, 60,
		77, 76, 75, 74, 73, 72, 71, 70,
		87, 86, 85, 84, 83, 82, 81, 80
	};

	CHECK(expected == TableOperations::flipHorizontally(original));
}
