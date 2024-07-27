#include <phase4/engine/board/position_moves.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/fen/fen_to_position.h>
#include <phase4/engine/fen/position_to_fen.h>

#include <phase4/engine/moves/move.h>

#include <doctest/doctest.h>

TEST_CASE("Position moves valid default") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = PositionState::DEFAULT;

	Moves moves;
	PositionMoves::getValidMoves(position, moves);
	CHECK(moves.size() == 20);
	CHECK(PositionMoves::findRealMove(position, Move("a2a3")));
	CHECK(PositionMoves::findRealMove(position, Move("a2a4")));

	CHECK(PositionMoves::findRealMove(position, Move("b2b3")));
	CHECK(PositionMoves::findRealMove(position, Move("b2b4")));

	CHECK(PositionMoves::findRealMove(position, Move("c2c3")));
	CHECK(PositionMoves::findRealMove(position, Move("c2c4")));

	CHECK(PositionMoves::findRealMove(position, Move("d2d3")));
	CHECK(PositionMoves::findRealMove(position, Move("d2d4")));

	CHECK(PositionMoves::findRealMove(position, Move("e2e3")));
	CHECK(PositionMoves::findRealMove(position, Move("e2e4")));

	CHECK(PositionMoves::findRealMove(position, Move("f2f3")));
	CHECK(PositionMoves::findRealMove(position, Move("f2f4")));

	CHECK(PositionMoves::findRealMove(position, Move("g2g3")));
	CHECK(PositionMoves::findRealMove(position, Move("g2g4")));

	CHECK(PositionMoves::findRealMove(position, Move("h2h3")));
	CHECK(PositionMoves::findRealMove(position, Move("h2h4")));

	CHECK(PositionMoves::findRealMove(position, Move("g1h3")));
	CHECK(PositionMoves::findRealMove(position, Move("g1f3")));

	CHECK(PositionMoves::findRealMove(position, Move("b1c3")));
	CHECK(PositionMoves::findRealMove(position, Move("b1a3")));
}

TEST_CASE("Position moves valid rn2bq2/pp2pp2/2**nrkb/2**pppp/2PP2PP/2BQ2NR/PPPP4/RNKB4 w - - 0 1") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = FenToPosition::parse("rn2bq2/pp2pp2/2**nrkb/2**pppp/2PP2PP/2BQ2NR/PPPP4/RNKB4 w - - 0 1");
	REQUIRE(position);

	Moves moves;
	PositionMoves::getValidMoves(*position, moves);
	CHECK(moves.size() == 27);
	CHECK(PositionMoves::findRealMove(*position, Move("a2a3")));
	CHECK(PositionMoves::findRealMove(*position, Move("a2a4")));

	CHECK(PositionMoves::findRealMove(*position, Move("b2b3")));
	CHECK(PositionMoves::findRealMove(*position, Move("b2b4")));

	CHECK(PositionMoves::findRealMove(*position, Move("b1a3")));

	CHECK(PositionMoves::findRealMove(*position, Move("c3b4")));
	CHECK(PositionMoves::findRealMove(*position, Move("c3a5")));

	CHECK(PositionMoves::findRealMove(*position, Move("d4e5")));

	CHECK(PositionMoves::findRealMove(*position, Move("d3e4")));
	CHECK(PositionMoves::findRealMove(*position, Move("d3e3")));
	CHECK(PositionMoves::findRealMove(*position, Move("d3e2")));
	CHECK(PositionMoves::findRealMove(*position, Move("d3f5")));
	CHECK(PositionMoves::findRealMove(*position, Move("d3f3")));
	CHECK(PositionMoves::findRealMove(*position, Move("d3f1")));

	CHECK(PositionMoves::findRealMove(*position, Move("d1e2")));
	CHECK(PositionMoves::findRealMove(*position, Move("d1f3")));

	CHECK(PositionMoves::findRealMove(*position, Move("g4f5")));
	CHECK(PositionMoves::findRealMove(*position, Move("g4h5")));

	CHECK(PositionMoves::findRealMove(*position, Move("g3e4")));
	CHECK(PositionMoves::findRealMove(*position, Move("g3e2")));
	CHECK(PositionMoves::findRealMove(*position, Move("g3f5")));
	CHECK(PositionMoves::findRealMove(*position, Move("g3f1")));
	CHECK(PositionMoves::findRealMove(*position, Move("g3h5")));
	CHECK(PositionMoves::findRealMove(*position, Move("g3h1")));

	CHECK(PositionMoves::findRealMove(*position, Move("h4g5")));

	CHECK(PositionMoves::findRealMove(*position, Move("h3h2")));
	CHECK(PositionMoves::findRealMove(*position, Move("h3h1")));
}

