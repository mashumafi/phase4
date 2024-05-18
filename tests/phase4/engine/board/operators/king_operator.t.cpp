#include <phase4/engine/board/operators/king_operator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>

#include <doctest/doctest.h>

TEST_CASE("KingOperator getLoudMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("8/r6p/Pk2ppp1/8/3K1P2/8/R4P1P/8 b - - 2 38");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::KingOperator::getLoudMoves(*position, moves);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("b6a6")));
	}
}

TEST_CASE("KingOperator getQuietMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("r2q1r1k/1ppb2pp/p4pn1/2b1pP2/2B1P3/1PN3QR/PBn3PP/R6K b - - 1 20");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::KingOperator::getQuietMoves(*position, moves);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h8g8")));
	}
}

TEST_CASE("KingOperator getAvailableCaptureMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("r4Bk1/1p6/p1pb3p/7q/1P1PQp2/P7/5PPP/5RK1 b - - 0 25");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::KingOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g8f8")));
	}
}

TEST_CASE("KingOperator isMoveLegal") {
	using namespace phase4::engine;
}
