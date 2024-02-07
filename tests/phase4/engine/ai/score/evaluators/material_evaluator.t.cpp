#include <phase4/engine/ai/score/evaluators/material_evaluator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>

#include <doctest/doctest.h>

#include <cstdint>

TEST_CASE("MaterialEvaluator white ahead 700") {
	using namespace phase4::engine;
	using namespace phase4::engine::common;
	using EC = board::EvaluationConstants;

	auto position = fen::FenToPosition::parse("r5k1/r1q3p1/4p3/2p5/1pP1PP2/1P6/1KN3QR/2N3R1 w - - 1 34");
	REQUIRE(position);

	const int32_t beginScore = ai::score::evaluators::MaterialEvaluator::evaluate(*position);
	{
		const int32_t blackScore =
				EC::pieceValue(PieceType::PAWN) * 4 + EC::pieceValue(PieceType::KNIGHT) * 0 + EC::pieceValue(PieceType::BISHOP) * 0 +
				EC::pieceValue(PieceType::ROOK) * 2 + EC::pieceValue(PieceType::QUEEN) * 1 + EC::pieceValue(PieceType::KING) * 1;
		const int32_t whiteScore =
				EC::pieceValue(PieceType::PAWN) * 4 + EC::pieceValue(PieceType::KNIGHT) * 2 + EC::pieceValue(PieceType::BISHOP) * 0 +
				EC::pieceValue(PieceType::ROOK) * 2 + EC::pieceValue(PieceType::QUEEN) * 1 + EC::pieceValue(PieceType::KING) * 1;

		CHECK(beginScore == whiteScore - blackScore);
		CHECK(beginScore == 700);
	}

	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("f4f5")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("c7e5")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("b2b1")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("a7a1")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("c2a1")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("a8a1")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("b1c2")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("e5c3")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("c2d1")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("a1c1")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("d1e2")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("c1c2")));

	const int32_t endScore = ai::score::evaluators::MaterialEvaluator::evaluate(*position);
	{
		const int32_t blackScore =
				EC::pieceValue(PieceType::PAWN) * 4 + EC::pieceValue(PieceType::KNIGHT) * 0 + EC::pieceValue(PieceType::BISHOP) * 0 +
				EC::pieceValue(PieceType::ROOK) * 1 + EC::pieceValue(PieceType::QUEEN) * 1 + EC::pieceValue(PieceType::KING) * 1;
		const int32_t whiteScore =
				EC::pieceValue(PieceType::PAWN) * 4 + EC::pieceValue(PieceType::KNIGHT) * 0 + EC::pieceValue(PieceType::BISHOP) * 0 +
				EC::pieceValue(PieceType::ROOK) * 2 + EC::pieceValue(PieceType::QUEEN) * 1 + EC::pieceValue(PieceType::KING) * 1;

		CHECK(endScore == whiteScore - blackScore);
		CHECK(endScore == 570);
	}
	CHECK(beginScore > endScore); // black got better
}

TEST_CASE("MaterialEvaluator black ahead -620") {
	using namespace phase4::engine;
	using namespace phase4::engine::common;
	using EC = board::EvaluationConstants;

	auto position = fen::FenToPosition::parse("2rq1nk1/1p6/pn2p3/4PrbQ/2pP2R1/2N5/PP3P2/5KR1 b - - 2 30");
	REQUIRE(position);

	const int32_t beginScore = ai::score::evaluators::MaterialEvaluator::evaluate(*position);
	{
		const int32_t blackScore =
				EC::pieceValue(PieceType::PAWN) * 4 + EC::pieceValue(PieceType::KNIGHT) * 2 + EC::pieceValue(PieceType::BISHOP) * 1 +
				EC::pieceValue(PieceType::ROOK) * 2 + EC::pieceValue(PieceType::QUEEN) * 1 + EC::pieceValue(PieceType::KING) * 1;
		const int32_t whiteScore =
				EC::pieceValue(PieceType::PAWN) * 5 + EC::pieceValue(PieceType::KNIGHT) * 1 + EC::pieceValue(PieceType::BISHOP) * 0 +
				EC::pieceValue(PieceType::ROOK) * 2 + EC::pieceValue(PieceType::QUEEN) * 1 + EC::pieceValue(PieceType::KING) * 1;

		CHECK(beginScore == whiteScore - blackScore);
		CHECK(beginScore == -620);
	}

	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("f8h7")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("c3e4")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("c8c7")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("e4f6")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("g8h8")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("g4g5")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("f5g5")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("g1g5")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("b6d5")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("g5g8")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("d8g8")));
	board::PositionMoves::makeMove(*position, *board::PositionMoves::findRealMove(*position, moves::Move("f6g8")));

	const int32_t endScore = ai::score::evaluators::MaterialEvaluator::evaluate(*position);
	{
		const int32_t blackScore =
				EC::pieceValue(PieceType::PAWN) * 4 + EC::pieceValue(PieceType::KNIGHT) * 2 + EC::pieceValue(PieceType::BISHOP) * 0 +
				EC::pieceValue(PieceType::ROOK) * 1 + EC::pieceValue(PieceType::QUEEN) * 0 + EC::pieceValue(PieceType::KING) * 1;
		const int32_t whiteScore =
				EC::pieceValue(PieceType::PAWN) * 5 + EC::pieceValue(PieceType::KNIGHT) * 1 + EC::pieceValue(PieceType::BISHOP) * 0 +
				EC::pieceValue(PieceType::ROOK) * 0 + EC::pieceValue(PieceType::QUEEN) * 1 + EC::pieceValue(PieceType::KING) * 1;

		CHECK(endScore == whiteScore - blackScore);
		CHECK(endScore == 370);
	}

	CHECK(beginScore < endScore); // white got better
}
