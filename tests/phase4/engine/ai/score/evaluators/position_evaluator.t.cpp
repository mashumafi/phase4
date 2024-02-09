#include <phase4/engine/ai/score/evaluators/position_evaluator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/piece_square_tables/piece_square_table_data.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>

#include <phase4/engine/moves/move.h>

#include <doctest/doctest.h>

#include <cstdint>

TEST_CASE("PositionEvaluator white ahead (110,-45)") {
	using namespace phase4::engine;
	using namespace phase4::engine::common;
	using PSTD = board::piece_square_tables::PieceSquareTablesData;
	using namespace phase4::engine::moves::literals;

	auto startingPosition = fen::FenToPosition::parse("rn1q1bnr/pbpp4/1p2k2p/4N1p1/3PPp2/2N5/PPP3PP/R1BQK2R b KQ - 0 9");
	REQUIRE(startingPosition);

	const int32_t beginScore = ai::score::evaluators::PositionEvaluator::evaluate(*startingPosition, common::PositionConstants::PHASE_RESOLUTION, 0);
	{
		const int32_t blackScore =
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::A7) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::B6) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::C7) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::D7) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::F4) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::G5) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::H6) +
				PSTD::value(PieceType::KNIGHT, PieceColor::BLACK, GamePhase::OPENING, Square::B8) +
				PSTD::value(PieceType::KNIGHT, PieceColor::BLACK, GamePhase::OPENING, Square::G8) +
				PSTD::value(PieceType::BISHOP, PieceColor::BLACK, GamePhase::OPENING, Square::B7) +
				PSTD::value(PieceType::BISHOP, PieceColor::BLACK, GamePhase::OPENING, Square::F8) +
				PSTD::value(PieceType::ROOK, PieceColor::BLACK, GamePhase::OPENING, Square::A8) +
				PSTD::value(PieceType::ROOK, PieceColor::BLACK, GamePhase::OPENING, Square::H8) +
				PSTD::value(PieceType::QUEEN, PieceColor::BLACK, GamePhase::OPENING, Square::D8) +
				PSTD::value(PieceType::KING, PieceColor::BLACK, GamePhase::OPENING, Square::E6);
		const int32_t whiteScore =
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::A2) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::B2) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::C2) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::D4) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::E4) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::G2) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::H2) +
				PSTD::value(PieceType::KNIGHT, PieceColor::WHITE, GamePhase::OPENING, Square::C3) +
				PSTD::value(PieceType::KNIGHT, PieceColor::WHITE, GamePhase::OPENING, Square::E5) +
				PSTD::value(PieceType::BISHOP, PieceColor::WHITE, GamePhase::OPENING, Square::C1) +
				PSTD::value(PieceType::ROOK, PieceColor::WHITE, GamePhase::OPENING, Square::A1) +
				PSTD::value(PieceType::ROOK, PieceColor::WHITE, GamePhase::OPENING, Square::H1) +
				PSTD::value(PieceType::QUEEN, PieceColor::WHITE, GamePhase::OPENING, Square::D1) +
				PSTD::value(PieceType::KING, PieceColor::WHITE, GamePhase::OPENING, Square::E1);

		CHECK(beginScore == whiteScore - blackScore);
	}

	auto endingPosition = board::PositionMoves::makeMoves(
			*startingPosition,
			moves::Moves({ "d7d6"_m, "d1g4"_m, "e6e7"_m, "c3d5"_m, "b7d5"_m, "e4d5"_m, "d8e8"_m, "g4e6"_m, "e7d8"_m, "e5f7"_m, "e8f7"_m, "e6f7"_m }));
	REQUIRE(endingPosition);

	const int32_t endScore = ai::score::evaluators::PositionEvaluator::evaluate(*endingPosition, 0, common::PositionConstants::PHASE_RESOLUTION);
	{
		const int32_t blackScore =
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::A7) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::B6) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::C7) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::D6) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::F4) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::G5) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::H6) +
				PSTD::value(PieceType::KNIGHT, PieceColor::BLACK, GamePhase::ENDING, Square::B8) +
				PSTD::value(PieceType::KNIGHT, PieceColor::BLACK, GamePhase::ENDING, Square::G8) +
				PSTD::value(PieceType::BISHOP, PieceColor::BLACK, GamePhase::ENDING, Square::F8) +
				PSTD::value(PieceType::ROOK, PieceColor::BLACK, GamePhase::ENDING, Square::A8) +
				PSTD::value(PieceType::ROOK, PieceColor::BLACK, GamePhase::ENDING, Square::H8) +
				PSTD::value(PieceType::KING, PieceColor::BLACK, GamePhase::ENDING, Square::D8);
		const int32_t whiteScore =
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::A2) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::B2) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::C2) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::D4) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::D5) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::G2) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::H2) +
				PSTD::value(PieceType::BISHOP, PieceColor::WHITE, GamePhase::ENDING, Square::C1) +
				PSTD::value(PieceType::ROOK, PieceColor::WHITE, GamePhase::ENDING, Square::A1) +
				PSTD::value(PieceType::ROOK, PieceColor::WHITE, GamePhase::ENDING, Square::H1) +
				PSTD::value(PieceType::QUEEN, PieceColor::WHITE, GamePhase::ENDING, Square::F7) +
				PSTD::value(PieceType::KING, PieceColor::WHITE, GamePhase::ENDING, Square::E1);

		CHECK(endScore == whiteScore - blackScore);
	}
	CHECK(beginScore > endScore); // black got better
}

