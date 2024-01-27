#include <phase4/engine/moves/patterns/box_pattern_generator.h>

#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("BoxPatternGenerator getPattern") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	//                                                     ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH
	//                                                     88888888 77777777 66666666 55555555 44444444 33333333 22222222 11111111
	CHECK(BoxPatternGenerator::getPattern(Square::H1) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000011'00000010);
	CHECK(BoxPatternGenerator::getPattern(Square::G1) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000111'00000101);
	CHECK(BoxPatternGenerator::getPattern(Square::G2) == 0b00000000'00000000'00000000'00000000'00000000'00000111'00000101'00000111);
	CHECK(BoxPatternGenerator::getPattern(Square::D3) == 0b00000000'00000000'00000000'00000000'00111000'00101000'00111000'00000000);
	CHECK(BoxPatternGenerator::getPattern(Square::A4) == 0b00000000'00000000'00000000'11000000'01000000'11000000'00000000'00000000);
	CHECK(BoxPatternGenerator::getPattern(Square::H5) == 0b00000000'00000000'00000011'00000010'00000011'00000000'00000000'00000000);
	CHECK(BoxPatternGenerator::getPattern(Square::E6) == 0b00000000'00011100'00010100'00011100'00000000'00000000'00000000'00000000);
	CHECK(BoxPatternGenerator::getPattern(Square::B7) == 0b11100000'10100000'11100000'00000000'00000000'00000000'00000000'00000000);
	CHECK(BoxPatternGenerator::getPattern(Square::G8) == 0b00000101'00000111'00000000'00000000'00000000'00000000'00000000'00000000);
}
