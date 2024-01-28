#include <phase4/engine/board/session.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/magic/magic_bitboards.h>
#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/move_flags.h>

#include <phase4/engine/common/piece_color.h>

#include <doctest/doctest.h>

#include <memory>

TEST_CASE("Session standard move/check/undo") {
	using namespace phase4::engine;

	moves::magic::MagicBitboards::initWithInternalKeys();

	auto session = std::make_unique<board::Session>();

	SUBCASE("Long castle") {
		session->makeMove(moves::Move("d2d4"));
	}

	SUBCASE("Short castle") {
	}

	SUBCASE("En passant") {
	}

	SUBCASE("Promotion") {
	}
}