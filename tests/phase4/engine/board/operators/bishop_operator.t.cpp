#include <phase4/engine/board/operators/bishop_operator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>

#include <doctest/doctest.h>

TEST_CASE("BishopOperator getLoudMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("r1b1kb1r/p1pN1ppp/5n2/8/P3q3/1P6/3P1PPP/R1BQ1RK1 b kq - 0 12");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::BishopOperator::getLoudMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("c8d7")));
	}
}

TEST_CASE("BishopOperator getQuietMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("2r2rk1/ppq1ppB1/3pb1pp/8/4P1PP/n4P2/PPP1N1B1/1K1RQ2R w - - 1 18");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::BishopOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g7c3")));
	}
}

TEST_CASE("BishopOperator getAvailableCaptureMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("8/1pR2Rpk/p3p2p/3rb3/P5B1/1r1P3P/5PP1/6K1 w - - 0 31");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::BishopOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g4e6")));
	}
}

TEST_CASE("BishopOperator getMobility") {
	using namespace phase4::engine;
}

TEST_CASE("BishopOperator isMoveLegal") {
	using namespace phase4::engine;
}
