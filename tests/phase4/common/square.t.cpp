#include <phase4/common/square.h>

#include <sstream>

#include <doctest/doctest.h>

TEST_CASE("Square output valid") {
	using namespace phase4::common;

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
	using namespace phase4::common;

	SUBCASE("Square with negative value") {
		const Square invalidSquare(-1);
		std::stringstream ss;
		ss << invalidSquare;
		CHECK(ss.fail());
	}

	SUBCASE("Square with value greater than 63") {
		const Square invalidSquare(64);
		std::stringstream ss;
		ss << invalidSquare;
		CHECK(ss.fail());
	}
}
