#include <phase4/engine/common/castling.h>

#include <sstream>

#include <doctest/doctest.h>

TEST_CASE("Castling::EVERYTHING is set correctly") {
	using namespace phase4::engine::common;
	CHECK(Castling::EVERYTHING == (Castling::WHITE_SHORT | Castling::WHITE_LONG | Castling::BLACK_SHORT | Castling::BLACK_LONG));
}

TEST_CASE("Castling output of mixed flags") {
	using namespace phase4::engine::common;

	std::stringstream ss;
	ss << static_cast<Castling>(Castling::WHITE_SHORT | Castling::BLACK_LONG);
	CHECK(ss.str() == "WHITE_SHORT | BLACK_LONG");
}
