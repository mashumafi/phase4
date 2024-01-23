#include <phase4/engine/common/piece_type.h>

#include <doctest/doctest.h>

TEST_CASE("PieceType increment and compare") {
	using namespace phase4::engine::common;

	PieceType piece = PieceType::PAWN;
	++piece;
	CHECK(piece == PieceType::KNIGHT);
	++piece;
	CHECK(piece == PieceType::BISHOP);
	++piece;
	CHECK(piece == PieceType::ROOK);
	++piece;
	CHECK(piece == PieceType::QUEEN);
	++piece;
	CHECK(piece == PieceType::KING);
	++piece;
	CHECK(piece == PieceType::INVALID);
}
