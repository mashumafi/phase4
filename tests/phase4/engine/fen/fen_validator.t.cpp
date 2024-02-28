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
	std::string blunder = "";
	std::vector<std::string> principalVariation = std::vector<std::string>();
	int32_t maxDepth = 32;
	phase4::engine::fen::FenValidator::Result result = phase4::engine::fen::FenValidator::MOVES_MATCH;
};

} //namespace

TEST_CASE("FenValidator") {
	using namespace phase4::engine;

	auto testCases = std::array{
		FenTestCase{
				"8/2Q2ppk/4b2p/8/4P3/2p3P1/4KP1P/2r5 w - - 4 49",
				"f2f4", { "e6b3", "e4e5", "c3c2", "e5e6", "f7e6", "c7c3", "c1d1", "c3b3", "c2c1n", "e2d1", "c1b3" },
				5, fen::FenValidator::FOUND_BEST },
		FenTestCase{
				"4r1k1/pp1brpp1/1qp2b1p/3p4/1P1P4/2PB1Q2/P3NPPP/R3R1K1 w - - 1 18",
				"e2g3", { "e7e1", "a1e1", "e8e1" },
				6, fen::FenValidator::MOVES_MATCH },
		FenTestCase{
				"8/r4pk1/3Np3/p1PbP1pp/P7/7P/2R3P1/7K b - - 0 39",
				"a7c7", { "d6e8", "g7f8", "e8c7" },
				12, fen::FenValidator::FOUND_BEST },
		FenTestCase{
				"8/1p1k4/p1pp4/7R/PP2n1p1/4P1K1/2P5/8 w - - 0 38",
				"g3g4", { "e4f6", "g4f5", "f6h5" },
				4, fen::FenValidator::FOUND_BEST },
		FenTestCase{
				"2r3k1/2r4p/4p1p1/1p1q1pP1/p1bP1P1Q/P6R/5B2/2R3K1 b - - 5 34",
				"c4e2", { "h4h7", "c7h7", "c1c8", "g8g7", "c8c7" },
				6, fen::FenValidator::FOUND_BEST },
	};

	for (const FenTestCase &testCase : testCases) {
		moves::Moves moves;
		for (const std::string &move : testCase.principalVariation) {
			moves.emplace_back(move);
		}
		auto session = std::make_unique<board::Session>();
		ai::search::SearchContext context(session.get());
		context.maxDepth = testCase.maxDepth;
		const fen::FenValidator::Result result = fen::FenValidator::validate(testCase.fen, moves::Move(testCase.blunder), moves, context);
		CHECK(result == testCase.result);
		if (result != testCase.result) {
			std::cerr << testCase.fen << " " << context.statistics.principalVariation << std::endl;
		}
	}
}

TEST_CASE("FenValidator r2q1rk1/4N1bp/p2p2p1/2p3N1/Pp4P1/1Q5P/1P1n1P2/5RK1 b - - 0 21" * doctest::skip()) {
	using namespace phase4::engine;

	const std::string fen = "r2q1rk1/4N1bp/p2p2p1/2p3N1/Pp4P1/1Q5P/1P1n1P2/5RK1 b - - 0 21";
	const moves::Move blunder("g8h8");
	const std::vector<std::string> principalVariation{ "b3g8", "f8g8", "g5f7" };

	moves::Moves moves;
	for (const std::string &move : principalVariation) {
		moves.emplace_back(move);
	}
	auto session = std::make_unique<board::Session>();
	ai::search::SearchContext context(session.get());
	context.maxDepth = 7;
	const fen::FenValidator::Result result = fen::FenValidator::validate(fen, blunder, moves, context);
	CHECK(result == fen::FenValidator::CHECKMATE);
}
