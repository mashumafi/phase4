#include <phase4/engine/fen/fen_validator.h>

#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/move.h>

#include <doctest/doctest.h>

#include <array>
#include <memory>
#include <string>
#include <vector>

namespace {

struct FenTestCase {
	std::string fen = "";
	std::string badMove = "";
	std::vector<std::string> principalVariation = std::vector<std::string>();
	int32_t maxDepth = 32;
	phase4::engine::fen::FenValidator::Result result = phase4::engine::fen::FenValidator::MOVES_MATCH;
};

} //namespace

TEST_CASE("FenValidator") {
	using namespace phase4::engine;

	auto testCases = std::array{ FenTestCase{
			"8/2Q2ppk/4b2p/8/4P3/2p3P1/4KP1P/2r5 w - - 4 49",
			"f2f4", { "e6b3", "e4e5", "c3c2", "e5e6", "f7e6", "c7c3", "c1d1", "c3b3", "c2c1n", "e2d1", "c1b3" }, 5, fen::FenValidator::FOUND_BEST } };

	auto session = std::make_unique<board::Session>();
	ai::search::SearchContext context(session.get());

	for (const FenTestCase &testCase : testCases) {
		moves::Moves moves;
		for (const std::string &move : testCase.principalVariation) {
			moves.emplace_back(move);
		}
		context.maxDepth = testCase.maxDepth;
		CHECK(fen::FenValidator::validate(testCase.fen, moves::Move(testCase.badMove), moves, context) == testCase.result);
	}
}
