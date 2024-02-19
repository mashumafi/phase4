#include <phase4/engine/board/ordering/see_piece.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("SeePiece D5 BLACK 3r3k/2nq3p/1pb3p1/2r5/8/P6P/1P2RPP1/1QR3K1 b - - 1 31") {
	using namespace phase4::engine;
	using namespace board::ordering;

	auto position = fen::FenToPosition::parse("3r3k/2nq3p/1pb3p1/2r5/8/P6P/1P2RPP1/1QR3K1 b - - 1 31");
	REQUIRE(position);

	const uint8_t attacks = SeePiece::getAttackingPiecesWithColor(*position, common::PieceColor::BLACK, common::Square::D5);
	const uint8_t expectedAttacks = SeePiece::KNIGHT1.asByte() | SeePiece::BISHOP.asByte() | SeePiece::ROOK1.asByte() | SeePiece::ROOK2.asByte() | SeePiece::QUEEN.asByte();
	CHECK(attacks == expectedAttacks);
}

TEST_CASE("SeePiece F8 BLACK r4rk1/1b2qpbp/p3n3/1p1p4/8/P1NP2R1/BPP3PP/R1BQ2K1 w - - 0 18") {
	using namespace phase4::engine;
	using namespace board::ordering;

	auto position = fen::FenToPosition::parse("r4rk1/1b2qpbp/p3n3/1p1p4/8/P1NP2R1/BPP3PP/R1BQ2K1 w - - 0 18");
	REQUIRE(position);

	const uint8_t attacks = SeePiece::getAttackingPiecesWithColor(*position, common::PieceColor::BLACK, common::Square::F8);
	const uint8_t expectedAttacks = SeePiece::KNIGHT1.asByte() | SeePiece::BISHOP.asByte() | SeePiece::ROOK1.asByte() | SeePiece::QUEEN.asByte() | SeePiece::KING.asByte();
	CHECK(attacks == expectedAttacks);
}

TEST_CASE("SeePiece F3 WHITE 4rrk1/p4p2/1b2b2p/6q1/4BRp1/2PP3P/PP1NQ1P1/5R1K w - - 1 25") {
	using namespace phase4::engine;
	using namespace board::ordering;

	auto position = fen::FenToPosition::parse("4rrk1/p4p2/1b2b2p/6q1/4BRp1/2PP3P/PP1NQ1P1/5R1K w - - 1 25");
	REQUIRE(position);

	const uint8_t attacks = SeePiece::getAttackingPiecesWithColor(*position, common::PieceColor::WHITE, common::Square::F3);
	const uint8_t expectedAttacks = SeePiece::PAWN.asByte() | SeePiece::KNIGHT1.asByte() | SeePiece::BISHOP.asByte() | SeePiece::ROOK1.asByte() | SeePiece::ROOK2.asByte() | SeePiece::QUEEN.asByte();
	CHECK(attacks == expectedAttacks);
}

TEST_CASE("SeePiece D2 WHITE r1bq1rk1/1p1p2bp/p1nPpnp1/2p3B1/4N3/5N2/PPP2PPP/R2QKB1R w KQ - 2 12") {
	using namespace phase4::engine;
	using namespace board::ordering;

	auto position = fen::FenToPosition::parse("r1bq1rk1/1p1p2bp/p1nPpnp1/2p3B1/4N3/5N2/PPP2PPP/R2QKB1R w KQ - 2 12");
	REQUIRE(position);

	const uint8_t attacks = SeePiece::getAttackingPiecesWithColor(*position, common::PieceColor::WHITE, common::Square::D2);
	const uint8_t expectedAttacks = SeePiece::KNIGHT1.asByte() | SeePiece::KNIGHT2.asByte() | SeePiece::BISHOP.asByte() | SeePiece::QUEEN.asByte() | SeePiece::KING.asByte();
	CHECK(attacks == expectedAttacks);
}

TEST_CASE("SeePiece G8 BLACK r2qr1Qk/1p4p1/p1pb1n1N/8/3P3B/3B4/PP3PPP/R5K1 b - - 2 21") {
	using namespace phase4::engine;
	using namespace board::ordering;

	auto position = fen::FenToPosition::parse("r2qr1Qk/1p4p1/p1pb1n1N/8/3P3B/3B4/PP3PPP/R5K1 b - - 2 21");
	REQUIRE(position);

	const uint8_t attacks = SeePiece::getAttackingPiecesWithColor(*position, common::PieceColor::BLACK, common::Square::G8);
	const uint8_t expectedAttacks = SeePiece::KNIGHT1.asByte() | SeePiece::ROOK1.asByte() | SeePiece::ROOK2.asByte() | SeePiece::QUEEN.asByte() | SeePiece::KING.asByte();
	CHECK(attacks == expectedAttacks);
}

TEST_CASE("SeePiece E4 WHITE 4r1k1/1b2rpp1/1p4qp/2p5/2P1n3/1P3P1P/PBQNR1P1/4R1K1 b - - 0 26") {
	using namespace phase4::engine;
	using namespace board::ordering;

	auto position = fen::FenToPosition::parse("4r1k1/1b2rpp1/1p4qp/2p5/2P1n3/1P3P1P/PBQNR1P1/4R1K1 b - - 0 26");
	REQUIRE(position);

	const uint8_t attacks = SeePiece::getAttackingPiecesWithColor(*position, common::PieceColor::WHITE, common::Square::E4);
	const uint8_t expectedAttacks = SeePiece::PAWN.asByte() | SeePiece::KNIGHT1.asByte() | SeePiece::ROOK1.asByte() | SeePiece::ROOK2.asByte() | SeePiece::QUEEN.asByte();
	CHECK(attacks == expectedAttacks);
}
