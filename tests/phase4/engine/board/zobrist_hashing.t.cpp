#include <phase4/engine/board/zobrist_hashing.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("ZobristHashing different order, same hash") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::board;

	constexpr ZobristHashing hash1 = ZobristHashing()
											 .addOrRemovePiece(PieceColor::WHITE, PieceType::BISHOP, Square::G1)
											 .changeSide()
											 .toggleEnPassant(3)
											 .addOrRemovePiece(PieceColor::BLACK, PieceType::ROOK, Square::C1);

	constexpr ZobristHashing hash2 = ZobristHashing()
											 .addOrRemovePiece(PieceColor::BLACK, PieceType::ROOK, Square::C1)
											 .toggleEnPassant(3)
											 .changeSide()
											 .addOrRemovePiece(PieceColor::WHITE, PieceType::BISHOP, Square::G1);

	CHECK(hash1 == hash2);
}