#include <phase4/engine/board/position_moves.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/fen/fen_to_position.h>
#include <phase4/engine/fen/position_to_fen.h>

#include <phase4/engine/moves/move.h>

#include <doctest/doctest.h>

TEST_CASE("Position moves valid default") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = PositionState::DEFAULT;

	Moves moves;
	PositionMoves::getValidMoves(position, moves);
	CHECK(moves.size() == 20);
	CHECK(PositionMoves::findRealMove(position, Move("a2a3")));
	CHECK(PositionMoves::findRealMove(position, Move("a2a4")));

	CHECK(PositionMoves::findRealMove(position, Move("b2b3")));
	CHECK(PositionMoves::findRealMove(position, Move("b2b4")));

	CHECK(PositionMoves::findRealMove(position, Move("c2c3")));
	CHECK(PositionMoves::findRealMove(position, Move("c2c4")));

	CHECK(PositionMoves::findRealMove(position, Move("d2d3")));
	CHECK(PositionMoves::findRealMove(position, Move("d2d4")));

	CHECK(PositionMoves::findRealMove(position, Move("e2e3")));
	CHECK(PositionMoves::findRealMove(position, Move("e2e4")));

	CHECK(PositionMoves::findRealMove(position, Move("f2f3")));
	CHECK(PositionMoves::findRealMove(position, Move("f2f4")));

	CHECK(PositionMoves::findRealMove(position, Move("g2g3")));
	CHECK(PositionMoves::findRealMove(position, Move("g2g4")));

	CHECK(PositionMoves::findRealMove(position, Move("h2h3")));
	CHECK(PositionMoves::findRealMove(position, Move("h2h4")));

	CHECK(PositionMoves::findRealMove(position, Move("g1h3")));
	CHECK(PositionMoves::findRealMove(position, Move("g1f3")));

	CHECK(PositionMoves::findRealMove(position, Move("b1c3")));
	CHECK(PositionMoves::findRealMove(position, Move("b1a3")));
}

TEST_CASE("Position moves valid rn2bq2/pp2pp2/2**nrkb/2**pppp/2PP2PP/2BQ2NR/PPPP4/RNKB4 w - - 0 1") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = FenToPosition::parse("rn2bq2/pp2pp2/2**nrkb/2**pppp/2PP2PP/2BQ2NR/PPPP4/RNKB4 w - - 0 1");
	REQUIRE(position);

	Moves moves;
	PositionMoves::getValidMoves(*position, moves);
	CHECK(moves.size() == 27);
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

TEST_CASE("Position moves valid rnbqkbnr/pppppppp/8/8/2**4/2**4/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = FenToPosition::parse("rnbqkbnr/pppppppp/8/8/2**4/2**4/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	REQUIRE(position);
	CHECK(position->walls() != 0);
	CHECK((position->occupancySummary() & phase4::engine::common::Square::C3.asBitboard()) != 0);
	CHECK((position->occupancySummary() & phase4::engine::common::Square::C4.asBitboard()) != 0);
	CHECK((position->occupancySummary() & phase4::engine::common::Square::D3.asBitboard()) != 0);
	CHECK((position->occupancySummary() & phase4::engine::common::Square::D4.asBitboard()) != 0);

	Moves moves;
	PositionMoves::getValidMoves(*position, moves);
	CHECK(moves.size() == 15);
	CHECK(PositionMoves::findRealMove(*position, Move("a2a3")));
	CHECK(PositionMoves::findRealMove(*position, Move("a2a4")));

	CHECK(PositionMoves::findRealMove(*position, Move("b2b3")));
	CHECK(PositionMoves::findRealMove(*position, Move("b2b4")));

	CHECK(PositionMoves::findRealMove(*position, Move("e2e3")));
	CHECK(PositionMoves::findRealMove(*position, Move("e2e4")));

	CHECK(PositionMoves::findRealMove(*position, Move("f2f3")));
	CHECK(PositionMoves::findRealMove(*position, Move("f2f4")));

	CHECK(PositionMoves::findRealMove(*position, Move("g2g3")));
	CHECK(PositionMoves::findRealMove(*position, Move("g2g4")));

	CHECK(PositionMoves::findRealMove(*position, Move("h2h3")));
	CHECK(PositionMoves::findRealMove(*position, Move("h2h4")));

	CHECK(PositionMoves::findRealMove(*position, Move("g1h3")));
	CHECK(PositionMoves::findRealMove(*position, Move("g1f3")));

	CHECK(PositionMoves::findRealMove(*position, Move("b1a3")));
}

TEST_CASE("Position moves valid rnbqkbnr/2p2p2/1p2p1p1/p2p3p/P2P3P/1P2P1P1/2P2P2/RNBQKBNR w KQkq d6 0 7") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = FenToPosition::parse("rnbqkbnr/2p2p2/1p2p1p1/p2p3p/P2P3P/1P2P1P1/2P2P2/RNBQKBNR w KQkq d6 0 7");
	REQUIRE(position);

	Moves moves;
	PositionMoves::getValidMoves(*position, moves);
	CHECK(moves.size() == 35);

	// Black rook
	CHECK(PositionMoves::findRealMove(*position, Move("a1a2")));
	CHECK(PositionMoves::findRealMove(*position, Move("a1a3")));

	// White rook
	CHECK(PositionMoves::findRealMove(*position, Move("h1h2")));
	CHECK(PositionMoves::findRealMove(*position, Move("h1h3")));

	// Black bishop
	CHECK(PositionMoves::findRealMove(*position, Move("c1a3")));
	CHECK(PositionMoves::findRealMove(*position, Move("c1b2")));
	CHECK(PositionMoves::findRealMove(*position, Move("c1d2")));

	// White bishop
	CHECK(PositionMoves::findRealMove(*position, Move("f1a6")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1b5")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1c4")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1d3")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1e2")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1g2")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1h3")));
}

TEST_CASE("Position moves valid rnbqkbnr/pppp3p/7P/4p1P1/3P2**/4pN**/PPPPPR2/RNBQKB2 b Qkq - 1 6") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = FenToPosition::parse("rnbqkbnr/pppp3p/7P/4p1P1/3P2**/4pN**/PPPPPR2/RNBQKB2 b Qkq - 1 6");
	REQUIRE(position);

	{
		Moves moves;
		PositionMoves::getValidMoves(*position, moves);
		CHECK(moves.size() == 29);

		CHECK(PositionMoves::findRealMove(moves, Move("e3d2")));
		CHECK(PositionMoves::findRealMove(moves, Move("e3f2")));

		auto queenMove = PositionMoves::findRealMove(moves, Move("d8g5"));
		REQUIRE(queenMove);
		CHECK(queenMove->flags().isCapture());
		PositionMoves::makeMove(*position, *queenMove);
	}

	{
		Moves moves;
		PositionMoves::getValidMoves(*position, moves);
		CHECK(moves.size() == 17);

		CHECK_FALSE(PositionMoves::findRealMove(moves, Move("f2g2")));
		CHECK_FALSE(PositionMoves::findRealMove(moves, Move("f2h2")));
	}
}
