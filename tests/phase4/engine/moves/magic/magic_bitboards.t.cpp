#include <phase4/engine/moves/magic/magic_bitboards.h>

#include <doctest/doctest.h>

TEST_CASE("MagicBitboards generateBishopAttacks") {
	using namespace phase4::engine::moves::magic;

	MagicBitboards::BishopMagicContainers bishopAttacks;
	MagicBitboards::generateBishopAttacks(bishopAttacks);
	CHECK(bishopAttacks.isValid);
	CHECK(bishopAttacks.containers[0].magicNumber.asSize() == 2325088863497781376ULL);
	CHECK(bishopAttacks.containers[1].magicNumber.asSize() == 585545056993181728ULL);
	CHECK(bishopAttacks.containers[2].magicNumber.asSize() == 18595494716637184ULL);
	CHECK(bishopAttacks.containers[4].magicNumber.asSize() == 4901055626601824324ULL);
	CHECK(bishopAttacks.containers[8].magicNumber.asSize() == 288301036979093760ULL);
	CHECK(bishopAttacks.containers[16].magicNumber.asSize() == 1168684245170849824ULL);
	CHECK(bishopAttacks.containers[32].magicNumber.asSize() == 6953874552997285968ULL);
}

TEST_CASE("MagicBitboards generateRookAttacks") {
}
