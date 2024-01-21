#include <phase4/engine/board/operators.h>

#include <phase4/engine/board/session.h>
#include <phase4/engine/moves/magic/magic_bitboards.h>
#include <phase4/engine/moves/move.h>

#include <doctest/doctest.h>

TEST_CASE("Operators standard getAllMoves") {
	using namespace phase4::engine;

	moves::magic::MagicBitboards::initWithInternalKeys();

	board::Position position;
	position.SetDefaultState();

	moves::Moves moves;
	board::Operators::GetAllMoves(position, moves);

	CHECK(moves.size() == 20);
}
