#include <phase4/engine/fen/fen_to_position.h>
#include <phase4/engine/fen/position_to_fen.h>

#include <phase4/engine/ai/search/iterative_deepening.h>
#include <phase4/engine/ai/search/search_context.h>
#include <phase4/engine/ai/search/search_statistics.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/move_flags.h>

#include <doctest/doctest.h>

#include <memory>
#include <optional>
#include <string_view>

TEST_CASE("FenToPosition mateIn2") {
	using namespace phase4::engine;

	const std::string_view fen = "q3k1nr/1pp1nQpp/3p4/1P2p3/4P3/B1PP1b2/B5PP/5K2 b k - 0 17";
	const std::optional<board::Position> position = fen::FenToPosition::parse(fen);
	REQUIRE(position);

	auto session = std::make_unique<board::Session>(*position);

	{
		ai::search::SearchContext context(session.get());
		context.maxDepth = 8;

		const moves::Move bestMove = ai::search::IterativeDeepening::findBestMove(context);

		CHECK(bestMove.from() == common::Square::E8);
		CHECK(bestMove.to() == common::Square::D8);

		session->makeMove(moves::Move(common::Square::E8, common::Square::D7, moves::MoveFlags::QUIET)); // Make the bad move
	}

	{
		ai::search::SearchContext context(session.get());
		context.maxDepth = 7;

		const moves::Move bestMove = ai::search::IterativeDeepening::findBestMove(context);

		CHECK(bestMove.from() == common::Square::A2);
		CHECK(bestMove.to() == common::Square::E6);

		session->makeMove(bestMove);
	}

	{
		ai::search::SearchContext context(session.get());
		context.maxDepth = 6;

		const moves::Move bestMove = ai::search::IterativeDeepening::findBestMove(context);

		CHECK(bestMove.from() == common::Square::D7);
		CHECK(bestMove.to() == common::Square::D8);

		session->makeMove(bestMove);
	}

	{
		ai::search::SearchContext context(session.get());
		context.maxDepth = 5;

		const moves::Move bestMove = ai::search::IterativeDeepening::findBestMove(context);

		CHECK(bestMove.from() == common::Square::F7);
		CHECK(bestMove.to() == common::Square::F8);

		session->makeMove(bestMove);
	}
}

void testTransform(std::string_view fen) {
	using namespace phase4::engine::fen;

	auto position = FenToPosition::parse(fen);
	REQUIRE(position);

	REQUIRE(fen == PositionToFen::encode(*position));
}

TEST_CASE("FEN input should match output") {
	using namespace phase4::engine::fen;

	SUBCASE("rn2bq2/pp2pp2/2**nrkb/2**pppp/2PP2PP/2BQ2NR/PPPP4/RNKB4 w KQkq - 0 1") {
		testTransform("rn2bq2/pp2pp2/2**nrkb/2**pppp/2PP2PP/2BQ2NR/PPPP4/RNKB4 w KQkq - 0 1");
	}

	SUBCASE("rnbqkbnr/pppppppp/2**4/2**4/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
		testTransform("rnbqkbnr/pppppppp/2**4/2**4/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	}

	SUBCASE("rnbqkbnr/pppppppp/4**2/4**2/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
		testTransform("rnbqkbnr/pppppppp/4**2/4**2/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	}

	SUBCASE("rnbqkbnr/pppppppp/2**4/2**4/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
		testTransform("rnbqkbnr/pppppppp/8/8/2**4/2**4/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	}

	SUBCASE("rnbqkbnr/pppppppp/4**2/4**2/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
		testTransform("rnbqkbnr/pppppppp/8/8/4**2/4**2/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	}
}
