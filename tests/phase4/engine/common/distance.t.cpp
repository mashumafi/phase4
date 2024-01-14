#include <phase4/engine/common/distance.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("Distance lookup") {
	using namespace phase4::engine::common;

	CHECK(Distance::lookup(Square::A4, Square::A4) == 0);
	CHECK(Distance::lookup(Square::B1, Square::B8) == 7);
	CHECK(Distance::lookup(Square::A7, Square::H8) == 7);
	CHECK(Distance::lookup(Square::A3, Square::F6) == 5);
	CHECK(Distance::lookup(Square::A2, Square::D7) == 5);
}
