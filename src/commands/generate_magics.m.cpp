#include <phase4/engine/moves/magic/magic_bitboards.h>

#include <iostream>

int main() {
	using namespace phase4::engine::moves::magic;

	MagicBitboards::BishopMagicContainers bishopAttacks;
	MagicBitboards::generateBishopAttacks(bishopAttacks);
	std::cout << "Bishop magics:" << std::endl;
	for (size_t i = 0; i < bishopAttacks.size(); ++i) {
		std::cout << bishopAttacks[i].magicNumber.asSize() << "ULL," << std::endl;
	}

	std::cout << "Rook magics:" << std::endl;
	MagicBitboards::RookMagicContainers rookAttacks;
	MagicBitboards::generateRookAttacks(rookAttacks);
	for (size_t i = 0; i < rookAttacks.size(); ++i) {
		std::cout << rookAttacks[i].magicNumber.asSize() << "ULL," << std::endl;
	}
}
