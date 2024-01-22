#include <phase4/engine/moves/patterns/rank_pattern_generator.h>

#include <phase4/engine/board/board_constants.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("RankPatternGenerator getPatternForField") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	//                                                              HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
	//                                                              87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
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
	using namespace phase4::engine::board;
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	CHECK(RankPatternGenerator::getPatternForRank(0) == BoardConstants::RANK_1);
	CHECK(RankPatternGenerator::getPatternForRank(1) == BoardConstants::RANK_2);
	CHECK(RankPatternGenerator::getPatternForRank(2) == BoardConstants::RANK_3);
	CHECK(RankPatternGenerator::getPatternForRank(3) == BoardConstants::RANK_4);
	CHECK(RankPatternGenerator::getPatternForRank(4) == BoardConstants::RANK_5);
	CHECK(RankPatternGenerator::getPatternForRank(5) == BoardConstants::RANK_6);
	CHECK(RankPatternGenerator::getPatternForRank(6) == BoardConstants::RANK_7);
	CHECK(RankPatternGenerator::getPatternForRank(7) == BoardConstants::RANK_8);
}
