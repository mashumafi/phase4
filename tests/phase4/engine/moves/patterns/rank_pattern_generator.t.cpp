#include <phase4/engine/moves/patterns/rank_pattern_generator.h>

#include <phase4/engine/common/position_constants.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("RankPatternGenerator getPatternForField") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	//                                                              ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH
	//                                                              88888888 77777777 66666666 55555555 44444444 33333333 22222222 11111111
	CHECK(RankPatternGenerator::getPatternForField(Square::H1) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111110);
	CHECK(RankPatternGenerator::getPatternForField(Square::G1) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111101);
	CHECK(RankPatternGenerator::getPatternForField(Square::G2) == 0b00000000'00000000'00000000'00000000'00000000'00000000'11111101'00000000);
	CHECK(RankPatternGenerator::getPatternForField(Square::D3) == 0b00000000'00000000'00000000'00000000'00000000'11101111'00000000'00000000);
	CHECK(RankPatternGenerator::getPatternForField(Square::A4) == 0b00000000'00000000'00000000'00000000'01111111'00000000'00000000'00000000);
	CHECK(RankPatternGenerator::getPatternForField(Square::H5) == 0b00000000'00000000'00000000'11111110'00000000'00000000'00000000'00000000);
	CHECK(RankPatternGenerator::getPatternForField(Square::E6) == 0b00000000'00000000'11110111'00000000'00000000'00000000'00000000'00000000);
	CHECK(RankPatternGenerator::getPatternForField(Square::B7) == 0b00000000'10111111'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(RankPatternGenerator::getPatternForField(Square::G8) == 0b11111101'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
}

TEST_CASE("RankPatternGenerator getPatternForRank") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	CHECK(RankPatternGenerator::getPatternForRank(0) == PositionConstants::RANK_1);
	CHECK(RankPatternGenerator::getPatternForRank(1) == PositionConstants::RANK_2);
	CHECK(RankPatternGenerator::getPatternForRank(2) == PositionConstants::RANK_3);
	CHECK(RankPatternGenerator::getPatternForRank(3) == PositionConstants::RANK_4);
	CHECK(RankPatternGenerator::getPatternForRank(4) == PositionConstants::RANK_5);
	CHECK(RankPatternGenerator::getPatternForRank(5) == PositionConstants::RANK_6);
	CHECK(RankPatternGenerator::getPatternForRank(6) == PositionConstants::RANK_7);
	CHECK(RankPatternGenerator::getPatternForRank(7) == PositionConstants::RANK_8);
}