TEST_CASE("PositionEvaluator black ahead (-30,205)") {
	using namespace phase4::engine;
	using namespace phase4::engine::common;
	using PSTD = board::piece_square_tables::PieceSquareTablesData;
	using namespace phase4::engine::moves::literals;

	auto startingPosition = fen::FenToPosition::parse("4r1k1/1p1n1p1p/p3r1p1/2PqP1P1/3B2QP/P4P2/4R2K/4R3 b - - 0 38");
	REQUIRE(startingPosition);

	const int32_t beginScore = ai::score::evaluators::PositionEvaluator::evaluate(*startingPosition, common::PositionConstants::PHASE_RESOLUTION, 0);
	{
		const int32_t blackScore =
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::A6) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::B7) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::F7) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::G6) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::OPENING, Square::H7) +
				PSTD::value(PieceType::KNIGHT, PieceColor::BLACK, GamePhase::OPENING, Square::D7) +
				PSTD::value(PieceType::ROOK, PieceColor::BLACK, GamePhase::OPENING, Square::E8) +
				PSTD::value(PieceType::ROOK, PieceColor::BLACK, GamePhase::OPENING, Square::E6) +
				PSTD::value(PieceType::QUEEN, PieceColor::BLACK, GamePhase::OPENING, Square::D5) +
				PSTD::value(PieceType::KING, PieceColor::BLACK, GamePhase::OPENING, Square::G8);
		const int32_t whiteScore =
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::A3) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::C5) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::E5) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::F3) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::G5) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::OPENING, Square::H4) +
				PSTD::value(PieceType::BISHOP, PieceColor::WHITE, GamePhase::OPENING, Square::D4) +
				PSTD::value(PieceType::ROOK, PieceColor::WHITE, GamePhase::OPENING, Square::E2) +
				PSTD::value(PieceType::ROOK, PieceColor::WHITE, GamePhase::OPENING, Square::E1) +
				PSTD::value(PieceType::QUEEN, PieceColor::WHITE, GamePhase::OPENING, Square::G4) +
				PSTD::value(PieceType::KING, PieceColor::WHITE, GamePhase::OPENING, Square::H2);

		CHECK(beginScore == whiteScore - blackScore);
	}

	auto endingPosition = board::PositionMoves::makeMoves(
			*startingPosition,
			moves::Moves({ "d7e5"_m, "e2e5"_m, "e6e5"_m, "e1e5"_m, "e8e5"_m, "g4c8"_m, "g8g7"_m, "d4c3"_m, "f7f6"_m, "g5f6"_m, "g7f6"_m, "f3f4"_m, "f6f7"_m, "f4e5"_m }));
	REQUIRE(endingPosition);

	const int32_t endScore = ai::score::evaluators::PositionEvaluator::evaluate(*endingPosition, 0, common::PositionConstants::PHASE_RESOLUTION);
	{
		const int32_t blackScore =
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::A6) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::B7) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::G6) +
				PSTD::value(PieceType::PAWN, PieceColor::BLACK, GamePhase::ENDING, Square::H7) +
				PSTD::value(PieceType::QUEEN, PieceColor::BLACK, GamePhase::ENDING, Square::D5) +
				PSTD::value(PieceType::KING, PieceColor::BLACK, GamePhase::ENDING, Square::F7);
		const int32_t whiteScore =
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::A3) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::C5) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::E5) +
				PSTD::value(PieceType::PAWN, PieceColor::WHITE, GamePhase::ENDING, Square::H4) +
				PSTD::value(PieceType::BISHOP, PieceColor::WHITE, GamePhase::ENDING, Square::C3) +
				PSTD::value(PieceType::QUEEN, PieceColor::WHITE, GamePhase::ENDING, Square::C8) +
				PSTD::value(PieceType::KING, PieceColor::WHITE, GamePhase::ENDING, Square::H2);

		CHECK(endScore == whiteScore - blackScore);
	}

	CHECK(beginScore < endScore); // white got better
}
