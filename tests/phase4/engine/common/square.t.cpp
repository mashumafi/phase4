#include <phase4/engine/common/square.h>

#include <sstream>

#include <doctest/doctest.h>

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

	//                                 HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
	//                                 87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
	CHECK(Square::C3.asBitboard() == 0b00000000'00000000'00000000'00000000'00000000'00100000'00000000'00000000);
	CHECK(Square::G5.asBitboard() == 0b00000000'00000000'00000000'00000010'00000000'00000000'00000000'00000000);
	CHECK(Square::A8.asBitboard() == 0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000);
}
