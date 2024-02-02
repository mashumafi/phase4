#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

#include <sstream>

TEST_CASE("Square output valid") {
	using namespace phase4::engine::common;

	SUBCASE("Square H1") {
		std::stringstream ss;
		ss << Square::H1;
		CHECK(ss.str() == "H1");
	}

	SUBCASE("Square D4") {
		std::stringstream ss;
		ss << Square::D4;
		CHECK(ss.str() == "D4");
	}

	SUBCASE("Square G2") {
		std::stringstream ss;
		ss << Square::G2;
		CHECK(ss.str() == "G2");
	}

	SUBCASE("Square A8") {
		std::stringstream ss;
		ss << Square::A8;
		CHECK(ss.str() == "A8");
	}
}

TEST_CASE("Square parsing") {
	using namespace phase4::engine::common;

	CHECK(Square("f2") == Square::F2);
	CHECK(Square("g3") == Square::G3);
	CHECK(Square("e6") == Square::E6);
	CHECK(Square("e7") == Square::E7);
}

TEST_CASE("Square output invalid") {
	using namespace phase4::engine::common;

	SUBCASE("Square with value greater than 63") {
		const Square invalidSquare(64);
		std::stringstream ss;
		ss << invalidSquare;
		CHECK(ss.fail());
	}
}

TEST_CASE("Square from FieldIndex") {
	using namespace phase4::engine::common;

	CHECK(Square(FieldIndex{ 2, 2 }) == Square::C3);
	CHECK(Square(FieldIndex{ 6, 4 }) == Square::G5);
}

TEST_CASE("Square to FieldIndex") {
	using namespace phase4::engine::common;

	CHECK(Square::C3.asFieldIndex() == FieldIndex{ 2, 2 });
	CHECK(Square::G5.asFieldIndex() == FieldIndex{ 6, 4 });
}

TEST_CASE("Square conversion FieldIndex") {
	using namespace phase4::engine::common;

	CHECK(Square(Square::C3.asFieldIndex()) == Square::C3);
	CHECK(Square(Square::G5.asFieldIndex()) == Square::G5);
}

TEST_CASE("Square asBitboard") {
	using namespace phase4::engine::common;

	//                                 ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH
	//                                 88888888 77777777 66666666 55555555 44444444 33333333 22222222 11111111
	CHECK(Square::C3.asBitboard() == 0b00000000'00000000'00000000'00000000'00000000'00100000'00000000'00000000);
	CHECK(Square::G5.asBitboard() == 0b00000000'00000000'00000000'00000010'00000000'00000000'00000000'00000000);
	CHECK(Square::A8.asBitboard() == 0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
}

TEST_CASE("Square increment and compare") {
	using namespace phase4::engine::common;

	Square square = Square::BEGIN;
	CHECK(square == Square::H1);
	++square;
	CHECK(square == Square::G1);
	++square;
	CHECK(square == Square::F1);
	++square;
	CHECK(square == Square::E1);
}

TEST_CASE("Square directions") {
	using namespace phase4::engine::common;

	Square::Direction direction = &Square::north;

	CHECK((Square::D4.*direction)(2) == Square::D6);

	CHECK(Square::C3.north(1) == Square::C4);
	CHECK(Square::C3.east(1) == Square::D3);
	CHECK(Square::C3.south(1) == Square::C2);
	CHECK(Square::C3.west(1) == Square::B3);

	CHECK(Square::E5.north(3) == Square::E8);
	CHECK(Square::E5.east(3) == Square::H5);
	CHECK(Square::E5.south(3) == Square::E2);
	CHECK(Square::E5.west(3) == Square::B5);
}