#include <phase4/engine/moves/magic/magic_bitboards.h>

#include <iostream>

int main() {
	using namespace phase4::engine::moves::magic;

	{
		std::unique_ptr<MagicBitboards::BishopMagicContainers> bishopAttacks = MagicBitboards::generateBishopAttacks();
		std::cout << "Bishop magics:" << std::endl;
		for (size_t i = 0; i < bishopAttacks->containers.size(); ++i) {
			std::cout << bishopAttacks->containers[i].magicNumber.get_raw_value() << "ULL," << std::endl;
		}
	}

	{
		std::cout << "Rook magics:" << std::endl;
		std::unique_ptr<MagicBitboards::RookMagicContainers> rookAttacks = MagicBitboards::generateRookAttacks();
		for (size_t i = 0; i < rookAttacks->containers.size(); ++i) {
			std::cout << rookAttacks->containers[i].magicNumber.get_raw_value() << "ULL," << std::endl;
		}
	}

	return 0;
}
