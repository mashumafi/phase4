#include <phase4/engine/fen/position_to_fen.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/move_flags.h>

#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

#include <memory>

TEST_CASE("PositionToFen default") {
	using namespace phase4::engine;

	const board::Position position = board::PositionState::DEFAULT;
	CHECK(fen::PositionToFen::encode(position) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST_CASE("PositionToFen enpassant") {
	using namespace phase4::engine;

	auto session = std::make_unique<board::Session>();

	// make moves
	session->makeMove(moves::Move("f2f4"));
	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pppppppp/8/8/5P2/8/PPPPP1PP/RNBQKBNR b KQkq - 0 1");
	session->makeMove(moves::Move("c7c6"));
	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pp1ppppp/2p5/8/5P2/8/PPPPP1PP/RNBQKBNR w KQkq - 0 2");
	session->makeMove(moves::Move("f4f5"));
	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pp1ppppp/2p5/5P2/8/8/PPPPP1PP/RNBQKBNR b KQkq - 0 2");
	session->makeMove(moves::Move(common::Square::E7, common::Square::E5, moves::MoveFlags::DOUBLE_PUSH));
	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pp1p1ppp/2p5/4pP2/8/8/PPPPP1PP/RNBQKBNR w KQkq e6 0 3");
	session->makeMove(moves::Move(common::Square::F5, common::Square::E6, moves::MoveFlags::EN_PASSANT));
	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pp1p1ppp/2p1P3/8/8/8/PPPPP1PP/RNBQKBNR b KQkq - 0 3");

	// undo moves
	session->undoMove(moves::Move(common::Square::F5, common::Square::E6, moves::MoveFlags::EN_PASSANT));
	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pp1p1ppp/2p5/4pP2/8/8/PPPPP1PP/RNBQKBNR w KQkq e6 0 3");
	session->undoMove(moves::Move(common::Square::E7, common::Square::E5, moves::MoveFlags::DOUBLE_PUSH));
	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pp1ppppp/2p5/5P2/8/8/PPPPP1PP/RNBQKBNR b KQkq - 0 2");
	session->undoMove(moves::Move("f4f5"));
	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pp1ppppp/2p5/8/5P2/8/PPPPP1PP/RNBQKBNR w KQkq - 0 2");
	session->undoMove(moves::Move("c7c6"));
	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pppppppp/8/8/5P2/8/PPPPP1PP/RNBQKBNR b KQkq - 0 1");
	session->undoMove(moves::Move("f2f4"));
	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}
