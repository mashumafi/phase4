#include <phase4/engine/ai/score/evaluators/rook_evaluator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>

#include <doctest/doctest.h>

#include <cstdint>

TEST_CASE("RookEvaluator Double/Open rooks black") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("4r1k1/ppq1rp1p/2p2p2/5p2/3P1b2/2PB1P1P/PPQ2P2/R3R1K1 b - - 6 26");
	REQUIRE(position);

	const int32_t score = ai::score::evaluators::RookEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0);

	const int32_t blackScore = board::EvaluationConstants::ROOK_ON_OPEN_FILE * 2 + board::EvaluationConstants::DOUBLED_ROOKS;
	const int32_t whiteScore = board::EvaluationConstants::ROOK_ON_OPEN_FILE;
	CHECK(score == whiteScore - blackScore);
}

TEST_CASE("RookEvaluator Double/Open rooks black") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("4Q3/2pr1R2/2k5/ppb3P1/8/2P2K2/P2r4/8 b - - 4 43");
	REQUIRE(position);

	const int32_t score = ai::score::evaluators::RookEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0);

	const int32_t blackScore = board::EvaluationConstants::ROOK_ON_OPEN_FILE * 2 + board::EvaluationConstants::DOUBLED_ROOKS;
	const int32_t whiteScore = board::EvaluationConstants::ROOK_ON_OPEN_FILE;
	CHECK(score == whiteScore - blackScore);
}

TEST_CASE("RookEvaluator Open rooks white") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("r3b1rk/1p4p1/pq2N2p/3nPB2/3N4/8/PPP3PP/2KRR3 b - - 0 23");
	REQUIRE(position);

	const int32_t score = ai::score::evaluators::RookEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0);

	const int32_t blackScore = 0;
	const int32_t whiteScore = board::EvaluationConstants::ROOK_ON_OPEN_FILE;
	CHECK(score == whiteScore - blackScore);
}

TEST_CASE("RookEvaluator Double/Open rooks white") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("8/1pk5/p1pp4/4nR2/P3P1q1/1PN1b3/2P1B1P1/5RK1 w - - 4 31");
	REQUIRE(position);

	const int32_t score = ai::score::evaluators::RookEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0);

	const int32_t blackScore = 0;
	const int32_t whiteScore = board::EvaluationConstants::ROOK_ON_OPEN_FILE * 2 + board::EvaluationConstants::DOUBLED_ROOKS;
	CHECK(score == whiteScore - blackScore);
}
