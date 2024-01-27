#include <phase4/engine/moves/patterns/chain_pattern_generator.h>

#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("ChainPatternGenerator getPattern") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	//                                                       HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
	//                                                       87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
	CHECK(ChainPatternGenerator::getPattern(Square::H1) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000010'00000000);
	CHECK(ChainPatternGenerator::getPattern(Square::G1) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000101'00000000);
	CHECK(ChainPatternGenerator::getPattern(Square::G2) == 0b00000000'00000000'00000000'00000000'00000000'00000101'00000000'00000101);
	CHECK(ChainPatternGenerator::getPattern(Square::D3) == 0b00000000'00000000'00000000'00000000'00101000'00000000'00101000'00000000);
	CHECK(ChainPatternGenerator::getPattern(Square::A4) == 0b00000000'00000000'00000000'01000000'00000000'01000000'00000000'00000000);
	CHECK(ChainPatternGenerator::getPattern(Square::H5) == 0b00000000'00000000'00000010'00000000'00000010'00000000'00000000'00000000);
	CHECK(ChainPatternGenerator::getPattern(Square::E6) == 0b00000000'00010100'00000000'00010100'00000000'00000000'00000000'00000000);
	CHECK(ChainPatternGenerator::getPattern(Square::B7) == 0b10100000'00000000'10100000'00000000'00000000'00000000'00000000'00000000);
	CHECK(ChainPatternGenerator::getPattern(Square::G8) == 0b00000000'00000101'00000000'00000000'00000000'00000000'00000000'00000000);
}