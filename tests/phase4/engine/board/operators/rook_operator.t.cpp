#include <phase4/engine/board/operators/rook_operator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>

#include <doctest/doctest.h>

TEST_CASE("RookOperator getLoudMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("5r1k/4r1N1/p5Qp/1p1B4/3b1P2/5R1P/6PK/4q3 b - - 0 41");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::RookOperator::getLoudMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e7g7")));
	}
}

TEST_CASE("RookOperator getQuietMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("6k1/pp3ppp/4p3/4P3/2R2PPP/2K5/3r4/8 b - - 2 34");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::RookOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d2d8")));
	}
}

TEST_CASE("RookOperator getAvailableCaptureMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("5k2/pp3B2/2p2K2/7P/2N5/2r2n2/P7/8 b - - 0 34");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::RookOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("c3c4")));
	}
}

TEST_CASE("RookOperator getMobility") {
	using namespace phase4::engine;
}

TEST_CASE("RookOperator isMoveLegal") {
	using namespace phase4::engine;
}
