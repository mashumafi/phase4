#include <phase4/engine/moves/magic/permutations_generator.h>

#include <phase4/engine/common/bitset.h>

#include <doctest/doctest.h>

TEST_CASE("PermutationsGenerator getPermutation") {
	using namespace phase4::engine::moves::magic;
	using namespace phase4::engine::common;

	CHECK(PermutationsGenerator::getPermutation(0b1101, 0) == 0b0000);
	CHECK(PermutationsGenerator::getPermutation(0b1101, 1) == 0b0001);
	CHECK(PermutationsGenerator::getPermutation(0b1101, 2) == 0b0100);
	CHECK(PermutationsGenerator::getPermutation(0b1101, 3) == 0b0101);
	CHECK(PermutationsGenerator::getPermutation(0b1101, 4) == 0b1000);
	CHECK(PermutationsGenerator::getPermutation(0b1101, 5) == 0b1001);
	CHECK(PermutationsGenerator::getPermutation(0b1101, 6) == 0b1100);
	CHECK(PermutationsGenerator::getPermutation(0b1101, 7) == 0b1101);
	CHECK(PermutationsGenerator::getPermutation(0b1101, 8) == 0b0000);
}
