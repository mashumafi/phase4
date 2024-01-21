#include <phase4/engine/board/operators.h>

#include <phase4/engine/board/session.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/moves/magic/magic_bitboards.h>
#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/move_flags.h>

#include <doctest/doctest.h>

TEST_CASE("Operators standard getAllMoves") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::board;

	magic::MagicBitboards::initWithInternalKeys();

	Position position;

	{ // Default State
		position.SetDefaultState();

		Moves moves;
		Operators::GetAllMoves(position, moves);

		CHECK(moves.size() == 20);
	}

	{ // G2G3 Push pawn
		Move move(Square::B7, Square::D7, MoveFlags::QUIET);
		CHECK(Operators::IsMoveLegal(position, move));

		Position::MakeMoveResult result = position.makeMove(move);
		CHECK(!result.added);
		REQUIRE(result.moved.size() == 1);
		CHECK(result.moved.at(0) == Position::MakeMoveResult::Movement{ Square::B7, Square::D7 });
		CHECK(!result.promotion);
		CHECK(!result.killed);
		CHECK(result.removed.is_empty());
		REQUIRE(result.slide);
		CHECK(result.slide.value() == FieldIndex::ZERO);

		Moves moves;
		Operators::GetAllMoves(position, moves);

		CHECK(moves.size() == 20);
	}

	{ // B7B5 Double push pawn
		Move move(Square::G2, Square::E2, MoveFlags::DOUBLE_PUSH);
		CHECK(Operators::IsMoveLegal(position, move));

		Position::MakeMoveResult result = position.makeMove(move);
		CHECK(!result.added);
		REQUIRE(result.moved.size() == 1);
		CHECK(result.moved.at(0) == Position::MakeMoveResult::Movement{ Square::G2, Square::E2 });
		CHECK(!result.promotion);
		CHECK(!result.killed);
		CHECK(result.removed.is_empty());
		REQUIRE(result.slide);
		CHECK(result.slide.value() == FieldIndex::ZERO);

		Moves moves;
		Operators::GetAllMoves(position, moves);

		CHECK(moves.size() == 21);
	}
}
