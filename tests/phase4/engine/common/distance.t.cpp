#include <phase4/engine/common/distance.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("Distance lookup") {
	using namespace phase4::engine::common;

	CHECK(Distance::lookup(Square::E1, Square::E1) == 0);
	CHECK(Distance::lookup(Square::H2, Square::A2) == 7);
	CHECK(Distance::lookup(Square::B1, Square::A8) == 7);
	CHECK(Distance::lookup(Square::F1, Square::C6) == 5);
	CHECK(Distance::lookup(Square::G1, Square::B4) == 5);
}
