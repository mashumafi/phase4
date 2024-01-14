#include <phase4/engine/common/piece_color.h>

#include <sstream>

#include <doctest/doctest.h>

TEST_CASE("PieceColor output is WHITE") {
	using namespace phase4::engine::common;

	std::stringstream ss;
	ss << PieceColor::WHITE;
	CHECK(ss.str() == "WHITE");
}

TEST_CASE("PieceColor output is BLACK") {
	using namespace phase4::engine::common;

	std::stringstream ss;
	ss << PieceColor::BLACK;
	CHECK(ss.str() == "BLACK");
}

TEST_CASE("PieceColor invert") {
	using namespace phase4::engine::common;

	CHECK(PieceColor::WHITE.invert() == PieceColor::BLACK);
	CHECK(PieceColor::BLACK.invert() == PieceColor::WHITE);
}
