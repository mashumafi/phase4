#include <phase4/engine/moves/magic/magic_bitboards.h>

#include <doctest/doctest.h>

TEST_CASE("MagicBitboards generateBishopAttacks") {
	using namespace phase4::engine::moves::magic;

	MagicBitboards::BishopMagicContainers bishopAttacks;
	MagicBitboards::generateBishopAttacks(bishopAttacks);
	CHECK(bishopAttacks.isValid);
	CHECK(bishopAttacks.containers[0].magicNumber.asSize() == 4613379283715375620ull);
	CHECK(bishopAttacks.containers[1].magicNumber.asSize() == 1134706804948992ull);
	CHECK(bishopAttacks.containers[2].magicNumber.asSize() == 1193454665762160640ull);
	CHECK(bishopAttacks.containers[4].magicNumber.asSize() == 6199115847304256ull);
	CHECK(bishopAttacks.containers[8].magicNumber.asSize() == 10090389800902263041ull);
	CHECK(bishopAttacks.containers[16].magicNumber.asSize() == 4728788474122995968ull);
	CHECK(bishopAttacks.containers[32].magicNumber.asSize() == 9511919209872884740ull);
}

TEST_CASE("MagicBitboards generateRookAttacks") {
	using namespace phase4::engine::moves::magic;

	MagicBitboards::RookMagicContainers rookAttacks;
	MagicBitboards::generateRookAttacks(rookAttacks);
	CHECK(rookAttacks.isValid);
}
