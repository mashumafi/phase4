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

	static constexpr FieldIndex NORTH = { 0, 1 };
	static constexpr FieldIndex EAST = { 1, 0 };
	static constexpr FieldIndex SOUTH = -NORTH;
	static constexpr FieldIndex WEST = -EAST;
	static constexpr FieldIndex NorthEast = NORTH + EAST;

	static constexpr Bitset TOP = PositionConstants::RANK_7 | PositionConstants::RANK_8;
	static constexpr Bitset BOTTOM = PositionConstants::RANK_1 | PositionConstants::RANK_2;
	static constexpr Bitset LEFT = PositionConstants::FILE_A | PositionConstants::FILE_B;
	static constexpr Bitset RIGHT = PositionConstants::FILE_G | PositionConstants::FILE_H;

	static constexpr int16_t floorStep(int16_t n, int16_t d) {
		return n / d * d;
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
		FieldIndex bottomLeftIndex = from.asFieldIndex();
		bottomLeftIndex.x = floorStep(bottomLeftIndex.x, 2);
		bottomLeftIndex.y = floorStep(bottomLeftIndex.y, 2);
		common::Square bottomLeftSquare(bottomLeftIndex);
		result[from] = bottomLeftSquare.asBitboard() | bottomLeftSquare.north(1).asBitboard() | bottomLeftSquare.east(1).asBitboard() | bottomLeftSquare.north(1).east(1).asBitboard();
	}
	return result;
}

inline constexpr std::array<common::Bitset, 64> WallOperations::SLIDE_FROM = WallOperations::populateSlideFromBB();

constexpr std::array<std::array<common::Bitset, 64>, 64> WallOperations::populateSlideToBB() {
	std::array<std::array<common::Bitset, 64>, 64> result{};
	for (common::Square wallIndex = common::Square::BEGIN; wallIndex != common::Square::INVALID; ++wallIndex) {
		for (common::Square landIndex = common::Square::BEGIN; landIndex != common::Square::INVALID; ++landIndex) {
			common::Bitset shiftedWall = SLIDE_FROM[wallIndex];
			const common::Bitset toBB = SLIDE_FROM[landIndex];

			if ((RIGHT & wallIndex.asBitboard()) == 0 && (wallIndex.east(2).asBitboard() & toBB) != 0) {
				shiftedWall >>= 2;
			} else if ((LEFT & wallIndex.asBitboard()) == 0 && (wallIndex.west(2).asBitboard() & toBB) != 0) {
				shiftedWall <<= 2;
			} else if ((TOP & wallIndex.asBitboard()) == 0 && (wallIndex.north(2).asBitboard() & toBB) != 0) {
				shiftedWall <<= 16;
			} else if ((BOTTOM & wallIndex.asBitboard()) == 0 && (wallIndex.south(2).asBitboard() & toBB) != 0) {
				shiftedWall >>= 16;
			}

			result[wallIndex][landIndex] = shiftedWall;
		}
	}
	return result;
}

constexpr std::array<std::array<FieldIndex, 64>, 64> WallOperations::populateSlideDir() {
	std::array<std::array<FieldIndex, 64>, 64> result{};
	for (common::Square wallIndex = common::Square::BEGIN; wallIndex != common::Square::INVALID; ++wallIndex) {
		for (common::Square landIndex = common::Square::BEGIN; landIndex != common::Square::INVALID; ++landIndex) {
			FieldIndex d{ 0, 0 };
			const common::Bitset landBB = SLIDE_FROM[landIndex];

			if ((RIGHT & wallIndex.asBitboard()) == 0 && (wallIndex.east(2).asBitboard() & landBB) != 0) {
				d = WEST * 2;
			} else if ((LEFT & wallIndex.asBitboard()) == 0 && (wallIndex.west(2).asBitboard() & landBB) != 0) {
				d = EAST * 2;
			} else if ((TOP & wallIndex.asBitboard()) == 0 && (wallIndex.north(2).asBitboard() & landBB) != 0) {
				d = SOUTH * 2;
			} else if ((BOTTOM & wallIndex.asBitboard()) == 0 && (wallIndex.south(2).asBitboard() & landBB) != 0) {
				d = NORTH * 2;
			}

			result[wallIndex][landIndex] = d;
		}
	}
	return result;
}

constexpr std::array<std::array<common::Square, 64>, 64> WallOperations::populateSlideSquare() {
	std::array<std::array<common::Square, 64>, 64> result{};
	for (common::Square wallIndex = common::Square::BEGIN; wallIndex != common::Square::INVALID; ++wallIndex) {
		for (common::Square landIndex = common::Square::BEGIN; landIndex != common::Square::INVALID; ++landIndex) {
			common::Square destinationSquare = landIndex;
			const common::Bitset wallBB = SLIDE_FROM[wallIndex];

			if ((RIGHT & wallIndex.asBitboard()) == 0 && (wallIndex.east(2).asBitboard() & wallBB) != 0) {
				destinationSquare = destinationSquare.west(2);
			} else if ((LEFT & wallIndex.asBitboard()) == 0 && (wallIndex.west(2).asBitboard() & wallBB) != 0) {
				destinationSquare = destinationSquare.east(2);
			} else if ((TOP & wallIndex.asBitboard()) == 0 && (wallIndex.north(2).asBitboard() & wallBB) != 0) {
				destinationSquare = destinationSquare.south(2);
			} else if ((BOTTOM & wallIndex.asBitboard()) == 0 && (wallIndex.south(2).asBitboard() & wallBB) != 0) {
				destinationSquare = destinationSquare.north(2);
			}

			result[wallIndex][landIndex] = destinationSquare;
		}
	}
	return result;
}

inline constexpr std::array<std::array<common::Bitset, 64>, 64> WallOperations::SLIDE_TO = WallOperations::populateSlideToBB();
inline constexpr std::array<std::array<FieldIndex, 64>, 64> WallOperations::SLIDE_DIR = WallOperations::populateSlideDir();
inline constexpr std::array<std::array<common::Square, 64>, 64> WallOperations::SLIDE_SQUARE = WallOperations::populateSlideSquare();

} //namespace phase4::engine::common

#endif
