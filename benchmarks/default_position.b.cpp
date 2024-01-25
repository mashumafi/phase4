#include <benchmark/benchmark.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_state.h>

static void setDefaultState(benchmark::State &state) {
	using namespace phase4::engine;

	board::Position position;
	for (auto _ : state) {
		board::PositionState::setDefaultState(position);
	}
}
BENCHMARK(setDefaultState);

// Make a copy, make a move, check if king is in check
static void copyDefaultState(benchmark::State &state) {
	using namespace phase4::engine;

	board::Position position;
	for (auto _ : state) {
		benchmark::DoNotOptimize(position = board::PositionState::DEFAULT);
	}
}
BENCHMARK(copyDefaultState);

BENCHMARK_MAIN();
