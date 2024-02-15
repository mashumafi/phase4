#ifndef PHASE4_ENGINE_COMMON_WALL_OPERATIONS_H
#define PHASE4_ENGINE_COMMON_WALL_OPERATIONS_H

#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/position_constants.h>
#include <phase4/engine/common/square.h>

#include <functional>

namespace phase4::engine::common {

class WallOperations {
public:
	static constexpr int16_t FileMax = 8;
	static constexpr int16_t RankMax = 8;
	static constexpr int16_t Rank2 = 1;
	static constexpr int16_t FileB = 1;

	static constexpr FieldIndex North = { 0, 1 };
	static constexpr FieldIndex East = { -1, 0 };
	static constexpr FieldIndex South = -North;
	static constexpr FieldIndex West = -East;
	static constexpr FieldIndex NorthWest = North + West;

	static constexpr Bitset TOP = PositionConstants::RANK_7 | PositionConstants::RANK_8;
	static constexpr Bitset BOTTOM = PositionConstants::RANK_1 | PositionConstants::RANK_2;
	static constexpr Bitset LEFT = PositionConstants::FILE_A | PositionConstants::FILE_B;
	static constexpr Bitset RIGHT = PositionConstants::FILE_G | PositionConstants::FILE_H;

	static constexpr int16_t Step(int16_t n, int16_t d) {
		return n / d * d;
	}

	static constexpr int64_t SquareBB(uint16_t fieldIndex) {
		return 1ull << fieldIndex;
	}

	static constexpr int64_t SquareBB(FieldIndex position) {
		return SquareBB(position.x + position.y * 8);
	}

	static constexpr int16_t GetFile(int16_t fieldIndex) {
		return fieldIndex % 8;
	}

	static constexpr int16_t GetRank(int16_t fieldIndex) {
		return fieldIndex / 8;
	}

	static constexpr std::array<common::Bitset, 64> populateSlideFromBB();

	static constexpr std::array<std::array<common::Bitset, 64>, 64> populateSlideToBB();

	static constexpr std::array<std::array<FieldIndex, 64>, 64> populateSlideDir();

	static constexpr std::array<std::array<common::Square, 64>, 64> populateSlideSquare();

	// utility, given an x,y gives you a valid wall mask for that coordinate
	static const std::array<common::Bitset, 64> SLIDE_FROM;

	// given a wall index and square, gives you a resulting wall mask
	// used to update the bitset used for the wall mask
	static const std::array<std::array<common::Bitset, 64>, 64> SLIDE_TO;

	// given a wall inddex and the landing square, gives the sliding direction
	// used to determine enpassand and undo history
	static const std::array<std::array<FieldIndex, 64>, 64> SLIDE_DIR;

	// given a wall inddex and the landing square, gives the resulting square movement
	// used to actually move pieces to their 'to' square
	static const std::array<std::array<common::Square, 64>, 64> SLIDE_SQUARE;
};

constexpr std::array<common::Bitset, 64> WallOperations::populateSlideFromBB() {
	std::array<common::Bitset, 64> result{};
	for (common::Square from = common::Square::BEGIN; from != common::Square::INVALID; ++from) {
		const FieldIndex bottomRight{ Step(GetFile(from), 2), Step(GetRank(from), 2) };
		result[from] = SquareBB(bottomRight) | SquareBB(bottomRight + North) | SquareBB(bottomRight + West) | SquareBB(bottomRight + NorthWest);
	}
	return result;
}

inline constexpr std::array<common::Bitset, 64> WallOperations::SLIDE_FROM = WallOperations::populateSlideFromBB();

constexpr std::array<std::array<common::Bitset, 64>, 64> WallOperations::populateSlideToBB() {
	std::array<std::array<common::Bitset, 64>, 64> result{};
	for (common::Square from = common::Square::BEGIN; from != common::Square::INVALID; ++from) {
		for (common::Square to = common::Square::BEGIN; to != common::Square::INVALID; ++to) {
			common::Bitset shiftedWall = SLIDE_FROM[from];
			const common::Bitset toBB = SLIDE_FROM[to];

			if ((RIGHT & from.asBitboard()) == 0 && (from.east(2).asBitboard() & toBB) != 0) {
				shiftedWall >>= 2;
			} else if ((LEFT & from.asBitboard()) == 0 && (from.west(2).asBitboard() & toBB) != 0) {
				shiftedWall <<= 2;
			} else if ((TOP & from.asBitboard()) == 0 && (from.north(2).asBitboard() & toBB) != 0) {
				shiftedWall <<= 16;
			} else if ((BOTTOM & from.asBitboard()) == 0 && (from.south(2).asBitboard() & toBB) != 0) {
				shiftedWall >>= 16;
			}

			result[from][to] = shiftedWall;
		}
	}
	return result;
}

constexpr std::array<std::array<FieldIndex, 64>, 64> WallOperations::populateSlideDir() {
	std::array<std::array<FieldIndex, 64>, 64> result{};
	for (common::Square from = common::Square::BEGIN; from != common::Square::INVALID; ++from) {
		for (common::Square to = common::Square::BEGIN; to != common::Square::INVALID; ++to) {
			FieldIndex d{ 0, 0 };
			const common::Bitset toBB = SLIDE_FROM[to];

			if ((RIGHT & from.asBitboard()) == 0 && (from.east(2).asBitboard() & toBB) != 0) {
				d = East * 2;
			} else if ((LEFT & from.asBitboard()) == 0 && (from.west(2).asBitboard() & toBB) != 0) {
				d = West * 2;
			} else if ((TOP & from.asBitboard()) == 0 && (from.north(2).asBitboard() & toBB) != 0) {
				d = North * 2;
			} else if ((BOTTOM & from.asBitboard()) == 0 && (from.south(2).asBitboard() & toBB) != 0) {
				d = South * 2;
			}

			result[from][to] = d;
		}
	}
	return result;
}

constexpr std::array<std::array<common::Square, 64>, 64> WallOperations::populateSlideSquare() {
	std::array<std::array<common::Square, 64>, 64> result{};
	for (common::Square from = common::Square::BEGIN; from != common::Square::INVALID; ++from) {
		for (common::Square to = common::Square::BEGIN; to != common::Square::INVALID; ++to) {
			common::Square destinationSquare = to;
			const common::Bitset toBB = SLIDE_FROM[to];

			if ((RIGHT & from.asBitboard()) == 0 && (from.east(2).asBitboard() & toBB) != 0) {
				destinationSquare = destinationSquare.west(2);
			} else if ((LEFT & from.asBitboard()) == 0 && (from.west(2).asBitboard() & toBB) != 0) {
				destinationSquare = destinationSquare.east(2);
			} else if ((TOP & from.asBitboard()) == 0 && (from.north(2).asBitboard() & toBB) != 0) {
				destinationSquare = destinationSquare.south(2);
			} else if ((BOTTOM & from.asBitboard()) == 0 && (from.south(2).asBitboard() & toBB) != 0) {
				destinationSquare = destinationSquare.north(2);
			}

			result[from][to] = destinationSquare;
		}
	}
	return result;
}

inline constexpr std::array<std::array<common::Bitset, 64>, 64> WallOperations::SLIDE_TO = WallOperations::populateSlideToBB();
inline constexpr std::array<std::array<FieldIndex, 64>, 64> WallOperations::SLIDE_DIR = WallOperations::populateSlideDir();
inline constexpr std::array<std::array<common::Square, 64>, 64> WallOperations::SLIDE_SQUARE = WallOperations::populateSlideSquare();

} //namespace phase4::engine::common

#endif
