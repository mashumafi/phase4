#ifndef PHASE4_ENGINE_MOVES_MAGIC_MAGIC_BITBOARDS_H
#define PHASE4_ENGINE_MOVES_MAGIC_MAGIC_BITBOARDS_H

#include <phase4/engine/moves/magic/attacks_generator.h>
#include <phase4/engine/moves/magic/magic_container.h>
#include <phase4/engine/moves/magic/magic_keys.h>
#include <phase4/engine/moves/magic/magic_shifts.h>
#include <phase4/engine/moves/magic/permutations_generator.h>
#include <phase4/engine/moves/patterns/diagonal_pattern_generator.h>
#include <phase4/engine/moves/patterns/file_pattern_generator.h>
#include <phase4/engine/moves/patterns/rank_pattern_generator.h>

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/position_constants.h>
#include <phase4/engine/common/random.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/common/util.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <optional>

namespace phase4::engine::moves::magic {

class MagicBitboards {
public:
	template <size_t SHIFT>
	struct MagicContainers {
		std::array<MagicContainer<1ull << SHIFT>, 64> containers = {};
	};
	using RookMagicContainers = MagicContainers<MagicShifts::MAX_ROOK_SHIFT>;
	using BishopMagicContainers = MagicContainers<MagicShifts::MAX_BISHOP_SHIFT>;

	using Masks = std::array<common::Bitboard, 64>;

	static inline common::Bitboard getRookMoves(common::Bitboard board, common::Square square) noexcept;

	static inline common::Bitboard getBishopMoves(common::Bitboard board, common::Square square) noexcept;

	static inline std::unique_ptr<RookMagicContainers> generateRookAttacks(const std::optional<MagicKeys::Array> &keys = {}) noexcept;

	static inline std::unique_ptr<BishopMagicContainers> generateBishopAttacks(const std::optional<MagicKeys::Array> &keys = {}) noexcept;

private:
	static inline constexpr Masks generateRookMasks() noexcept {
		Masks masks;
		for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
			masks[fieldIndex] =
					(patterns::FilePatternGenerator::getPatternForField(fieldIndex) & ~common::PositionConstants::TOP_BOTTOM_EDGE) |
					(patterns::RankPatternGenerator::getPatternForField(fieldIndex) & ~common::PositionConstants::RIGHT_LEFT_EDGE);
		}
		return masks;
	}

	static inline constexpr Masks generateBishopMasks() noexcept {
		Masks masks;
		for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
			masks[fieldIndex] = patterns::DiagonalPatternGenerator::getPattern(fieldIndex) & ~common::PositionConstants::EDGES;
		}
		return masks;
	}

	template <size_t N>
	static inline constexpr bool validate(MagicContainer<N> &container, int32_t shift, const std::array<common::Bitboard, N> &permutations, const std::array<common::Bitboard, N> &attacks) noexcept {
		const size_t length = 1ull << shift;
		for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
			const common::Bitboard hash = permutations[permutationIndex] * container.magicNumber;
			const common::Bitboard attackIndex = hash >> container.shift;
			const common::Bitboard attack = container.attacks[attackIndex.get_raw_value()];

			if (attack != 0 && attack != attacks[permutationIndex]) {
				return false;
			}

			container.attacks[attackIndex.get_raw_value()] = attacks[permutationIndex];
		}

		return true;
	}

	template <size_t N>
	static inline constexpr bool generateAttacks(
			MagicContainer<N> &container,
			common::Bitboard mask,
			int32_t shift,
			const std::array<common::Bitboard, N> &permutations,
			const std::array<common::Bitboard, N> &attacks,
			std::optional<uint64_t> key = {}) noexcept {
		common::Random rand(123456);
		const uint64_t first = key ? key.value() : rand.fewBits();

		container = MagicContainer<N>{
			mask,
			first,
			{},
			64 - shift,
		};

		const size_t retryLimit = key ? 1 : 100'000;
		for (size_t i = 0; i < retryLimit; ++i) {
			if (validate(container, shift, permutations, attacks)) {
				return true;
			}

			container.magicNumber = rand.fewBits();

			if (container.magicNumber == first) {
				return false;
			}

			container.attacks.fill(0);
		}

		return false;
	}

	static const std::unique_ptr<RookMagicContainers> ROOK_MAGIC_ARRAY;
	static const std::unique_ptr<MagicBitboards::BishopMagicContainers> BISHOP_MAGIC_ARRAY;
};

