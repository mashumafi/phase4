#include <phase4/engine/common/square.h>

#include <sstream>

#include <doctest/doctest.h>

TEST_CASE("Square output valid") {
	using namespace phase4::engine::common;

	SUBCASE("Square A1") {
		std::stringstream ss;
		ss << Square::A1;
		CHECK(ss.str() == "A1");
	}

	SUBCASE("Square D5") {
		std::stringstream ss;
		ss << Square::D5;
		CHECK(ss.str() == "D5");
	}

	SUBCASE("Square B7") {
		std::stringstream ss;
		ss << Square::B7;
		CHECK(ss.str() == "B7");
	}

	SUBCASE("Square H8") {
		std::stringstream ss;
		ss << Square::H8;
		CHECK(ss.str() == "H8");
	}
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

	CHECK(Square(FieldIndex{ 2, 2 }) == Square::C6);
	CHECK(Square(FieldIndex{ 6, 4 }) == Square::E2);
}

TEST_CASE("Square to FieldIndex") {
	using namespace phase4::engine::common;

	CHECK(Square::C6.asFieldIndex() == FieldIndex{ 2, 2 });
	CHECK(Square::E2.asFieldIndex() == FieldIndex{ 6, 4 });
}

TEST_CASE("Square conversion FieldIndex") {
	using namespace phase4::engine::common;

	CHECK(Square(Square::C6.asFieldIndex()) == Square::C6);
	CHECK(Square(Square::E2.asFieldIndex()) == Square::E2);
}

TEST_CASE("Square asBitboard") {
	using namespace phase4::engine::common;

	//                                 HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
	//                                 87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
	CHECK(Square::C6.asBitboard() == 0b00000000'00000000'00000000'00000000'00000000'00100000'00000000'00000000);
	CHECK(Square::E2.asBitboard() == 0b00000000'00000000'00000000'00000010'00000000'00000000'00000000'00000000);
	CHECK(Square::H8.asBitboard() == 0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
}
