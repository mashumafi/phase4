#include <phase4/engine/ai/search/quiescence_search.h>

#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/ai/search/search_statistics.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/magic/magic_bitboards.h>
#include <phase4/engine/moves/move.h>

#include <doctest/doctest.h>

#include <memory>

TEST_CASE("QuiescenceSearch default state") {
	using namespace phase4::engine;

	moves::magic::MagicBitboards::initWithInternalKeys();

	auto session = std::make_shared<board::Session>();
	session->setDefaultState();

	ai::search::SearchContext context(session.get());

	const int32_t alpha = board::SearchConstants::MIN_VALUE;
	const int32_t beta = board::SearchConstants::MAX_VALUE;

	const int32_t eval = ai::search::QuiescenceSearch::findBestMove(context, 1, 0, alpha, beta);
	CHECK(-100 <= eval && eval <= 100);
}
