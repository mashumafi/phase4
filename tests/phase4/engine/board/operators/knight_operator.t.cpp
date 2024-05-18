#include <phase4/engine/board/operators/knight_operator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>

#include <doctest/doctest.h>

TEST_CASE("KnightOperator getLoudMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("8/7p/N1n3p1/1p1k1p2/1P3P1P/3K2P1/8/8 b - - 4 45");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::KnightOperator::getLoudMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("c6b4")));
	}
}

TEST_CASE("KnightOperator getQuietMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("r4rk1/2Rnqpp1/p3p2p/3bP2Q/3PN3/3B4/5PPP/1R4K1 b - - 9 25");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::KnightOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d7f6")));
	}
}

TEST_CASE("KnightOperator getAvailableCaptureMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("4r1k1/ppp5/2nb2p1/3p1r1p/3P4/1BP2N1Q/PP3RPP/4q1K1 w - - 0 21");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::KnightOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f3e1")));
	}
}

TEST_CASE("KnightOperator getMobility") {
	using namespace phase4::engine;
}

TEST_CASE("KnightOperator isMoveLegal") {
	using namespace phase4::engine;
}
