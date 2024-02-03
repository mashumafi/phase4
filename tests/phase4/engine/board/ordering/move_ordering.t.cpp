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
	board::Operators::getAvailableCaptureMoves(session->m_position, moves);
	board::ordering::MoveOrdering::assignQValues(session->m_position, moves, moveValues);

	REQUIRE(moves.size() == 5);
	REQUIRE(moves.size() == moveValues.size());

	CHECK(moves[0] == moves::Move(common::Square::F2, common::Square::G3, moves::MoveFlags::CAPTURE)); // bishop
	CHECK(moves[1] == moves::Move(common::Square::E7, common::Square::E6, moves::MoveFlags::CAPTURE)); // rook
	CHECK(moves[2] == moves::Move(common::Square::B2, common::Square::C2, moves::MoveFlags::CAPTURE)); // queen
	CHECK(moves[3] == moves::Move(common::Square::B2, common::Square::A2, moves::MoveFlags::CAPTURE)); // queen
	CHECK(moves[4] == moves::Move(common::Square::B2, common::Square::B3, moves::MoveFlags::CAPTURE)); // queen
}
