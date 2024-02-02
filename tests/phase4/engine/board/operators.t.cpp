#include <phase4/engine/board/operators.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_state.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/move_flags.h>

#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("Operators standard getAllMoves") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::board;

	Position position;

	{ // Default State
		position = PositionState::DEFAULT;

		Moves moves;
		Operators::getAllMoves(position, moves);

		CHECK(moves.size() == 20);
	}

	{ // G2G3 Push pawn
		Move move(Square::G2, Square::G3, MoveFlags::QUIET);
		CHECK(Operators::isMoveLegal(position, move));

		Position::MakeMoveResult result = position.makeMove(move);
		CHECK(!result.added);
		REQUIRE(result.moved.size() == 1);
		CHECK(result.moved.at(0) == Position::MakeMoveResult::Movement{ Square::G2, Square::G3 });
		CHECK(!result.promotion);
		CHECK(!result.killed);
		CHECK(result.removed.is_empty());
		REQUIRE(result.slide);
		CHECK(result.slide.value() == FieldIndex::ZERO);

		Moves moves;
		Operators::getAllMoves(position, moves);

		CHECK(moves.size() == 20);
	}

	{ // B7B5 Double push pawn
		Move move(Square::B7, Square::B5, MoveFlags::DOUBLE_PUSH);
		CHECK(Operators::isMoveLegal(position, move));

		Position::MakeMoveResult result = position.makeMove(move);
		CHECK(!result.added);
		REQUIRE(result.moved.size() == 1);
		CHECK(result.moved.at(0) == Position::MakeMoveResult::Movement{ Square::B7, Square::B5 });
		CHECK(!result.promotion);
		CHECK(!result.killed);
		CHECK(result.removed.is_empty());
		REQUIRE(result.slide);
		CHECK(result.slide.value() == FieldIndex::ZERO);

		Moves moves;
		Operators::getAllMoves(position, moves);

		CHECK(moves.size() == 21);
	}

	{ // Fianchetto bishop
		Move move(Square::F1, Square::G2, MoveFlags::QUIET);
		CHECK(Operators::isMoveLegal(position, move));

		Position::MakeMoveResult result = position.makeMove(move);
		CHECK(!result.added);
		REQUIRE(result.moved.size() == 1);
		CHECK(result.moved.at(0) == Position::MakeMoveResult::Movement{ Square::F1, Square::G2 });
		CHECK(!result.promotion);
		CHECK(!result.killed);
		CHECK(result.removed.is_empty());
		REQUIRE(result.slide);
		CHECK(result.slide.value() == FieldIndex::ZERO);

		Moves moves;
		Operators::getAllMoves(position, moves);

		CHECK(moves.size() == 21);
	}

	{ // E7E6 Push king pawn
		Move move(Square::E7, Square::E6, MoveFlags::QUIET);
		CHECK(Operators::isMoveLegal(position, move));

		Position::MakeMoveResult result = position.makeMove(move);
		CHECK(!result.added);
		REQUIRE(result.moved.size() == 1);
		CHECK(result.moved.at(0) == Position::MakeMoveResult::Movement{ Square::E7, Square::E6 });
		CHECK(!result.promotion);
		CHECK(!result.killed);
		CHECK(result.removed.is_empty());
		REQUIRE(result.slide);
		CHECK(result.slide.value() == FieldIndex::ZERO);

		Moves moves;
		Operators::getAllMoves(position, moves);

		CHECK(moves.size() == 28);
	}

	{ // E7E6 Capture rook with bishop
		Move move(Square::G2, Square::A8, MoveFlags::CAPTURE);
		CHECK(Operators::isMoveLegal(position, move));

		Position::MakeMoveResult result = position.makeMove(move);
		CHECK(!result.added);
		REQUIRE(result.moved.size() == 1);
		CHECK(result.moved.at(0) == Position::MakeMoveResult::Movement{ Square::G2, Square::A8 });
		CHECK(!result.promotion);
		REQUIRE(result.killed);
		CHECK(result.killed.value() == Position::MakeMoveResult::PieceAndSquare{ PieceType::ROOK, Square::A8 });
		REQUIRE(result.removed.size() == 1);
		CHECK(result.removed.at(0) == Position::MakeMoveResult::PieceAndSquare{ PieceType::ROOK, Square::A8 });
		REQUIRE(result.slide);
		CHECK(result.slide.value() == FieldIndex::ZERO);

		Moves moves;
		Operators::getAllMoves(position, moves);

		CHECK(moves.size() == 31);
	}
}
