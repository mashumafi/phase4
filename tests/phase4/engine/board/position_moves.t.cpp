#include <phase4/engine/board/position_moves.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/moves/move.h>

#include <doctest/doctest.h>

TEST_CASE("Position moves valid rn2bq2/pp2pp2/2**nrkb/2**pppp/2PP2PP/2BQ2NR/PPPP4/RNKB4 w - - 0 1") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = FenToPosition::parse("rn2bq2/pp2pp2/2**nrkb/2**pppp/2PP2PP/2BQ2NR/PPPP4/RNKB4 w - - 0 1");
	REQUIRE(position);

	Moves moves;
	PositionMoves::getValidMoves(*position, moves);
	CHECK(moves.size() == 28);
	CHECK(PositionMoves::findRealMove(*position, Move("a2a3")));
	CHECK(PositionMoves::findRealMove(*position, Move("a2a4")));

	CHECK(PositionMoves::findRealMove(*position, Move("b2b3")));
	CHECK(PositionMoves::findRealMove(*position, Move("b2b4")));

	CHECK(PositionMoves::findRealMove(*position, Move("b1a3")));

	CHECK(PositionMoves::findRealMove(*position, Move("c3b4")));
	CHECK(PositionMoves::findRealMove(*position, Move("c3a5")));

	CHECK(PositionMoves::findRealMove(*position, Move("d4e5")));

	CHECK(PositionMoves::findRealMove(*position, Move("d3e4")));
	CHECK(PositionMoves::findRealMove(*position, Move("d3e3")));
	CHECK(PositionMoves::findRealMove(*position, Move("d3e2")));
	CHECK(PositionMoves::findRealMove(*position, Move("d3f5")));
	CHECK(PositionMoves::findRealMove(*position, Move("d3f3")));
	CHECK(PositionMoves::findRealMove(*position, Move("d3f1")));

	CHECK(PositionMoves::findRealMove(*position, Move("d1e2")));
	CHECK(PositionMoves::findRealMove(*position, Move("d1f3")));

	CHECK(PositionMoves::findRealMove(*position, Move("g4f5")));
	CHECK(PositionMoves::findRealMove(*position, Move("g4h5")));

	CHECK(PositionMoves::findRealMove(*position, Move("g3e4")));
	CHECK(PositionMoves::findRealMove(*position, Move("g3e2")));
	CHECK(PositionMoves::findRealMove(*position, Move("g3f5")));
	CHECK(PositionMoves::findRealMove(*position, Move("g3f1")));
	CHECK(PositionMoves::findRealMove(*position, Move("g3h5")));
	CHECK(PositionMoves::findRealMove(*position, Move("g3h1")));

	CHECK(PositionMoves::findRealMove(*position, Move("h4g5")));

	CHECK(PositionMoves::findRealMove(*position, Move("h3h2")));
	CHECK(PositionMoves::findRealMove(*position, Move("h3h1")));
}
