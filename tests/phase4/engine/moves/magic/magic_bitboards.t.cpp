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
	CHECK(rookAttacks.containers[0].magicNumber.asSize() == 36028866283716608ull);
	CHECK(rookAttacks.containers[1].magicNumber.asSize() == 54078380169052160ull);
	CHECK(rookAttacks.containers[2].magicNumber.asSize() == 293859888579350528ull);
	CHECK(rookAttacks.containers[4].magicNumber.asSize() == 293859888579350528ull);
	CHECK(rookAttacks.containers[8].magicNumber.asSize() == 18155154251546664ull);
	CHECK(rookAttacks.containers[16].magicNumber.asSize() == 598684085518480ull);
	CHECK(rookAttacks.containers[32].magicNumber.asSize() == 576603963768439842ull);
}
