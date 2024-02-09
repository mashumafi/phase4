#include <phase4/engine/ai/score/evaluation.h>
#include <phase4/engine/ai/score/evaluation_statistics.h>

#include <phase4/engine/board/position_state.h>
#include <phase4/engine/board/session.h>

#include <doctest/doctest.h>

#include <memory>

TEST_CASE("Evaluation evaluate cached") {
	using namespace phase4::engine;

	auto session = std::make_unique<board::Session>();
	ai::score::EvaluationStatistics statistics;

	const int32_t score = ai::score::Evaluation::evaluate(*session, statistics);
	CHECK(-100 <= score);
	CHECK(score <= 100);
}

TEST_CASE("Evaluation evaluate not cached") {
	using namespace phase4::engine;

	const board::Position position = board::PositionState::DEFAULT;

	const int32_t score = ai::score::Evaluation::evaluate(position);
	CHECK(-100 <= score);
	CHECK(score <= 100);
}

TEST_CASE("Evaluation fast evaluate") {
	using namespace phase4::engine;

	auto session = std::make_unique<board::Session>();
	ai::score::EvaluationStatistics statistics;

	const int32_t score = ai::score::Evaluation::fastEvaluate(*session, statistics);
	CHECK(-100 <= score);
	CHECK(score <= 100);
}
