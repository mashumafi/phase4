#include <phase4/engine/moves/move.h>

#include <sstream>

#include <doctest/doctest.h>

TEST_CASE("Move raw constructor") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;

	SUBCASE("Not a promotion") {
		Move move(Square::B2, Square::D6, MoveFlags::QUIET);
		CHECK(move.from() == Square::B2);
		CHECK(move.to() == Square::D6);
		CHECK(move.flags() == MoveFlags::QUIET);

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "B2D6");
	}

	SUBCASE("Promotion") {
		Move move(Square::B2, Square::D6, MoveFlags::QUEEN_PROMOTION);
		CHECK(move.from() == Square::B2);
		CHECK(move.to() == Square::D6);
		CHECK(move.flags() == MoveFlags::QUEEN_PROMOTION);

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "B2D6q");
	}
}

TEST_CASE("Move string constructor") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;

	SUBCASE("Not a promotion") {
		Move move("b2d6");
		CHECK(move.from() == Square::B2);
		CHECK(move.to() == Square::D6);
		CHECK(move.flags() == MoveFlags::QUIET);

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "B2D6");
	}

	SUBCASE("Promotion") {
		Move move("b2d6q");
		CHECK(move.from() == Square::B2);
		CHECK(move.to() == Square::D6);
		CHECK(move.flags() == MoveFlags::QUEEN_PROMOTION);

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "B2D6q");
	}
}
