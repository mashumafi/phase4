#include <phase4/engine/common/random.h>

#include <phase4/engine/common/bitboard.h>

#include <doctest/doctest.h>

#include <unordered_set>

TEST_CASE("Random seed next") {
	using namespace phase4::engine::common;

	Random rand(42);

	CHECK(Bitboard(rand.next()) == 0b1000000010110000000001010100001100100100010111011011001011001110);
	CHECK(Bitboard(rand.next()) == 0b1100000110010010001100001001110101000100010010100111111111001000);
	CHECK(Bitboard(rand.next()) == 0b1000111101111111001110010010100111111010011000010011000110111111);
	CHECK(Bitboard(rand.next()) == 0b1011010100110100111100111100001101110001110100001001000010110101);
	CHECK(Bitboard(rand.next()) == 0b0001010110001110101100011100001111110010100110011010100110011000);
	CHECK(Bitboard(rand.next()) == 0b0001000011110100110001001001010010110001101100011011000111101100);
	CHECK(Bitboard(rand.next()) == 0b0011001101100101100001100111000011000100011100101101011110110100);
}

TEST_CASE("Random no repeats") {
	using namespace phase4::engine::common;

	Random rand(42);
	std::unordered_set<size_t> results;
	for (size_t i = 0; i < 500'000; ++i) {
		REQUIRE(results.insert(rand.next()).second);
	}
}
