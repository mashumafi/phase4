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
#include <exception> // exceptions
#include <memory>
#include <optional>

namespace phase4::engine::moves::magic {

class MagicBitboards {
public:
	template <size_t SHIFT>
	struct MagicContainers {
		bool isValid = false;
		std::array<MagicContainer<1ULL << SHIFT>, 64> containers = {};
	};
	using RookMagicContainers = MagicContainers<MagicShifts::MAX_ROOK_SHIFT>;
	using BishopMagicContainers = MagicContainers<MagicShifts::MAX_BISHOP_SHIFT>;

	using Masks = std::array<common::Bitset, 64>;

	static void initWithInternalKeys() {
		generateRookAttacks(ROOK_MAGIC_ARRAY, MagicKeys::ROOK_KEYS);
		generateBishopAttacks(BISHOP_MAGIC_ARRAY, MagicKeys::BISHOP_KEYS);
	}

	static common::Bitset getRookMoves(common::Bitset board, common::Square square);

	static common::Bitset getBishopMoves(common::Bitset board, common::Square square);

	static void generateRookAttacks(RookMagicContainers &magicArray, const std::optional<MagicKeys::Array> &keys = {});

	static void generateBishopAttacks(BishopMagicContainers &magicArray, const std::optional<MagicKeys::Array> &keys = {});

private:
	static constexpr Masks generateRookMasks() {
		Masks masks;
		for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
			masks[fieldIndex] =
					(patterns::FilePatternGenerator::getPatternForField(common::Square(fieldIndex)) & ~board::BoardConstants::TOP_BOTTOM_EDGE) |
					(patterns::RankPatternGenerator::getPatternForField(common::Square(fieldIndex)) & ~board::BoardConstants::RIGHT_LEFT_EDGE);
		}
		return masks;
	}

	static constexpr Masks generateBishopMasks() {
		Masks masks;
		for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
			masks[fieldIndex] = patterns::DiagonalPatternGenerator::getPattern(common::Square(fieldIndex)) & ~board::BoardConstants::EDGES;
		}
		return masks;
	}

	template <size_t N>
	static bool generateAttacks(
			MagicContainer<N> &container,
			common::Bitset mask,
			int32_t shift,
			const std::array<common::Bitset, N> &permutations,
			const std::array<common::Bitset, N> &attacks,
			std::optional<uint64_t> key = {}) {
		common::Random rand(123456);
		const uint64_t first = key ? key.value() : rand.fewBits();

		container = MagicContainer<N>{
			mask,
			first,
			{},
			64 - shift,
		};

		constexpr std::array<common::Bitset, N> empty;

		const auto validate = [shift, &empty, &permutations, &container, &attacks, &rand]() -> bool {
			const size_t length = 1ull << shift;
			for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
				const common::Bitset hash = permutations[permutationIndex] * container.magicNumber;
				const common::Bitset attackIndex = hash >> container.shift;
				const common::Bitset attack = container.attacks[attackIndex.asSize()];

				if (attack != 0 && attack != attacks[permutationIndex]) {
					container.magicNumber = rand.fewBits();
					container.attacks = empty;
					return false;
				}

				container.attacks[attackIndex.asSize()] = attacks[permutationIndex];
			}

			return true;
		};

		const size_t limit = key ? 1 : 100000;
		for (size_t i = 0; i < limit; ++i) {
			if (validate()) {
				return true;
			}

			if (container.magicNumber == first) {
				std::cout << "Repeated the first number in the random sequence" << std::endl;
				return false;
			}
		}

		return false;
	}

	static RookMagicContainers ROOK_MAGIC_ARRAY;
	static BishopMagicContainers BISHOP_MAGIC_ARRAY;
};

inline MagicBitboards::RookMagicContainers MagicBitboards::ROOK_MAGIC_ARRAY;
inline MagicBitboards::BishopMagicContainers MagicBitboards::BISHOP_MAGIC_ARRAY;

inline common::Bitset MagicBitboards::getRookMoves(common::Bitset board, common::Square square) {
	assert(ROOK_MAGIC_ARRAY.isValid);
	board = board & ROOK_MAGIC_ARRAY.containers[square].mask;
	board = board * ROOK_MAGIC_ARRAY.containers[square].magicNumber;
	board = board >> ROOK_MAGIC_ARRAY.containers[square].shift;
	return ROOK_MAGIC_ARRAY.containers[square].attacks[board.asSize()];
}

inline common::Bitset MagicBitboards::getBishopMoves(common::Bitset board, common::Square square) {
	assert(BISHOP_MAGIC_ARRAY.isValid);
	board = board & BISHOP_MAGIC_ARRAY.containers[square].mask;
	board = board * BISHOP_MAGIC_ARRAY.containers[square].magicNumber;
	board = board >> BISHOP_MAGIC_ARRAY.containers[square].shift;
	return BISHOP_MAGIC_ARRAY.containers[square].attacks[board.asSize()];
}

inline void MagicBitboards::generateRookAttacks(RookMagicContainers &magicArray, const std::optional<MagicKeys::Array> &keys) {
	constexpr Masks masks = generateRookMasks();

	auto permutations = std::array<common::Bitset, 1ull << MagicShifts::MAX_ROOK_SHIFT>();
	auto attacks = std::array<common::Bitset, 1ull << MagicShifts::MAX_ROOK_SHIFT>();
	for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
		const size_t length = 1ull << MagicShifts::ROOK_SHIFTS[fieldIndex];
		for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
			permutations[permutationIndex] = PermutationsGenerator::getPermutation(masks[fieldIndex], permutationIndex);
			attacks[permutationIndex] = AttacksGenerator::getFileRankAttacks(permutations[permutationIndex], common::Square(fieldIndex));
		}

		generateAttacks(magicArray.containers[fieldIndex], masks[fieldIndex], MagicShifts::ROOK_SHIFTS[fieldIndex], permutations, attacks, keys ? std::optional<uint64_t>(keys.value()[fieldIndex]) : std::nullopt);
	}
	magicArray.isValid = true;
}

inline void MagicBitboards::generateBishopAttacks(BishopMagicContainers &magicArray, const std::optional<MagicKeys::Array> &keys) {
	constexpr Masks masks = generateBishopMasks();

	auto permutations = std::array<common::Bitset, 1ull << MagicShifts::MAX_BISHOP_SHIFT>();
	auto attacks = std::array<common::Bitset, 1ull << MagicShifts::MAX_BISHOP_SHIFT>();
	for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
		const size_t length = 1ull << MagicShifts::BISHOP_SHIFTS[fieldIndex];
		for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
			permutations[permutationIndex] = PermutationsGenerator::getPermutation(masks[fieldIndex], permutationIndex);
			attacks[permutationIndex] = AttacksGenerator::getDiagonalAttacks(permutations[permutationIndex], common::Square(fieldIndex));
		}

		generateAttacks(magicArray.containers[fieldIndex], masks[fieldIndex], MagicShifts::BISHOP_SHIFTS[fieldIndex], permutations, attacks, keys ? std::optional<uint64_t>(keys.value()[fieldIndex]) : std::nullopt);
	}
	magicArray.isValid = true;
}

} //namespace phase4::engine::moves::magic

#endif
