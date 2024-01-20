#include <phase4/engine/board/zobrist_hashing.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("ZobristHashing different order, same hash") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::board;

	constexpr ZobristHashing hash1 = ZobristHashing()
											 .addOrRemovePiece(PieceColor::WHITE, PieceType::BISHOP, Square::A2)
											 .changeSide()
											 .toggleEnPassant(3)
											 .addOrRemovePiece(PieceColor::BLACK, PieceType::ROOK, Square::A6);

	constexpr ZobristHashing hash2 = ZobristHashing()
											 .addOrRemovePiece(PieceColor::BLACK, PieceType::ROOK, Square::A6)
											 .toggleEnPassant(3)
											 .changeSide()
											 .addOrRemovePiece(PieceColor::WHITE, PieceType::BISHOP, Square::A2);

	CHECK(hash1 == hash2);
}