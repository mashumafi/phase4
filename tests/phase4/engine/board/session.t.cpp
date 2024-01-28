#include <phase4/engine/board/session.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/magic/magic_bitboards.h>
#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/move_flags.h>

#include <phase4/engine/common/piece_color.h>

#include <doctest/doctest.h>

TEST_CASE("Session standard move/check/undo") {
	using namespace phase4::engine;

	moves::magic::MagicBitboards::initWithInternalKeys();

	SUBCASE("Long castle") {
	}

	SUBCASE("Short castle") {
	}

	SUBCASE("En passant") {
	}

	SUBCASE("Promotion") {
	}
}