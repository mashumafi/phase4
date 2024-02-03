#include <phase4/engine/board/session.h>

#include <phase4/engine/fen/position_to_fen.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/move_flags.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

#include <memory>

TEST_CASE("Session scholar's mate") {
	using namespace phase4::engine;

	auto session = std::make_unique<board::Session>();
	session->makeMove(moves::Move(common::Square::E2, common::Square::E3, moves::MoveFlags::QUIET));
	session->makeMove(moves::Move(common::Square::E7, common::Square::E5, moves::MoveFlags::DOUBLE_PUSH));
	session->makeMove(moves::Move(common::Square::F1, common::Square::C4, moves::MoveFlags::QUIET));
	session->makeMove(moves::Move(common::Square::D7, common::Square::D6, moves::MoveFlags::QUIET));
	session->makeMove(moves::Move(common::Square::D1, common::Square::H5, moves::MoveFlags::QUIET));
	session->makeMove(moves::Move(common::Square::B8, common::Square::C6, moves::MoveFlags::QUIET));
	session->makeMove(moves::Move(common::Square::H5, common::Square::F7, moves::MoveFlags::CAPTURE));
	CHECK(fen::PositionToFen::encode(session->m_position) == "r1bqkbnr/ppp2Qpp/2np4/4p3/2B5/4P3/PPPP1PPP/RNB1K1NR b KQkq - 0 4");
	session->makeMove(moves::Move(common::Square::E8, common::Square::F7, moves::MoveFlags::CAPTURE));
	session->makeMove(moves::Move(common::Square::C4, common::Square::F7, moves::MoveFlags::CAPTURE));

	CHECK(session->m_position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::KING.get_raw_value()] == 0);

	session->undoMove(moves::Move(common::Square::C4, common::Square::F7, moves::MoveFlags::CAPTURE));
	session->undoMove(moves::Move(common::Square::E8, common::Square::F7, moves::MoveFlags::CAPTURE));
	session->undoMove(moves::Move(common::Square::H5, common::Square::F7, moves::MoveFlags::CAPTURE));
	session->undoMove(moves::Move(common::Square::B8, common::Square::C6, moves::MoveFlags::QUIET));
	session->undoMove(moves::Move(common::Square::D1, common::Square::H5, moves::MoveFlags::QUIET));
	session->undoMove(moves::Move(common::Square::D7, common::Square::D6, moves::MoveFlags::QUIET));
	session->undoMove(moves::Move(common::Square::F1, common::Square::C4, moves::MoveFlags::QUIET));
	session->undoMove(moves::Move(common::Square::E7, common::Square::E5, moves::MoveFlags::DOUBLE_PUSH));
	session->undoMove(moves::Move(common::Square::E2, common::Square::E3, moves::MoveFlags::QUIET));

	CHECK(fen::PositionToFen::encode(session->m_position) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}
