#include <phase4/engine/ai/time/time_scheduler.h>

#include <doctest/doctest.h>

TEST_CASE("TimeScheduler calculateTimeForMove") {
	using namespace phase4::engine::ai::time;

	CHECK(TimeScheduler::calculateTimeForMove(10 * 60 * 1000, 10 * 1000, 0) == 25 * 1000);
	CHECK(TimeScheduler::calculateTimeForMove(5 * 60 * 1000, 10 * 1000, 20) == 25 * 1000);
	CHECK(TimeScheduler::calculateTimeForMove(2 * 60 * 1000, 10 * 1000, 40) == 16 * 1000);
}
