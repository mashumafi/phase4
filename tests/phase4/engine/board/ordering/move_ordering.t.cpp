#include <phase4/engine/board/ordering/move_ordering.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/operators.h>

#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/move_flags.h>

#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

#include <memory>

TEST_CASE("MoveOrdering assignQValues 00008") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("r6k/pp2r2p/4Rp1Q/3p4/8/1N1P2R1/PqP2bPP/7K b - - 0 24");
	REQUIRE(position);
	const auto session = std::make_unique<board::Session>(*position);

	moves::Moves moves;
	moves::MoveValues moveValues;
	board::Operators::getAvailableCaptureMoves(session->position(), moves);
	board::ordering::MoveOrdering::assignQValues(session->position(), moves, moveValues);

	REQUIRE(moves.size() == 5);
	REQUIRE(moves.size() == moveValues.size());

	CHECK(moves[0] == moves::Move(common::Square::F2, common::Square::G3, moves::MoveFlags::CAPTURE)); // bishop
	CHECK(moves[1] == moves::Move(common::Square::E7, common::Square::E6, moves::MoveFlags::CAPTURE)); // rook
	CHECK(moves[2] == moves::Move(common::Square::B2, common::Square::C2, moves::MoveFlags::CAPTURE)); // queen
	CHECK(moves[3] == moves::Move(common::Square::B2, common::Square::A2, moves::MoveFlags::CAPTURE)); // queen
	CHECK(moves[4] == moves::Move(common::Square::B2, common::Square::B3, moves::MoveFlags::CAPTURE)); // queen
}

TEST_CASE("MoveOrdering sortNextBestMove") {
	using namespace phase4::engine;

	const moves::Move m1(common::Square::A1, common::Square::A2, moves::MoveFlags::QUIET);
	const moves::Move m2(common::Square::B3, common::Square::B4, moves::MoveFlags::QUIET);
	const moves::Move m3(common::Square::C5, common::Square::C6, moves::MoveFlags::QUIET);
	const moves::Move m4(common::Square::D7, common::Square::D8, moves::MoveFlags::QUIET);
	const moves::Move m5(common::Square::E2, common::Square::E3, moves::MoveFlags::QUIET);

	moves::Moves moves;
	moves.push_back(m1);
	moves.push_back(m2);
	moves.push_back(m3);
	moves.push_back(m4);
	moves.push_back(m5);

	moves::MoveValues moveValues;
	moveValues.push_back(42);
	moveValues.push_back(-33);
	moveValues.push_back(57);
	moveValues.push_back(-22);
	moveValues.push_back(22);

	board::ordering::MoveOrdering::sortNextBestMove(moves, moveValues, 0);
	CHECK(moves[0] == m3);
	CHECK(moveValues[0] == 57);

	board::ordering::MoveOrdering::sortNextBestMove(moves, moveValues, 1);
	CHECK(moves[1] == m1);
	CHECK(moveValues[1] == 42);

	board::ordering::MoveOrdering::sortNextBestMove(moves, moveValues, 2);
	CHECK(moves[2] == m5);
	CHECK(moveValues[2] == 22);

	board::ordering::MoveOrdering::sortNextBestMove(moves, moveValues, 3);
	CHECK(moves[3] == m4);
	CHECK(moveValues[3] == -22);

	board::ordering::MoveOrdering::sortNextBestMove(moves, moveValues, 4);
	CHECK(moves[4] == m2);
	CHECK(moveValues[4] == -33);
}
