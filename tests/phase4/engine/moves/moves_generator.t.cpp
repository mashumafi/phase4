#include <phase4/engine/moves/moves_generator.h>

#include <doctest/doctest.h>

#include <phase4/engine/moves/moves_generator.h>

#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

namespace {
phase4::engine::common::Bitset EMPTY(0);
}

TEST_CASE("MovesGenerator getKnightMoves") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;

	//                                                       HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
	//                                                       87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
	CHECK(MovesGenerator::getKnightMoves(Square::A1) == 0b00000000'00000000'00000000'00000000'00000000'00000010'00000100'00000000);
	CHECK(MovesGenerator::getKnightMoves(Square::A2) == 0b00000000'00000000'00000000'00000000'00000000'00000101'00001000'00000000);
	CHECK(MovesGenerator::getKnightMoves(Square::B2) == 0b00000000'00000000'00000000'00000000'00000101'00001000'00000000'00001000);
	CHECK(MovesGenerator::getKnightMoves(Square::C5) == 0b00000000'00000000'00000000'00101000'01000100'00000000'01000100'00101000);
	CHECK(MovesGenerator::getKnightMoves(Square::D8) == 0b00000000'00000000'01000000'00100000'00000000'00100000'01000000'00000000);
	CHECK(MovesGenerator::getKnightMoves(Square::E1) == 0b00000000'00000010'00000100'00000000'00000100'00000010'00000000'00000000);
	CHECK(MovesGenerator::getKnightMoves(Square::F4) == 0b00010100'00100010'00000000'00100010'00010100'00000000'00000000'00000000);
	CHECK(MovesGenerator::getKnightMoves(Square::G7) == 0b00010000'00000000'00010000'10100000'00000000'00000000'00000000'00000000);
	CHECK(MovesGenerator::getKnightMoves(Square::H2) == 0b00000000'00001000'00000101'00000000'00000000'00000000'00000000'00000000);
}

TEST_CASE("MovesGenerator getBishopMoves") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::moves::magic;

	return; // TODO: Enable tests

	MagicBitboards::InitWithInternalKeys();

	SUBCASE("Empty occupancy") {
		//                                                           HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
		//                                                           87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
		CHECK(MovesGenerator::getBishopMoves(EMPTY, Square::A1) == 0b0);
		CHECK(MovesGenerator::getBishopMoves(EMPTY, Square::A2) == 0b0);
		CHECK(MovesGenerator::getBishopMoves(EMPTY, Square::B2) == 0b0);
		CHECK(MovesGenerator::getBishopMoves(EMPTY, Square::C5) == 0b0);
		CHECK(MovesGenerator::getBishopMoves(EMPTY, Square::D8) == 0b0);
		CHECK(MovesGenerator::getBishopMoves(EMPTY, Square::E1) == 0b0);
		CHECK(MovesGenerator::getBishopMoves(EMPTY, Square::F4) == 0b0);
		CHECK(MovesGenerator::getBishopMoves(EMPTY, Square::G7) == 0b0);
		CHECK(MovesGenerator::getBishopMoves(EMPTY, Square::H2) == 0b0);
	}
}

TEST_CASE("MovesGenerator getRookMoves") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::moves::magic;

	return; // TODO: Enable tests

	MagicBitboards::InitWithInternalKeys();

	SUBCASE("Empty occupancy") {
		//                                                         HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
		//                                                         87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
		CHECK(MovesGenerator::getRookMoves(EMPTY, Square::A1) == 0b0);
		CHECK(MovesGenerator::getRookMoves(EMPTY, Square::A2) == 0b0);
		CHECK(MovesGenerator::getRookMoves(EMPTY, Square::B2) == 0b0);
		CHECK(MovesGenerator::getRookMoves(EMPTY, Square::C5) == 0b0);
		CHECK(MovesGenerator::getRookMoves(EMPTY, Square::D8) == 0b0);
		CHECK(MovesGenerator::getRookMoves(EMPTY, Square::E1) == 0b0);
		CHECK(MovesGenerator::getRookMoves(EMPTY, Square::F4) == 0b0);
		CHECK(MovesGenerator::getRookMoves(EMPTY, Square::G7) == 0b0);
		CHECK(MovesGenerator::getRookMoves(EMPTY, Square::H2) == 0b0);
	}
}

TEST_CASE("MovesGenerator getQueenMoves") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::moves::magic;

	return; // TODO: Enable tests

	MagicBitboards::InitWithInternalKeys();

	SUBCASE("Empty occupancy") {
		//                                                          HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
		//                                                          87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
		CHECK(MovesGenerator::getQueenMoves(EMPTY, Square::A1) == 0b0);
		CHECK(MovesGenerator::getQueenMoves(EMPTY, Square::A2) == 0b0);
		CHECK(MovesGenerator::getQueenMoves(EMPTY, Square::B2) == 0b0);
		CHECK(MovesGenerator::getQueenMoves(EMPTY, Square::C5) == 0b0);
		CHECK(MovesGenerator::getQueenMoves(EMPTY, Square::D8) == 0b0);
		CHECK(MovesGenerator::getQueenMoves(EMPTY, Square::E1) == 0b0);
		CHECK(MovesGenerator::getQueenMoves(EMPTY, Square::F4) == 0b0);
		CHECK(MovesGenerator::getQueenMoves(EMPTY, Square::G7) == 0b0);
		CHECK(MovesGenerator::getQueenMoves(EMPTY, Square::H2) == 0b0);
	}
}

TEST_CASE("MovesGenerator getKingMoves") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;

	//                                                  HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
	//                                                  87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
	CHECK(MovesGenerator::getKingMoves(Square::A1) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000011'00000010);
	CHECK(MovesGenerator::getKingMoves(Square::A2) == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000111'00000101);
	CHECK(MovesGenerator::getKingMoves(Square::B2) == 0b00000000'00000000'00000000'00000000'00000000'00000111'00000101'00000111);
	CHECK(MovesGenerator::getKingMoves(Square::C5) == 0b00000000'00000000'00000000'00000000'00111000'00101000'00111000'00000000);
	CHECK(MovesGenerator::getKingMoves(Square::D8) == 0b00000000'00000000'00000000'11000000'01000000'11000000'00000000'00000000);
	CHECK(MovesGenerator::getKingMoves(Square::E1) == 0b00000000'00000000'00000011'00000010'00000011'00000000'00000000'00000000);
	CHECK(MovesGenerator::getKingMoves(Square::F4) == 0b00000000'00011100'00010100'00011100'00000000'00000000'00000000'00000000);
	CHECK(MovesGenerator::getKingMoves(Square::G7) == 0b11100000'10100000'11100000'00000000'00000000'00000000'00000000'00000000);
	CHECK(MovesGenerator::getKingMoves(Square::H2) == 0b00000101'00000111'00000000'00000000'00000000'00000000'00000000'00000000);
}