TEST_CASE("Position moves valid rnbqkbnr/pppppppp/8/8/2**4/2**4/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = FenToPosition::parse("rnbqkbnr/pppppppp/8/8/2**4/2**4/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	REQUIRE(position);
	CHECK(position->walls() != 0);
	CHECK((position->occupancySummary() & phase4::engine::common::Square::C3.asBitboard()) != 0);
	CHECK((position->occupancySummary() & phase4::engine::common::Square::C4.asBitboard()) != 0);
	CHECK((position->occupancySummary() & phase4::engine::common::Square::D3.asBitboard()) != 0);
	CHECK((position->occupancySummary() & phase4::engine::common::Square::D4.asBitboard()) != 0);

	Moves moves;
	PositionMoves::getValidMoves(*position, moves);
	CHECK(moves.size() == 15);
	CHECK(PositionMoves::findRealMove(*position, Move("a2a3")));
	CHECK(PositionMoves::findRealMove(*position, Move("a2a4")));

	CHECK(PositionMoves::findRealMove(*position, Move("b2b3")));
	CHECK(PositionMoves::findRealMove(*position, Move("b2b4")));

	CHECK(PositionMoves::findRealMove(*position, Move("e2e3")));
	CHECK(PositionMoves::findRealMove(*position, Move("e2e4")));

	CHECK(PositionMoves::findRealMove(*position, Move("f2f3")));
	CHECK(PositionMoves::findRealMove(*position, Move("f2f4")));

	CHECK(PositionMoves::findRealMove(*position, Move("g2g3")));
	CHECK(PositionMoves::findRealMove(*position, Move("g2g4")));

	CHECK(PositionMoves::findRealMove(*position, Move("h2h3")));
	CHECK(PositionMoves::findRealMove(*position, Move("h2h4")));

	CHECK(PositionMoves::findRealMove(*position, Move("g1h3")));
	CHECK(PositionMoves::findRealMove(*position, Move("g1f3")));

	CHECK(PositionMoves::findRealMove(*position, Move("b1a3")));
}

TEST_CASE("Position moves valid rnbqkbnr/2p2p2/1p2p1p1/p2p3p/P2P3P/1P2P1P1/2P2P2/RNBQKBNR w KQkq d6 0 7") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = FenToPosition::parse("rnbqkbnr/2p2p2/1p2p1p1/p2p3p/P2P3P/1P2P1P1/2P2P2/RNBQKBNR w KQkq d6 0 7");
	REQUIRE(position);

	Moves moves;
	PositionMoves::getValidMoves(*position, moves);
	CHECK(moves.size() == 35);

	// Black rook
	CHECK(PositionMoves::findRealMove(*position, Move("a1a2")));
	CHECK(PositionMoves::findRealMove(*position, Move("a1a3")));

	// White rook
	CHECK(PositionMoves::findRealMove(*position, Move("h1h2")));
	CHECK(PositionMoves::findRealMove(*position, Move("h1h3")));

	// Black bishop
	CHECK(PositionMoves::findRealMove(*position, Move("c1a3")));
	CHECK(PositionMoves::findRealMove(*position, Move("c1b2")));
	CHECK(PositionMoves::findRealMove(*position, Move("c1d2")));

	// White bishop
	CHECK(PositionMoves::findRealMove(*position, Move("f1a6")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1b5")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1c4")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1d3")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1e2")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1g2")));
	CHECK(PositionMoves::findRealMove(*position, Move("f1h3")));
}

