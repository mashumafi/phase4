#ifndef PHASE4_ENGINE_MOVES_MAGIC_MAGIC_BITBOARDS_H
#define PHASE4_ENGINE_MOVES_MAGIC_MAGIC_BITBOARDS_H

#include <phase4/engine/board/board_constants.h> // TODO: Remove circular dependency
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/random.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/moves/magic/attacks_generator.h>
#include <phase4/engine/moves/magic/magic_container.h>
#include <phase4/engine/moves/magic/magic_keys.h>
#include <phase4/engine/moves/magic/magic_shifts.h>
#include <phase4/engine/moves/magic/permutations_generator.h>
#include <phase4/engine/moves/patterns/diagonal_pattern_generator.h>
#include <phase4/engine/moves/patterns/file_pattern_generator.h>
#include <phase4/engine/moves/patterns/rank_pattern_generator.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <optional>

namespace phase4::engine::moves::magic {

class MagicBitboards {
private:
	using MagicContainers = std::array<MagicContainer, 64>;

	static std::unique_ptr<MagicContainers> _rookMagicArray;
	static std::unique_ptr<MagicContainers> _bishopMagicArray;

	using Masks = std::array<common::Bitset, 64>;

public:
	static void InitWithInternalKeys() {
		_rookMagicArray = generateRookAttacks(MagicKeys::ROOK_KEYS);
		_bishopMagicArray = generateBishopAttacks(MagicKeys::BISHOP_KEYS);
	}

	static common::Bitset getRookMoves(common::Bitset board, common::Square square) {
		board = board & (*_rookMagicArray)[square].Mask;
		board = board * (*_rookMagicArray)[square].MagicNumber;
		board = board >> (*_rookMagicArray)[square].Shift;
		return (*_rookMagicArray)[square].Attacks[board.asSize()];
	}

	static common::Bitset getBishopMoves(common::Bitset board, common::Square square) {
		board = board & (*_bishopMagicArray)[square].Mask;
		board = board * (*_bishopMagicArray)[square].MagicNumber;
		board = board >> (*_bishopMagicArray)[square].Shift;
		return (*_bishopMagicArray)[square].Attacks[board.asSize()];
	}

private:
	static std::unique_ptr<MagicContainers> generateRookAttacks(const std::optional<MagicKeys::Array> &keys = {}) {
		Masks masks;
		auto permutations = allocate_unique<MagicShifts::Permutations>();
		auto attacks = allocate_unique<MagicShifts::Attacks>();

		for (int fieldIndex = 0; fieldIndex < 64; ++fieldIndex) {
			masks[fieldIndex] =
					(patterns::FilePatternGenerator::getPatternForField(common::Square(fieldIndex)) & ~board::BoardConstants::TOP_BOTTOM_EDGE) |
					(patterns::RankPatternGenerator::getPatternForField(common::Square(fieldIndex)) & ~board::BoardConstants::RIGHT_LEFT_EDGE);

			const size_t length = 1ull << MagicShifts::ROOK_SHIFTS[fieldIndex];
			for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
				(*permutations)[fieldIndex][permutationIndex] = PermutationsGenerator::getPermutation(masks[fieldIndex], permutationIndex);
				(*attacks)[fieldIndex][permutationIndex] = AttacksGenerator::getFileRankAttacks((*permutations)[fieldIndex][permutationIndex], common::Square(fieldIndex));
			}
		}

		return generateAttacks(masks, permutations, attacks, MagicShifts::ROOK_SHIFTS, keys);
	}

	static std::unique_ptr<MagicContainers> generateBishopAttacks(const std::optional<MagicKeys::Array> &keys = {}) {
		Masks masks;
		auto permutations = allocate_unique<MagicShifts::Permutations>();
		auto attacks = allocate_unique<MagicShifts::Attacks>();

		for (int fieldIndex = 0; fieldIndex < 64; ++fieldIndex) {
			masks[fieldIndex] = patterns::DiagonalPatternGenerator::getPattern(common::Square(fieldIndex)) & ~board::BoardConstants::EDGES;

			const size_t length = 1ull << MagicShifts::BISHOP_SHIFTS[fieldIndex];
			for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
				(*permutations)[fieldIndex][permutationIndex] = PermutationsGenerator::getPermutation(masks[fieldIndex], permutationIndex);
				(*attacks)[fieldIndex][permutationIndex] = AttacksGenerator::getDiagonalAttacks((*permutations)[fieldIndex][permutationIndex], common::Square(fieldIndex));
			}
		}

		return generateAttacks(masks, *permutations, *attacks, MagicShifts::BISHOP_SHIFTS, keys);
	}

	static std::unique_ptr<MagicContainers> generateAttacks(const Masks &masks, const MagicShifts::Permutations &permutations, const MagicShifts::Attacks &attacks, const MagicShifts::Array &shifts, const std::optional<MagicKeys::Array> &keys = {}) {
		std::unique_ptr<MagicContainers> magicArray = std::make_unique<MagicContainers>();
		common::Random rand(123456);

		for (int fieldIndex = 0; fieldIndex < 64; ++fieldIndex) {
			(*magicArray)[fieldIndex] = MagicContainer{
				masks[fieldIndex],
				keys ? keys.value()[fieldIndex] : rand.next(),
				{},
				64 - shifts[fieldIndex],
			};

			bool success = false;
			while (!success) {
				success = true;

				const size_t length = 1ull << shifts[fieldIndex];
				for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
					const common::Bitset hash = permutations[fieldIndex][permutationIndex] * (*magicArray)[fieldIndex].MagicNumber;
					const common::Bitset attackIndex = hash >> (*magicArray)[fieldIndex].Shift;
					const common::Bitset attack = (*magicArray)[fieldIndex].Attacks[attackIndex.asSize()];

					if (attack != 0 && attack != attacks[fieldIndex][permutationIndex]) {
						(*magicArray)[fieldIndex].MagicNumber = rand.next();
						std::fill((*magicArray)[fieldIndex].Attacks.begin(), (*magicArray)[fieldIndex].Attacks.end(), 0);
						success = false;
						break;
					}

					(*magicArray)[fieldIndex].Attacks[attackIndex.asSize()] = attacks[fieldIndex][permutationIndex];
				}
			}
		}

		return magicArray;
	}
};

std::unique_ptr<MagicBitboards::MagicContainers> MagicBitboards::_rookMagicArray;
std::unique_ptr<MagicBitboards::MagicContainers> MagicBitboards::_bishopMagicArray;

} //namespace phase4::engine::moves::magic

#endif