inline common::Bitboard MagicBitboards::getRookMoves(common::Bitboard board, common::Square square) noexcept {
	board = board & ROOK_MAGIC_ARRAY->containers[square].mask;
	board = board * ROOK_MAGIC_ARRAY->containers[square].magicNumber;
	board = board >> ROOK_MAGIC_ARRAY->containers[square].shift;
	return ROOK_MAGIC_ARRAY->containers[square].attacks[board.get_raw_value()];
}

inline common::Bitboard MagicBitboards::getBishopMoves(common::Bitboard board, common::Square square) noexcept {
	board = board & BISHOP_MAGIC_ARRAY->containers[square].mask;
	board = board * BISHOP_MAGIC_ARRAY->containers[square].magicNumber;
	board = board >> BISHOP_MAGIC_ARRAY->containers[square].shift;
	return BISHOP_MAGIC_ARRAY->containers[square].attacks[board.get_raw_value()];
}

inline std::unique_ptr<MagicBitboards::RookMagicContainers> MagicBitboards::generateRookAttacks(const std::optional<MagicKeys::Array> &keys) noexcept {
	auto magicArray = std::make_unique<RookMagicContainers>();
	constexpr Masks masks = generateRookMasks();

	auto permutations = std::array<common::Bitboard, 1ull << MagicShifts::MAX_ROOK_SHIFT>();
	auto attacks = std::array<common::Bitboard, 1ull << MagicShifts::MAX_ROOK_SHIFT>();
	for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
		const size_t length = 1ull << MagicShifts::ROOK_SHIFTS[fieldIndex];
		for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
			permutations[permutationIndex] = PermutationsGenerator::getPermutation(masks[fieldIndex], permutationIndex);
			attacks[permutationIndex] = AttacksGenerator::getFileRankAttacks(permutations[permutationIndex], common::Square(fieldIndex));
		}
		generateAttacks(magicArray->containers[fieldIndex], masks[fieldIndex], MagicShifts::ROOK_SHIFTS[fieldIndex], permutations, attacks, keys ? std::optional<uint64_t>(keys.value()[fieldIndex]) : std::nullopt);
	}

	return magicArray;
}

inline std::unique_ptr<MagicBitboards::BishopMagicContainers> MagicBitboards::generateBishopAttacks(const std::optional<MagicKeys::Array> &keys) noexcept {
	auto magicArray = std::make_unique<BishopMagicContainers>();
	constexpr Masks masks = generateBishopMasks();

	auto permutations = std::array<common::Bitboard, 1ull << MagicShifts::MAX_BISHOP_SHIFT>();
	auto attacks = std::array<common::Bitboard, 1ull << MagicShifts::MAX_BISHOP_SHIFT>();
	for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
		const size_t length = 1ull << MagicShifts::BISHOP_SHIFTS[fieldIndex];
		for (size_t permutationIndex = 0; permutationIndex < length; ++permutationIndex) {
			permutations[permutationIndex] = PermutationsGenerator::getPermutation(masks[fieldIndex], permutationIndex);
			attacks[permutationIndex] = AttacksGenerator::getDiagonalAttacks(permutations[permutationIndex], common::Square(fieldIndex));
		}

		generateAttacks(magicArray->containers[fieldIndex], masks[fieldIndex], MagicShifts::BISHOP_SHIFTS[fieldIndex], permutations, attacks, keys ? std::optional<uint64_t>(keys.value()[fieldIndex]) : std::nullopt);
	}
	return magicArray;
}

inline const std::unique_ptr<MagicBitboards::BishopMagicContainers> MagicBitboards::BISHOP_MAGIC_ARRAY = generateBishopAttacks(MagicKeys::BISHOP_KEYS);
inline const std::unique_ptr<MagicBitboards::RookMagicContainers> MagicBitboards::ROOK_MAGIC_ARRAY = generateRookAttacks(MagicKeys::ROOK_KEYS);

} //namespace phase4::engine::moves::magic

#endif
