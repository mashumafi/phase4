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
		CHECK(ss.str() == "g2c4");
	}

	SUBCASE("Promotion") {
		Move move(Square::G2, Square::C4, MoveFlags::QUEEN_PROMOTION);
		CHECK(move.from() == Square::G2);
		CHECK(move.to() == Square::C4);
		CHECK(move.flags() == MoveFlags::QUEEN_PROMOTION);

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "g2c4q");
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
		CHECK(ss.str() == "b2d6");
	}

	SUBCASE("Promotion Queen") {
		Move move("b2d6q");
		CHECK(move.from() == Square::B2);
		CHECK(move.to() == Square::D6);
		CHECK(move.flags() == MoveFlags::QUEEN_PROMOTION);
		CHECK(move.flags().isPromotion());

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "b2d6q");
	}

	SUBCASE("Promotion Knight") {
		Move move("c2c1n");
		CHECK(move.from() == Square::C2);
		CHECK(move.to() == Square::C1);
		CHECK(move.flags() == MoveFlags::KNIGHT_PROMOTION);
		CHECK(move.flags().isPromotion());

		std::ostringstream ss;
		ss << move;
		CHECK(ss.str() == "c2c1n");
	}
}

TEST_CASE("Move as uci notation") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;

	using namespace std::string_literals;

	CHECK(Move(Square::A1, Square::B2, MoveFlags::QUIET).asUciNotation().data() == "a1b2"s);
	CHECK(Move(Square::A1, Square::B2, MoveFlags::ROOK_PROMOTION).asUciNotation().data() == "a1b2r"s);
}