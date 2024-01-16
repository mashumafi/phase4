#include <phase4/engine/moves/magic/attacks_generator.h>

#include <phase4/engine/common/bitset.h>

#include <doctest/doctest.h>

namespace
{
phase4::engine::common::Bitset EMPTY(0);
}

TEST_CASE("AttacksGenerator getFileRankAttacks") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::moves::magic;

	SUBCASE("Empty occupancy") {
		//                                                                 HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
		//                                                                 87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
		CHECK(AttacksGenerator::getFileRankAttacks(EMPTY, Square::A1) == 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'11111110);
		CHECK(AttacksGenerator::getFileRankAttacks(EMPTY, Square::A2) == 0b00000010'00000010'00000010'00000010'00000010'00000010'00000010'11111101);
		CHECK(AttacksGenerator::getFileRankAttacks(EMPTY, Square::B2) == 0b00000010'00000010'00000010'00000010'00000010'00000010'11111101'00000010);
		CHECK(AttacksGenerator::getFileRankAttacks(EMPTY, Square::C5) == 0b00010000'00010000'00010000'00010000'00010000'11101111'00010000'00010000);
		CHECK(AttacksGenerator::getFileRankAttacks(EMPTY, Square::D8) == 0b10000000'10000000'10000000'10000000'01111111'10000000'10000000'10000000);
		CHECK(AttacksGenerator::getFileRankAttacks(EMPTY, Square::E1) == 0b00000001'00000001'00000001'11111110'00000001'00000001'00000001'00000001);
		CHECK(AttacksGenerator::getFileRankAttacks(EMPTY, Square::F4) == 0b00001000'00001000'11110111'00001000'00001000'00001000'00001000'00001000);
		CHECK(AttacksGenerator::getFileRankAttacks(EMPTY, Square::G7) == 0b01000000'10111111'01000000'01000000'01000000'01000000'01000000'01000000);
		CHECK(AttacksGenerator::getFileRankAttacks(EMPTY, Square::H2) == 0b11111101'00000010'00000010'00000010'00000010'00000010'00000010'00000010);
	}
}

TEST_CASE("AttacksGenerator getDiagonalAttacks") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::moves::magic;

	SUBCASE("Empty occupancy") {
		//                                                                HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
		//                                                                87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
		CHECK(AttacksGenerator::getDiagonalAttacks(EMPTY, Square::A1) == 0b1000000'00100000'00010000'00001000'00000100'00000010'00000001'000000000);
		CHECK(AttacksGenerator::getDiagonalAttacks(EMPTY, Square::A2) == 0b0000000'01000000'00100000'00010000'00001000'00000100'00000010'100000000);
		CHECK(AttacksGenerator::getDiagonalAttacks(EMPTY, Square::B2) == 0b1000000'00100000'00010000'00001000'00000100'00000010'10000000'000000101);
		CHECK(AttacksGenerator::getDiagonalAttacks(EMPTY, Square::C5) == 0b0000000'00000000'11000001'00100010'00010100'00000000'00010100'001000100);
		CHECK(AttacksGenerator::getDiagonalAttacks(EMPTY, Square::D8) == 0b0000100'00001000'00010000'00100000'00000000'00100000'00010000'000010000);
		CHECK(AttacksGenerator::getDiagonalAttacks(EMPTY, Square::E1) == 0b0000100'00000010'00000001'00000000'00000001'00000010'00000100'000010000);
		CHECK(AttacksGenerator::getDiagonalAttacks(EMPTY, Square::F4) == 0b0010001'00001010'00000000'00001010'00010001'00100000'11000000'000000000);
		CHECK(AttacksGenerator::getDiagonalAttacks(EMPTY, Square::G7) == 0b1010000'00000000'01010000'00001000'00000100'00000010'00000001'000000001);
		CHECK(AttacksGenerator::getDiagonalAttacks(EMPTY, Square::H2) == 0b0000000'00000010'10000100'00001000'00010000'00100000'01000000'000000000);
	}
}