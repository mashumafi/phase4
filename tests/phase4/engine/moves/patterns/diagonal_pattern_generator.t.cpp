#include <phase4/engine/moves/patterns/diagonal_pattern_generator.h>

#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("DiagonalPatternGenerator getPattern") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	//                                                          ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH
	//                                                          88888888 77777777 66666666 55555555 44444444 33333333 22222222 11111111
	CHECK(DiagonalPatternGenerator::getPattern(Square::H1) == 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000000);
	CHECK(DiagonalPatternGenerator::getPattern(Square::G1) == 0b00000000'10000000'01000000'00100000'00010000'00001000'00000101'00000000);
	CHECK(DiagonalPatternGenerator::getPattern(Square::G2) == 0b10000000'01000000'00100000'00010000'00001000'00000101'00000000'00000101);
	CHECK(DiagonalPatternGenerator::getPattern(Square::D3) == 0b00000000'00000001'10000010'01000100'00101000'00000000'00101000'01000100);
	CHECK(DiagonalPatternGenerator::getPattern(Square::A4) == 0b00001000'00010000'00100000'01000000'00000000'01000000'00100000'00010000);
	CHECK(DiagonalPatternGenerator::getPattern(Square::H5) == 0b00001000'00000100'00000010'00000000'00000010'00000100'00001000'00010000);
	CHECK(DiagonalPatternGenerator::getPattern(Square::E6) == 0b00100010'00010100'00000000'00010100'00100010'01000001'10000000'00000000);
	CHECK(DiagonalPatternGenerator::getPattern(Square::B7) == 0b10100000'00000000'10100000'00010000'00001000'00000100'00000010'00000001);
	CHECK(DiagonalPatternGenerator::getPattern(Square::G8) == 0b00000000'00000101'00001000'00010000'00100000'01000000'10000000'00000000);
}
