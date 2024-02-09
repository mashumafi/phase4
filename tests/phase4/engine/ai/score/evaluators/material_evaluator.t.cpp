#include <phase4/engine/ai/score/evaluators/material_evaluator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>

#include <phase4/engine/moves/move.h>

#include <doctest/doctest.h>

#include <cstdint>

TEST_CASE("MaterialEvaluator white ahead 700") {
	using namespace phase4::engine;
	using namespace phase4::engine::common;
	using EC = board::EvaluationConstants;
	using namespace phase4::engine::moves::literals;

	auto startingPosition = fen::FenToPosition::parse("r5k1/r1q3p1/4p3/2p5/1pP1PP2/1P6/1KN3QR/2N3R1 w - - 1 34");
	REQUIRE(startingPosition);

	const int32_t beginScore = ai::score::evaluators::MaterialEvaluator::evaluate(*startingPosition);
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

	auto endingPosition = board::PositionMoves::makeMoves(
			*startingPosition,
			moves::Moves({ "f4f5"_m, "c7e5"_m, "b2b1"_m, "a7a1"_m, "c2a1"_m, "a8a1"_m, "b1c2"_m, "e5c3"_m, "c2d1"_m, "a1c1"_m, "d1e2"_m, "c1c2"_m }));
	REQUIRE(endingPosition);

	const int32_t endScore = ai::score::evaluators::MaterialEvaluator::evaluate(*endingPosition);
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
	using namespace phase4::engine::moves::literals;

	auto startingPosition = fen::FenToPosition::parse("2rq1nk1/1p6/pn2p3/4PrbQ/2pP2R1/2N5/PP3P2/5KR1 b - - 2 30");
	REQUIRE(startingPosition);

	const int32_t beginScore = ai::score::evaluators::MaterialEvaluator::evaluate(*startingPosition);
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

	auto endingPosition = board::PositionMoves::makeMoves(
			*startingPosition,
			moves::Moves({ "f8h7"_m, "c3e4"_m, "c8c7"_m, "e4f6"_m, "g8h8"_m, "g4g5"_m, "f5g5"_m, "g1g5"_m, "b6d5"_m, "g5g8"_m, "d8g8"_m, "f6g8"_m }));
	REQUIRE(endingPosition);

	const int32_t endScore = ai::score::evaluators::MaterialEvaluator::evaluate(*endingPosition);
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
