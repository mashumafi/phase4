#include <phase4/engine/fen/position_to_fen.h>
#include <phase4/engine/fen/fen_to_position.h>

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
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppppppp/8/8/5P2/8/PPPPP1PP/RNBQKBNR b KQkq - 0 1");
	session->makeMove(moves::Move("c7c6"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pp1ppppp/2p5/8/5P2/8/PPPPP1PP/RNBQKBNR w KQkq - 0 2");
	session->makeMove(moves::Move("f4f5"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pp1ppppp/2p5/5P2/8/8/PPPPP1PP/RNBQKBNR b KQkq - 0 2");
	session->makeMove(moves::Move(common::Square::E7, common::Square::E5, moves::MoveFlags::DOUBLE_PUSH));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pp1p1ppp/2p5/4pP2/8/8/PPPPP1PP/RNBQKBNR w KQkq e6 0 3");
	session->makeMove(moves::Move(common::Square::F5, common::Square::E6, moves::MoveFlags::EN_PASSANT));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pp1p1ppp/2p1P3/8/8/8/PPPPP1PP/RNBQKBNR b KQkq - 0 3");

	// undo moves
	session->undoMove(moves::Move(common::Square::F5, common::Square::E6, moves::MoveFlags::EN_PASSANT));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pp1p1ppp/2p5/4pP2/8/8/PPPPP1PP/RNBQKBNR w KQkq e6 0 3");
	session->undoMove(moves::Move(common::Square::E7, common::Square::E5, moves::MoveFlags::DOUBLE_PUSH));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pp1ppppp/2p5/5P2/8/8/PPPPP1PP/RNBQKBNR b KQkq - 0 2");
	session->undoMove(moves::Move("f4f5"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pp1ppppp/2p5/8/5P2/8/PPPPP1PP/RNBQKBNR w KQkq - 0 2");
	session->undoMove(moves::Move("c7c6"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppppppp/8/8/5P2/8/PPPPP1PP/RNBQKBNR b KQkq - 0 1");
	session->undoMove(moves::Move("f2f4"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST_CASE("PositionToFen slide") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("rnbqkbnr/pppppppp/8/8/2**4/2**4/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	REQUIRE(position);

	auto session = std::make_unique<board::Session>(*position);
	session->makeMove(moves::Move("e2e4"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppppppp/8/8/2P1**2/4**2/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
	session->makeMove(moves::Move("e7e5"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppp1ppp/4**2/4**2/2P5/4p3/PPPP1PPP/RNBQKBNR w KQkq - 0 2");
	session->makeMove(moves::Move("f2f3"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppp1ppp/8/4pP2/2P1**2/4**2/PPPP2PP/RNBQKBNR b KQkq - 0 2");
	session->makeMove(moves::Move("f7f6"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppp2pp/4**2/4**2/2P2p2/4pP2/PPPP2PP/RNBQKBNR w KQkq - 0 3");
	session->makeMove(moves::Move("g1e2"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppp2pp/4**2/4**2/2P2p2/4pP2/PPPPN1PP/RNBQKB1R b KQkq - 1 3");
	session->makeMove(moves::Move("d7d6"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/ppp3pp/2**1p2/2**4/2P2p2/4pP2/PPPPN1PP/RNBQKB1R w KQkq - 0 4");
	session->makeMove(moves::Move("b1c3"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/ppp3pp/2P2p2/2N5/2**1p2/2**pP2/PPPPN1PP/R1BQKB1R b KQkq - 1 4");
	session->makeMove(moves::Move("d8d6"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnb1kbnr/ppp3pp/2**1p2/2**4/2Pq1p2/2N1pP2/PPPPN1PP/R1BQKB1R w KQkq - 2 5");
	session->makeMove(moves::Move("d2d3"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnb1kbnr/ppp3pp/2Pq1p2/2NP4/2**1p2/2**pP2/PPP1N1PP/R1BQKB1R b KQkq - 0 5");
	session->makeMove(moves::Move(common::Square::B8, common::Square::C6, moves::MoveFlags::CAPTURE));
	CHECK(fen::PositionToFen::encode(session->position()) == "r1b1kbnr/ppp3pp/2**1p2/2**4/2nq1p2/2NPpP2/PPP1N1PP/R1BQKB1R w KQkq - 0 6");
	session->makeMove(moves::Move(common::Square::C3, common::Square::B5, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r1b1kbnr/ppp3pp/**3p2/**1N4/2nq1p2/3PpP2/PPP1N1PP/R1BQKB1R b KQkq - 1 6");
	session->makeMove(moves::Move(common::Square::C7, common::Square::C6, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r1b1kbnr/pp4pp/p1**1p2/1N**4/2nq1p2/3PpP2/PPP1N1PP/R1BQKB1R w KQkq - 0 7");
	session->makeMove(moves::Move(common::Square::E2, common::Square::C3, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r1b1kbnr/pp4pp/p1nq1p2/1NNP4/2**1p2/2**pP2/PPP3PP/R1BQKB1R b KQkq - 1 7");
	session->makeMove(moves::Move(common::Square::C8, common::Square::F5, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r3kbnr/pp4pp/p1nq1p2/1NNP1b2/2**1p2/2**pP2/PPP3PP/R1BQKB1R w KQkq - 2 8");
	session->makeMove(moves::Move(common::Square::C5, common::Square::A4, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r3kbnr/pp4pp/p1nq1p2/1N1P1b2/**N2p2/**2pP2/PPP3PP/R1BQKB1R b KQkq - 3 8");
	session->makeMove(moves::Move(common::Square::F5, common::Square::D3, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r3kbnr/pp4pp/p1nq1p2/1N1P4/N1**1p2/1b**pP2/PPP3PP/R1BQKB1R w KQkq - 4 9");
	session->makeMove(moves::Move(common::Square::B5, common::Square::D6, moves::MoveFlags::CAPTURE));
	CHECK(fen::PositionToFen::encode(session->position()) == "r3kbnr/pp4pp/p1**1p2/2**4/N1nN1p2/1b1PpP2/PPP3PP/R1BQKB1R b KQkq - 0 9");
	session->makeMove(moves::Move(common::Square::E8, common::Square::C8, moves::MoveFlags::QUEEN_CASTLE));
	CHECK(fen::PositionToFen::encode(session->position()) == "2**1bnr/pp**2pp/p1kr1p2/8/N1nN1p2/1b1PpP2/PPP3PP/R1BQKB1R w KQ - 0 10");

	session->undoMove(moves::Move(common::Square::E8, common::Square::C8, moves::MoveFlags::QUEEN_CASTLE));
	CHECK(fen::PositionToFen::encode(session->position()) == "r3kbnr/pp4pp/p1**1p2/2**4/N1nN1p2/1b1PpP2/PPP3PP/R1BQKB1R b KQkq - 0 9");
	session->undoMove(moves::Move(common::Square::B5, common::Square::D6, moves::MoveFlags::CAPTURE));
	CHECK(fen::PositionToFen::encode(session->position()) == "r3kbnr/pp4pp/p1nq1p2/1N1P4/N1**1p2/1b**pP2/PPP3PP/R1BQKB1R w KQkq - 4 9");
	session->undoMove(moves::Move(common::Square::F5, common::Square::D3, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r3kbnr/pp4pp/p1nq1p2/1N1P1b2/**N2p2/**2pP2/PPP3PP/R1BQKB1R b KQkq - 3 8");
	session->undoMove(moves::Move(common::Square::C5, common::Square::A4, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r3kbnr/pp4pp/p1nq1p2/1NNP1b2/2**1p2/2**pP2/PPP3PP/R1BQKB1R w KQkq - 2 8");
	session->undoMove(moves::Move(common::Square::C8, common::Square::F5, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r1b1kbnr/pp4pp/p1nq1p2/1NNP4/2**1p2/2**pP2/PPP3PP/R1BQKB1R b KQkq - 1 7");
	session->undoMove(moves::Move(common::Square::E2, common::Square::C3, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r1b1kbnr/pp4pp/p1**1p2/1N**4/2nq1p2/3PpP2/PPP1N1PP/R1BQKB1R w KQkq - 0 7");
	session->undoMove(moves::Move(common::Square::C7, common::Square::C6, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r1b1kbnr/ppp3pp/**3p2/**1N4/2nq1p2/3PpP2/PPP1N1PP/R1BQKB1R b KQkq - 1 6");
	session->undoMove(moves::Move(common::Square::C3, common::Square::B5, moves::MoveFlags::QUIET));
	CHECK(fen::PositionToFen::encode(session->position()) == "r1b1kbnr/ppp3pp/2**1p2/2**4/2nq1p2/2NPpP2/PPP1N1PP/R1BQKB1R w KQkq - 0 6");
	session->undoMove(moves::Move(common::Square::B8, common::Square::C6, moves::MoveFlags::CAPTURE));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnb1kbnr/ppp3pp/2Pq1p2/2NP4/2**1p2/2**pP2/PPP1N1PP/R1BQKB1R b KQkq - 0 5");
	session->undoMove(moves::Move("d2d3"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnb1kbnr/ppp3pp/2**1p2/2**4/2Pq1p2/2N1pP2/PPPPN1PP/R1BQKB1R w KQkq - 2 5");
	session->undoMove(moves::Move("d8d6"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/ppp3pp/2P2p2/2N5/2**1p2/2**pP2/PPPPN1PP/R1BQKB1R b KQkq - 1 4");
	session->undoMove(moves::Move("b1c3"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/ppp3pp/2**1p2/2**4/2P2p2/4pP2/PPPPN1PP/RNBQKB1R w KQkq - 0 4");
	session->undoMove(moves::Move("d7d6"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppp2pp/4**2/4**2/2P2p2/4pP2/PPPPN1PP/RNBQKB1R b KQkq - 1 3");
	session->undoMove(moves::Move("g1e2"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppp2pp/4**2/4**2/2P2p2/4pP2/PPPP2PP/RNBQKBNR w KQkq - 0 3");
	session->undoMove(moves::Move("f7f6"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppp1ppp/8/4pP2/2P1**2/4**2/PPPP2PP/RNBQKBNR b KQkq - 0 2");
	session->undoMove(moves::Move("f2f3"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppp1ppp/4**2/4**2/2P5/4p3/PPPP1PPP/RNBQKBNR w KQkq - 0 2");
	session->undoMove(moves::Move("e7e5"));
	CHECK(fen::PositionToFen::encode(session->position()) == "rnbqkbnr/pppppppp/8/8/2P1**2/4**2/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
	session->undoMove(moves::Move("e2e4"));
}
