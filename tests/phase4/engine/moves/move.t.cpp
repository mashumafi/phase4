#include <phase4/engine/moves/move.h>

#include <doctest/doctest.h>

#include <sstream>

TEST_CASE("Move raw constructor") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;

	SUBCASE("Not a promotion") {
		Move move(Square::G2, Square::C4, MoveFlags::QUIET);
		CHECK(move.from() == Square::G2);
		CHECK(move.to() == Square::C4);
		CHECK(move.flags() == MoveFlags::QUIET);

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "G2C4");
	}

	SUBCASE("Promotion") {
		Move move(Square::G2, Square::C4, MoveFlags::QUEEN_PROMOTION);
		CHECK(move.from() == Square::G2);
		CHECK(move.to() == Square::C4);
		CHECK(move.flags() == MoveFlags::QUEEN_PROMOTION);

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "G2C4q");
	}
}

TEST_CASE("Move string constructor") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;

	SUBCASE("Not a promotion") {
		Move move("b2d6");
		CHECK(move.from() == Square::G2);
		CHECK(move.to() == Square::C4);
		CHECK(move.flags() == MoveFlags::QUIET);

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "G2C4");
	}

	SUBCASE("Promotion") {
		Move move("b2d6q");
		CHECK(move.from() == Square::G2);
		CHECK(move.to() == Square::C4);
		CHECK(move.flags() == MoveFlags::QUEEN_PROMOTION);

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "G2C4q");
	}
}
