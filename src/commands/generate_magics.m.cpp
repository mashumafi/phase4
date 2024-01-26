#include <phase4/engine/moves/magic/magic_bitboards.h>

#include <iostream>

int main() {
	using namespace phase4::engine::moves::magic;

	MagicBitboards::BishopMagicContainers bishopAttacks;
	MagicBitboards::generateBishopAttacks(bishopAttacks);
	if (!bishopAttacks.isValid) {
	}
	std::cout << "Bishop magics:" << std::endl;
	for (size_t i = 0; i < bishopAttacks.containers.size(); ++i) {
		std::cout << bishopAttacks.containers[i].magicNumber.asSize() << "ULL," << std::endl;
	}

	std::cout << "Rook magics:" << std::endl;
	MagicBitboards::RookMagicContainers rookAttacks;
	MagicBitboards::generateRookAttacks(rookAttacks);
	if (!rookAttacks.isValid) {
	}
	for (size_t i = 0; i < rookAttacks.containers.size(); ++i) {
		std::cout << rookAttacks.containers[i].magicNumber.asSize() << "ULL," << std::endl;
	}
}
