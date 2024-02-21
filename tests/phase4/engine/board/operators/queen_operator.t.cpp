#include <phase4/engine/board/operators/queen_operator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>

#include <doctest/doctest.h>

TEST_CASE("QueenOperator getLoudMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("8/1Q1b1k2/p2q3p/2p2p2/2P3p1/1P2N1P1/5P1P/6K1 w - - 0 38");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getLoudMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 0);
	}

	{
		auto position = fen::FenToPosition::parse("3q1r2/p4p1k/3p1Rpb/2p2B1N/2p1P2P/3P1KQ1/Pr4P1/8 b - - 3 28");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getLoudMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d8f6")));
	}

	{
		auto position = fen::FenToPosition::parse("r5k1/2p3p1/p1pp2pp/4p3/4P1Pq/3P3n/PPP1N1KN/R3Qr2 w - - 0 22");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getLoudMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 2);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e1f1")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e1h4")));
	}
}

TEST_CASE("QueenOperator getQuietMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("1r3qk1/2Q3pp/8/4N3/3Pn3/4P3/P4P1P/2R3K1 w - - 11 34");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 0);
	}

	{
		auto position = fen::FenToPosition::parse("1r2r3/p1pkbR1Q/1p2p1p1/3p2q1/8/B1p5/P1P3PP/1R4K1 w - - 0 24");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h7h3")));
	}

	{
		auto position = fen::FenToPosition::parse("4r3/2r1pk1p/3pR1pP/2qP1pP1/pp3P2/8/PPPQ4/1K1R4 w - - 0 31");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 2);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d2c1")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d2c3")));
	}

	{
		auto position = fen::FenToPosition::parse("1r6/2R2p2/4p1k1/4Q1p1/P7/8/5PPP/6K1 w - - 1 39");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 3);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e5a1")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e5e1")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e5e2")));
	}

	{
		auto position = fen::FenToPosition::parse("4r1k1/p4ppp/8/2Nn4/R2P4/3QB1PP/1q6/6K1 w - - 2 28");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 4);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d3b1")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d3d1")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d3f1")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d3e2")));
	}

	{
		auto position = fen::FenToPosition::parse("8/8/5Kp1/6P1/p7/8/k7/1q4Q1 w - - 0 73");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 5);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g1f1")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g1f2")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g1d4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g1g4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g1b6")));
	}

	{
		auto position = fen::FenToPosition::parse("2b1k2r/1pp3pp/1pnp1q2/1Q2p3/4P3/5N2/rPP2PPP/2KR1B1R b k - 1 12");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 6);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f6f8")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f6d8")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f6f7")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f6e7")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f6g6")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f6e6")));
	}

	{
		auto position = fen::FenToPosition::parse("5r2/1n2k3/3pp3/7Q/1P6/2PP2q1/P7/2R1RK2 w - - 2 36");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::QueenOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 7);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h5h1")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h5h2")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h5e2")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h5h3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h5f3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h5f5")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h5f7")));
	}
}

TEST_CASE("QueenOperator getAvailableCaptureMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("r2q1rk1/ppp2pp1/1b5p/8/3n1RQ1/1B1P3P/PPP3PN/R6K b - - 0 16");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::QueenOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 0);
	}

	{
		auto position = fen::FenToPosition::parse("r1b2rk1/p4p1p/1pqb1R2/2p5/8/4B3/P1PQ2PP/1R5K b - - 0 22");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::QueenOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("c6g2")));
	}

	{
		auto position = fen::FenToPosition::parse("1k5r/pp6/3qp1Q1/3N4/3np3/6P1/PP3P2/R4RK1 w - - 0 27");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::QueenOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 2);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g6e4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g6e6")));
	}

	{
		auto position = fen::FenToPosition::parse("2r2r1k/p3R1p1/2p1R1qp/1p6/3Q1p2/2P5/P1P2PPP/6K1 b - - 1 24");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::QueenOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 3);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g6g2")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g6c2")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g6e6")));
	}

	{
		auto position = fen::FenToPosition::parse("r3r3/pp3qk1/5bp1/4p2p/1n1pQ2P/1N1B4/PPP3P1/4RRK1 w - - 2 22");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::QueenOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 4);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e4d4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e4b7")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e4e5")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e4g6")));
	}

	{
		auto position = fen::FenToPosition::parse("N1bk4/1p6/p2pp3/6q1/2PnQ2p/3Bn1p1/PP4P1/R3K2R w KQ - 1 25");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::QueenOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 5);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e4e6")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e4h4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e4e3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e4d4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e4b7")));
	}
}

TEST_CASE("QueenOperator getMobility") {
	using namespace phase4::engine;
}

TEST_CASE("QueenOperator isMoveLegal") {
	using namespace phase4::engine;
}
