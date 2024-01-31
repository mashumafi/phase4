#include <phase4/engine/ai/search/iterative_deepening.h>

#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/ai/search/search_statistics.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/magic/magic_bitboards.h>
#include <phase4/engine/moves/move.h>

#include <doctest/doctest.h>

#include <memory>

TEST_CASE("IterativeDeepening default state") {
	using namespace phase4::engine;

	moves::magic::MagicBitboards::initWithInternalKeys();

	auto session = std::make_shared<board::Session>();
	session->setDefaultState();

	ai::search::SearchContext context(session.get());
	context.maxDepth = 8;

	const moves::Move bestMove = ai::search::IterativeDeepening::findBestMove(context, [](const ai::search::SearchStatistics &) {});

	CHECK(bestMove != moves::Move::Empty);
	CHECK(-100 <= context.statistics.score && context.statistics.score <= 100);
}
