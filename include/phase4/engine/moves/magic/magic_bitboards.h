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
#include <exception> // TODO: remove when no longer throwing
#include <optional>
#include <sstream> // TODO: remove when no longer throwing

namespace phase4::engine::moves::magic {

class MagicBitboards {
private:
	using MagicContainers = std::array<MagicContainer, 64>;

	static MagicContainers _rookMagicArray;
	static MagicContainers _bishopMagicArray;

	using Masks = std::array<common::Bitset, 64>;

public:
	static void InitWithInternalKeys() {
		generateRookAttacks(MagicKeys::RookKeys);
		generateBishopAttacks(MagicKeys::BishopKeys);
	}

	static common::Bitset getRookMoves(common::Bitset board, int fieldIndex) {
		board = board & _rookMagicArray[fieldIndex].Mask;
		board = board * _rookMagicArray[fieldIndex].MagicNumber;
		board = board >> _rookMagicArray[fieldIndex].Shift;
		return _rookMagicArray[fieldIndex].Attacks[board.asSize()];
	}

	static common::Bitset getBishopMoves(common::Bitset board, int fieldIndex) {
		board = board & _bishopMagicArray[fieldIndex].Mask;
		board = board * _bishopMagicArray[fieldIndex].MagicNumber;
		board = board >> _bishopMagicArray[fieldIndex].Shift;
		return _bishopMagicArray[fieldIndex].Attacks[board.asSize()];
	}

	static MagicContainers generateRookAttacks(const std::optional<MagicKeys::Array> &keys = {}) {
		Masks masks;
		MagicShifts::Permutations permutations{};
		MagicShifts::Attacks attacks{};

		for (int fieldIndex = 0; fieldIndex < 64; ++fieldIndex) {
			masks[fieldIndex] =
					(patterns::FilePatternGenerator::getPatternForField(common::Square(fieldIndex)) & ~board::BoardConstants::TOP_BOTTOM_EDGE) |
					(patterns::RankPatternGenerator::getPatternForField(common::Square(fieldIndex)) & ~board::BoardConstants::RIGHT_LEFT_EDGE);

			const int length = 1 << MagicShifts::RookShifts[fieldIndex];
			for (int permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
				permutations[fieldIndex][permutationIndex] = PermutationsGenerator::getPermutation(masks[fieldIndex], permutationIndex);
				attacks[fieldIndex][permutationIndex] = AttacksGenerator::getFileRankAttacks(permutations[fieldIndex][permutationIndex], common::Square(fieldIndex));
			}
		}

		return _rookMagicArray = generateAttacks(masks, permutations, attacks, MagicShifts::RookShifts, keys);
	}

	static MagicContainers generateBishopAttacks(const std::optional<MagicKeys::Array> &keys = {}) {
		Masks masks;
		MagicShifts::Permutations permutations{};
		MagicShifts::Attacks attacks{};

		for (int fieldIndex = 0; fieldIndex < 64; ++fieldIndex) {
			masks[fieldIndex] = patterns::DiagonalPatternGenerator::getPattern(common::Square(fieldIndex)) & ~board::BoardConstants::EDGES;

			const int length = 1 << MagicShifts::RookShifts[fieldIndex];
			for (int permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
				permutations[fieldIndex][permutationIndex] = PermutationsGenerator::getPermutation(masks[fieldIndex], permutationIndex);
				attacks[fieldIndex][permutationIndex] = AttacksGenerator::getDiagonalAttacks(permutations[fieldIndex][permutationIndex], common::Square(fieldIndex));
			}
		}

		return _bishopMagicArray = generateAttacks(masks, permutations, attacks, MagicShifts::BishopShifts, keys);
	}

private:
	static MagicContainers generateAttacks(const Masks &masks, const MagicShifts::Permutations &permutations, const MagicShifts::Attacks &attacks, const MagicShifts::Array &shifts, const std::optional<MagicKeys::Array> &keys = {}) {
		MagicContainers magicArray;

		for (int fieldIndex = 0; fieldIndex < 64; ++fieldIndex) {
			magicArray[fieldIndex] = MagicContainer{
				masks[fieldIndex],
				0,
				{},
				64 - shifts[fieldIndex],
			};

			bool success = false;
			while (!success) {
				success = true;
				magicArray[fieldIndex].MagicNumber = keys ? keys.value()[fieldIndex] : 1; // TODO: Random number

				const size_t length = 1 << shifts[fieldIndex];
				for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
					const common::Bitset hash = permutations[fieldIndex][permutationIndex] * magicArray[fieldIndex].MagicNumber;
					const common::Bitset attackIndex = hash >> magicArray[fieldIndex].Shift;

					if (magicArray[fieldIndex].Attacks[attackIndex.asSize()] != 0 && magicArray[fieldIndex].Attacks[attackIndex.asSize()] != attacks[fieldIndex][permutationIndex]) {
						if (keys) {
							std::stringstream ss;
							ss << "The supplied key was not a valid magic number. Failed on fieldIndex:" << fieldIndex << " permutation:" << permutationIndex << "/" << length << ".";
							throw std::invalid_argument(ss.str());
						}
						std::fill(magicArray[fieldIndex].Attacks.begin(), magicArray[fieldIndex].Attacks.end(), 0);
						success = false;
						break;
					}

					magicArray[fieldIndex].Attacks[attackIndex.asSize()] = attacks[fieldIndex][permutationIndex];
				}
			}
		}

		return magicArray;
	}
};

MagicBitboards::MagicContainers MagicBitboards::_rookMagicArray;
MagicBitboards::MagicContainers MagicBitboards::_bishopMagicArray;

} //namespace phase4::engine::moves::magic

#endif
