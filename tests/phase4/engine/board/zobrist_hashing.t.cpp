#include <phase4/engine/board/zobrist_hashing.h>

#include <doctest/doctest.h>

TEST_CASE("ZobristHashing different order, same hash") {
	using namespace phase4::engine::board;

	constexpr ZobristHashing hash1 = ZobristHashing()
											 .addOrRemovePiece(0, 3, 3)
											 .changeSide()
											 .toggleEnPassant(3)
											 .addOrRemovePiece(1, 4, 5);

	constexpr ZobristHashing hash2 = ZobristHashing()
											 .addOrRemovePiece(1, 4, 5)
											 .toggleEnPassant(3)
											 .changeSide()
											 .addOrRemovePiece(0, 3, 3);

	CHECK(hash1 == hash2);
}