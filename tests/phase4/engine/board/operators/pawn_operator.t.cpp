#include <phase4/engine/board/operators/pawn_operator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>

#include <doctest/doctest.h>

TEST_CASE("PawnOperator getLoudMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("3q2k1/5pp1/3Np2p/2b1P3/p1p1P3/2B4P/1Q3PP1/6K1 w - - 0 38");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::PawnOperator::getLoudMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 0);
	}

	{
		auto position = fen::FenToPosition::parse("6k1/2R2pp1/4P3/3pK2p/4r3/3r3P/3B2P1/2R5 w - - 1 36");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::PawnOperator::getLoudMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e6f7")));
	}

	{
		auto position = fen::FenToPosition::parse("r1b2bk1/5p1p/3p1nP1/1q2n3/2p1P3/6Q1/PPP1N1PP/R4R1K b - - 0 21");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::PawnOperator::getLoudMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 2);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h7g6")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f7g6")));
	}
}

TEST_CASE("PawnOperator getQuietMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("4nk2/4p2p/3pP1BP/2pP2K1/1pP5/1P6/8/8 b - - 0 48");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::PawnOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 0);
	}

	{
		auto position = fen::FenToPosition::parse("8/8/5p2/2p1q1p1/2P1PkPp/3p4/2r3BP/5Q1K b - - 1 47");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::PawnOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f6f5")));
	}

	{
		auto position = fen::FenToPosition::parse("r1b1r1k1/1p1pPp1p/pqn3p1/2p3NQ/4B3/2B5/PPP2PPP/R3R1K1 b - - 0 16");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::PawnOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 2);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h7h6")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f7f6")));
	}

	{
		auto position = fen::FenToPosition::parse("8/5p2/6k1/p2pp2r/6K1/Pr4P1/1P1R1P2/4R3 w - - 0 45");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::PawnOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 3);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("a3a4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f2f3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f2f4")));
	}

	{
		auto position = fen::FenToPosition::parse("r5k1/5p2/6p1/3R4/1p1p4/1Q5P/2Pq1PPK/8 w - - 0 31");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::PawnOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 4);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f2f3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f2f4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g2g3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g2g4")));
	}

	{
		auto position = fen::FenToPosition::parse("8/R5p1/4ppkp/1p6/1R1P4/2n3P1/5PKP/3r4 w - - 0 34");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::PawnOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 5);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f2f3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f2f3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h2h3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h2h4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d4d5")));
	}

	{
		auto position = fen::FenToPosition::parse("8/6p1/4p1p1/pp1k4/5K1P/1PP5/P5P1/8 w - - 0 34");
		REQUIRE(position);

		moves::Moves moves;
		const common::Bitboard &evasionMask = position->getEvasionMask();
		board::operators::PawnOperator::getQuietMoves(*position, moves, evasionMask);
		CHECK(moves.size() == 6);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("a2a4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("b3b4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("c3c4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g2g3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g2g4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("h4h5")));
	}
}

TEST_CASE("PawnOperator getAvailableCaptureMoves") {
	using namespace phase4::engine;

	{
		auto position = fen::FenToPosition::parse("8/k1q3p1/ppP1p1p1/3p1p2/R2Pn3/N2QK3/PP3P1r/8 b - - 3 36");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::PawnOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 0);
	}

	{
		auto position = fen::FenToPosition::parse("4rrk1/p1q2ppp/1p6/2pP4/2P5/5Nb1/PP1QRP2/R5K1 w - - 0 21");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::PawnOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 1);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("f2g3")));
	}

	{
		auto position = fen::FenToPosition::parse("r1bqr1k1/1ppp1pp1/p6B/2b4p/B2pP3/2NP2Q1/PPP2PPP/R4RK1 b - - 1 13");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::PawnOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 2);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("d4c3")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g7h6")));
	}

	{
		auto position = fen::FenToPosition::parse("r3k1r1/pbp2p2/1p2p3/8/2PP1n1p/4PBPq/P4P1P/R2Q1RK1 w q - 0 19");
		REQUIRE(position);

		moves::Moves moves;
		board::operators::PawnOperator::getAvailableCaptureMoves(*position, moves);
		CHECK(moves.size() == 3);
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("e3f4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g3f4")));
		CHECK(board::PositionMoves::findRealMove(moves, moves::Move("g3h4")));
	}
}

TEST_CASE("PawnOperator isMoveLegal") {
	using namespace phase4::engine;
}
