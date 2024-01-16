#include <phase4/engine/moves/patterns/box_pattern_generator.h>

#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("BoxPatternGenerator getPattern") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	//                                                     HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
	//                                                     87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
	CHECK(BoxPatternGenerator::getPattern(Square::A1) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000011'00000010);
	CHECK(BoxPatternGenerator::getPattern(Square::A2) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000111'00000101);
	CHECK(BoxPatternGenerator::getPattern(Square::B2) == 0b00000000'00000000'00000000'00000000'00000000'00000111'00000101'00000111);
	CHECK(BoxPatternGenerator::getPattern(Square::C5) == 0b00000000'00000000'00000000'00000000'00111000'00101000'00111000'00000000);
	CHECK(BoxPatternGenerator::getPattern(Square::D8) == 0b00000000'00000000'00000000'11000000'01000000'11000000'00000000'00000000);
	CHECK(BoxPatternGenerator::getPattern(Square::E1) == 0b00000000'00000000'00000011'00000010'00000011'00000000'00000000'00000000);
	CHECK(BoxPatternGenerator::getPattern(Square::F4) == 0b00000000'00011100'00010100'00011100'00000000'00000000'00000000'00000000);
	CHECK(BoxPatternGenerator::getPattern(Square::G7) == 0b11100000'10100000'11100000'00000000'00000000'00000000'00000000'00000000);
	CHECK(BoxPatternGenerator::getPattern(Square::H2) == 0b00000101'00000111'00000000'00000000'00000000'00000000'00000000'00000000);
}