TEST_CASE("Position moves valid rnbqkbnr/pppp3p/7P/4p1P1/3P2**/4pN**/PPPPPR2/RNBQKB2 b Qkq - 1 6") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::fen;

	auto position = FenToPosition::parse("rnbqkbnr/pppp3p/7P/4p1P1/3P2**/4pN**/PPPPPR2/RNBQKB2 b Qkq - 1 6");
	REQUIRE(position);

	{
		Moves moves;
		PositionMoves::getValidMoves(*position, moves);
		CHECK(moves.size() == 29);

		CHECK(PositionMoves::findRealMove(moves, Move("e3d2")));
		CHECK(PositionMoves::findRealMove(moves, Move("e3f2")));

		auto queenMove = PositionMoves::findRealMove(moves, Move("d8g5"));
		REQUIRE(queenMove);
		CHECK(queenMove->flags().isCapture());
		PositionMoves::makeMove(*position, *queenMove);
	}

	{
		Moves moves;
		PositionMoves::getValidMoves(*position, moves);
		CHECK(moves.size() == 17);

		CHECK_FALSE(PositionMoves::findRealMove(moves, Move("f2g2")));
		CHECK_FALSE(PositionMoves::findRealMove(moves, Move("f2h2")));
	}
}

TEST_CASE("PositionMoves Algebraic Notation") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves;
	using namespace phase4::engine::board;
	using namespace phase4::engine::fen;

	using namespace std::string_literals;

	// Pawn moves
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), Move(Square::E2, Square::E4, MoveFlags::QUIET)).data() == L"1. e4"s);
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3"), Move(Square::E5, Square::F6, MoveFlags::QUIET)).data() == L"3. exf6"s);
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("rnbqkbnr/ppp3p1/4p3/3pPpPp/8/8/PPPP1P1P/RNBQKBNR w KQkq f6 0 5"), Move(Square::G5, Square::F6, MoveFlags::QUIET)).data() == L"5. gxf6"s);

	// Capture + Check
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("rnbqkbnr/ppp3p1/4p3/3pPpPp/8/8/PPPP1P1P/RNBQKBNR w KQkq f6 0 5"), Move(Square::D1, Square::H5, MoveFlags::QUIET)).data() == L"5. ♛xh5+"s);

	// Castling
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("rnbqk1nr/ppp5/4p3/2bpPpPp/8/5N1B/PPPP1P1P/RNB1K2R w KQkq - 2 8"), Move(Square::E1, Square::G1, MoveFlags::QUIET)).data() == L"8. O-O"s);
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("r3k1nr/pppb4/2n1pP2/2bp1BPp/8/3P1N2/PPP2P1P/RNB2RK1 b kq - 0 11"), Move(Square::E8, Square::C8, MoveFlags::QUIET)).data() == L"O-O-O"s);
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("rnbqk2r/ppppp2p/7b/3n4/8/5KP1/PPPPP2P/RNBQ1BNR b kq - 0 6"), Move(Square::E8, Square::G8, MoveFlags::QUIET)).data() == L"O-O+"s);

	// Promotion
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("rnbq3r/pppk2P1/4p3/2bpPp2/8/8/PPPP1P1P/RNB1KBNR w KQ - 1 11"), Move(Square::G7, Square::G8, MoveFlags::BISHOP_PROMOTION)).data() == L"11. g8=♝"s);
	// Promotion + Capture
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("rnbq3r/pppk2P1/4p3/2bpPp2/8/8/PPPP1P1P/RNB1KBNR w KQ - 1 11"), Move(Square::G7, Square::H8, MoveFlags::KNIGHT_PROMOTION_CAPTURE)).data() == L"11. gxh8=♞"s);
	// Promotion + Check
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("rnbqk2r/ppp3P1/4p3/2bpPp2/8/5P2/PPPP3P/RNB1KBNR w KQ - 1 12"), Move(Square::G7, Square::G8, MoveFlags::QUEEN_PROMOTION)).data() == L"12. g8=♛+"s);
	// Promotion + Capture + Check
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("rnbqk2r/ppp3P1/4p3/2bpPp2/8/5P2/PPPP3P/RNB1KBNR w KQ - 1 12"), Move(Square::G7, Square::H8, MoveFlags::ROOK_PROMOTION_CAPTURE)).data() == L"12. gxh8=♜+"s);

	// Checkmate
	CHECK(PositionMoves::algebraicNotation(*FenToPosition::parse("2r3k1/2q1pr2/3p2p1/p2P4/3Q4/1Pp2P2/P5P1/2K4R w - - 0 25"), Move(Square::D4, Square::H8, MoveFlags::QUIET)).data() == L"25. ♛h8#"s);
}