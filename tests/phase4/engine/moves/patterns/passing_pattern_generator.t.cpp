#include <phase4/engine/moves/patterns/passing_pattern_generator.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("PassingPatternGenerator getPattern") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	//                                                                            ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH
	//                                                                            88888888 77777777 66666666 55555555 44444444 33333333 22222222 11111111
	CHECK(PassingPatternGenerator::getPattern(PieceColor::WHITE, Square::H1) == 0b00000011'00000011'00000011'00000011'00000011'00000011'00000011'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::WHITE, Square::G1) == 0b00000111'00000111'00000111'00000111'00000111'00000111'00000111'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::WHITE, Square::G2) == 0b00000111'00000111'00000111'00000111'00000111'00000111'00000000'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::WHITE, Square::D3) == 0b00111000'00111000'00111000'00111000'00111000'00000000'00000000'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::WHITE, Square::A4) == 0b11000000'11000000'11000000'11000000'00000000'00000000'00000000'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::WHITE, Square::H5) == 0b00000011'00000011'00000011'00000000'00000000'00000000'00000000'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::WHITE, Square::E6) == 0b00010100'00011100'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::WHITE, Square::B7) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::WHITE, Square::G8) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);

	//                                                                            ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH
	//                                                                            88888888 77777777 66666666 55555555 44444444 33333333 22222222 11111111
	CHECK(PassingPatternGenerator::getPattern(PieceColor::BLACK, Square::H1) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::BLACK, Square::G1) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::BLACK, Square::G2) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::BLACK, Square::D3) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00111000'00101000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::BLACK, Square::A4) == 0b00000000'00000000'00000000'00000000'00000000'11000000'11000000'11000000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::BLACK, Square::H5) == 0b00000000'00000000'00000000'00000000'00000011'00000011'00000011'00000011);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::BLACK, Square::E6) == 0b00000000'00000000'00000000'00011100'00011100'00011100'00011100'00011100);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::BLACK, Square::B7) == 0b00000000'00000000'11100000'11100000'11100000'11100000'11100000'11100000);
	CHECK(PassingPatternGenerator::getPattern(PieceColor::BLACK, Square::G8) == 0b00000000'00000111'00000111'00000111'00000111'00000111'00000111'00000111);
}