#include <phase4/engine/moves/patterns/diagonal_pattern_generator.h>

#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("DiagonalPatternGenerator getPattern") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	//                                                          HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
	//                                                          87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
	CHECK(DiagonalPatternGenerator::getPattern(Square::A1) == 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000000ull);
	CHECK(DiagonalPatternGenerator::getPattern(Square::A2) == 0b00000000'10000000'01000000'00100000'00010000'00001000'00000101'00000000ull);
	CHECK(DiagonalPatternGenerator::getPattern(Square::B2) == 0b10000000'01000000'00100000'00010000'00001000'00000101'00000000'00000101ull);
	CHECK(DiagonalPatternGenerator::getPattern(Square::C5) == 0b00000000'00000001'10000010'01000100'00101000'00000000'00101000'01000100ull);
	CHECK(DiagonalPatternGenerator::getPattern(Square::D8) == 0b00001000'00010000'00100000'01000000'00000000'01000000'00100000'00010000ull);
	CHECK(DiagonalPatternGenerator::getPattern(Square::E1) == 0b00001000'00000100'00000010'00000000'00000010'00000100'00001000'00010000ull);
	CHECK(DiagonalPatternGenerator::getPattern(Square::F4) == 0b00100010'00010100'00000000'00010100'00100010'01000001'10000000'00000000ull);
	CHECK(DiagonalPatternGenerator::getPattern(Square::G7) == 0b10100000'00000000'10100000'00010000'00001000'00000100'00000010'00000001ull);
	CHECK(DiagonalPatternGenerator::getPattern(Square::H2) == 0b00000000'00000101'00001000'00010000'00100000'01000000'10000000'00000000ull);
}
