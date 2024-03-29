#include <phase4/engine/ai/score/evaluators/bishop_evaluator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>

#include <doctest/doctest.h>

#include <cstdint>

TEST_CASE("BishopEvaluator Fianchetto black") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("r4rk1/pp3pbp/4N1p1/2Q1P3/5P2/3qn1PP/PP4BK/R1B2R2 b - - 0 19");
	REQUIRE(position);

	const int32_t score = ai::score::evaluators::BishopEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0);

	const int32_t blackScore = board::EvaluationConstants::FIANCHETTO;
	const int32_t whiteScore = board::EvaluationConstants::PAIR_OF_BISHOPS;
	CHECK(score == whiteScore - blackScore);
}

TEST_CASE("BishopEvaluator Fianchetto No Bishop black") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("r4rk1/pp3p1p/4N1p1/2Q1P3/5P2/3qn1PP/PP4BK/R1B2R2 b - - 0 19");
	REQUIRE(position);

	const int32_t score = ai::score::evaluators::BishopEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0);

	const int32_t blackScore = board::EvaluationConstants::FIANCHETTO_WITHOUT_BISHOP;
	const int32_t whiteScore = board::EvaluationConstants::PAIR_OF_BISHOPS;
	CHECK(score == whiteScore - blackScore);
}

TEST_CASE("BishopEvaluator Fianchetto white") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("r3kb1r/pbp1p1pp/1pn2n2/3qNp2/2QP4/6P1/PP2PPBP/RNB2RK1 b kq - 2 9");
	REQUIRE(position);

	const int32_t score = ai::score::evaluators::BishopEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0);

	const int32_t blackScore = board::EvaluationConstants::PAIR_OF_BISHOPS;
	const int32_t whiteScore = board::EvaluationConstants::PAIR_OF_BISHOPS + board::EvaluationConstants::FIANCHETTO;
	CHECK(score == whiteScore - blackScore);
}

TEST_CASE("BishopEvaluator Fianchetto No Bishop white") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("r3kb1r/pbp1p1pp/1pn2n2/3qNp2/2QP4/6P1/PP2PP1P/RNB2RK1 b kq - 2 9");
	REQUIRE(position);

	const int32_t score = ai::score::evaluators::BishopEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0);

	const int32_t blackScore = board::EvaluationConstants::PAIR_OF_BISHOPS;
	const int32_t whiteScore = board::EvaluationConstants::FIANCHETTO_WITHOUT_BISHOP;
	CHECK(score == whiteScore - blackScore);
}
