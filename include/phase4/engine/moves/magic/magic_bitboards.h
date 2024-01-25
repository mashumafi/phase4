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
private:
	using RookMagicContainers = std::array<MagicContainer<1ULL << MagicShifts::MAX_ROOK_SHIFT>, 64>;
	using BishopMagicContainers = std::array<MagicContainer<1ULL << MagicShifts::MAX_BISHOP_SHIFT>, 64>;

	static std::unique_ptr<RookMagicContainers> ROOK_MAGIC_ARRAY;
	static BishopMagicContainers BISHOP_MAGIC_ARRAY;

	using Masks = std::array<common::Bitset, 64>;

public:
	static void initWithInternalKeys() {
		ROOK_MAGIC_ARRAY = generateRookAttacks(MagicKeys::ROOK_KEYS);
		BISHOP_MAGIC_ARRAY = generateBishopAttacks(MagicKeys::BISHOP_KEYS);
	}

	static common::Bitset getRookMoves(common::Bitset board, common::Square square);

	static common::Bitset getBishopMoves(common::Bitset board, common::Square square);

	static std::unique_ptr<RookMagicContainers> generateRookAttacks(const std::optional<MagicKeys::Array> &keys = {});

	static BishopMagicContainers generateBishopAttacks(const std::optional<MagicKeys::Array> &keys = {});

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
	static void generateAttacks(
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

		bool success = false;
		while (!success) {
			success = true;

			const size_t length = 1ull << shift;
			for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
				const common::Bitset hash = permutations[permutationIndex] * container.magicNumber;
				const common::Bitset attackIndex = hash >> container.shift;
				const common::Bitset attack = container.attacks[attackIndex.asSize()];

				if (attack != 0 && attack != attacks[permutationIndex]) {
					const uint64_t next = rand.fewBits();
					if (next == first) {
						throw std::invalid_argument("Repeated the first number in the random sequence");
					}
					container.magicNumber = next;
					std::fill(container.attacks.begin(), container.attacks.end(), 0);
					success = false;
					break;
				}

				container.attacks[attackIndex.asSize()] = attacks[permutationIndex];
			}
		}
	}
};

inline std::unique_ptr<MagicBitboards::RookMagicContainers> MagicBitboards::ROOK_MAGIC_ARRAY;
inline MagicBitboards::BishopMagicContainers MagicBitboards::BISHOP_MAGIC_ARRAY;

inline common::Bitset MagicBitboards::getRookMoves(common::Bitset board, common::Square square) {
	assert(ROOK_MAGIC_ARRAY);

	board = board & (*ROOK_MAGIC_ARRAY)[square].mask;
	board = board * (*ROOK_MAGIC_ARRAY)[square].magicNumber;
	board = board >> (*ROOK_MAGIC_ARRAY)[square].shift;
	return (*ROOK_MAGIC_ARRAY)[square].attacks[board.asSize()];
}

inline common::Bitset MagicBitboards::getBishopMoves(common::Bitset board, common::Square square) {
	board = board & BISHOP_MAGIC_ARRAY[square].mask;
	board = board * BISHOP_MAGIC_ARRAY[square].magicNumber;
	board = board >> BISHOP_MAGIC_ARRAY[square].shift;
	return BISHOP_MAGIC_ARRAY[square].attacks[board.asSize()];
}

inline std::unique_ptr<MagicBitboards::RookMagicContainers> MagicBitboards::generateRookAttacks(const std::optional<MagicKeys::Array> &keys) {
	std::unique_ptr<RookMagicContainers> magicArray = std::make_unique<RookMagicContainers>();

	constexpr Masks masks = generateRookMasks();

	auto permutations = std::array<common::Bitset, 1ull << MagicShifts::MAX_ROOK_SHIFT>();
	auto attacks = std::array<common::Bitset, 1ull << MagicShifts::MAX_ROOK_SHIFT>();
	for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
		const size_t length = 1ull << MagicShifts::ROOK_SHIFTS[fieldIndex];
		for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
			permutations[permutationIndex] = PermutationsGenerator::getPermutation(masks[fieldIndex], permutationIndex);
			attacks[permutationIndex] = AttacksGenerator::getFileRankAttacks(permutations[permutationIndex], common::Square(fieldIndex));
		}

		generateAttacks((*magicArray)[fieldIndex], masks[fieldIndex], MagicShifts::ROOK_SHIFTS[fieldIndex], permutations, attacks, keys ? std::optional<uint64_t>(keys.value()[fieldIndex]) : std::nullopt);
	}

	return magicArray;
}

inline MagicBitboards::BishopMagicContainers MagicBitboards::generateBishopAttacks(const std::optional<MagicKeys::Array> &keys) {
	BishopMagicContainers magicArray;

	constexpr Masks masks = generateBishopMasks();

	auto permutations = std::array<common::Bitset, 1ull << MagicShifts::MAX_BISHOP_SHIFT>();
	auto attacks = std::array<common::Bitset, 1ull << MagicShifts::MAX_BISHOP_SHIFT>();
	for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
		const size_t length = 1ull << MagicShifts::BISHOP_SHIFTS[fieldIndex];
		for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
			permutations[permutationIndex] = PermutationsGenerator::getPermutation(masks[fieldIndex], permutationIndex);
			attacks[permutationIndex] = AttacksGenerator::getDiagonalAttacks(permutations[permutationIndex], common::Square(fieldIndex));
		}

		generateAttacks(magicArray[fieldIndex], masks[fieldIndex], MagicShifts::BISHOP_SHIFTS[fieldIndex], permutations, attacks, keys ? std::optional<uint64_t>(keys.value()[fieldIndex]) : std::nullopt);
	}

	return magicArray;
}

} //namespace phase4::engine::moves::magic

#endif
