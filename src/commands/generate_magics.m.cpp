#include <phase4/engine/moves/magic/magic_bitboards.h>

#include <iostream>

int main() {
	using namespace phase4::engine::moves::magic;

	{
		auto bishopAttacks = std::make_unique<MagicBitboards::BishopMagicContainers>();
		MagicBitboards::generateBishopAttacks(*bishopAttacks);
		if (!bishopAttacks->isValid) {
			std::cout << "Could not generate bishop magics" << std::endl;
			return 1;
		}
		std::cout << "Bishop magics:" << std::endl;
		for (size_t i = 0; i < bishopAttacks->containers.size(); ++i) {
			std::cout << bishopAttacks->containers[i].magicNumber.get_raw_value() << "ULL," << std::endl;
		}
	}

	{
		std::cout << "Rook magics:" << std::endl;
		auto rookAttacks = std::make_unique<MagicBitboards::RookMagicContainers>();
		MagicBitboards::generateRookAttacks(*rookAttacks);
		if (!rookAttacks->isValid) {
			std::cout << "Could not generate rook magics" << std::endl;
			return 1;
		}
		for (size_t i = 0; i < rookAttacks->containers.size(); ++i) {
			std::cout << rookAttacks->containers[i].magicNumber.get_raw_value() << "ULL," << std::endl;
		}
	}

	return 0;
}
