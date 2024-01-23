#ifndef PHASE4_ENGINE_COMMON_WALL_OPERATIONS_H
#define PHASE4_ENGINE_COMMON_WALL_OPERATIONS_H

#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/square.h>

#include <functional>

namespace phase4::engine::common {

inline constexpr FieldIndex North = { 0, 1 };
inline constexpr FieldIndex East = { -1, 0 };
inline constexpr FieldIndex South = -North;
inline constexpr FieldIndex West = -East;
inline constexpr FieldIndex NorthWest = North + West;

class WallOperations {
public:
	static constexpr int FileMax = 8;
	static constexpr int RankMax = 8;
	static constexpr int Rank2 = 1;
	static constexpr int FileB = 1;

	static constexpr int16_t Round(int16_t n, int16_t d) {
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

	static constexpr std::array<std::array<uint64_t, 8>, 8> populateSlideFromBB();

	static constexpr std::array<std::array<common::Bitset, 64>, 64> populateSlideToBB();

	static constexpr std::array<std::array<FieldIndex, 64>, 64> populateSlideDir();

	static constexpr std::array<std::array<common::Square, 64>, 64> populateSlideSquare();

	// utility, given an x,y gives you a valid wall mask for that coordinate
	static const std::array<std::array<uint64_t, 8>, 8> SLIDE_FROM;

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

constexpr std::array<std::array<uint64_t, 8>, 8> WallOperations::populateSlideFromBB() {
	std::array<std::array<uint64_t, 8>, 8> result{};
	for (common::Square from = common::Square::BEGIN; from != common::Square::INVALID; ++from) {
		FieldIndex bottomRight{ Round(GetFile(from), 2), Round(GetRank(from), 2) };
		uint64_t wallSquares = SquareBB(bottomRight) | SquareBB(bottomRight + North) | SquareBB(bottomRight + West) | SquareBB(bottomRight + NorthWest);
		result[from % 8][from / 8] = wallSquares;
	}
	return result;
}

inline constexpr std::array<std::array<uint64_t, 8>, 8> WallOperations::SLIDE_FROM = WallOperations::populateSlideFromBB();

constexpr std::array<std::array<common::Bitset, 64>, 64> WallOperations::populateSlideToBB() {
	std::array<std::array<common::Bitset, 64>, 64> result{};
	for (common::Square from = common::Square::BEGIN; from != common::Square::INVALID; ++from) {
		FieldIndex bottomRight{ Round(GetFile(from), 2), Round(GetRank(from), 2) };
		int64_t wallSquares = SLIDE_FROM[from % 8][from / 8];

		for (common::Square to = common::Square::BEGIN; to != common::Square::INVALID; ++to) {
			FieldIndex d{ 0, 0 };
			int64_t shiftedWall = wallSquares;
			int64_t toBB = SquareBB(to);

			if (GetFile(from) > FileB && ((wallSquares >> (-East * 2).x) & toBB) > 0) {
				d = East * 2;
				shiftedWall = wallSquares >> -d.x;
			} else if (GetFile(from) < FileMax - 2 && ((wallSquares << (West * 2).x) & toBB) > 0) {
				d = West * 2;
				shiftedWall = wallSquares << d.x;
			} else if (GetRank(from) < RankMax - 2 && ((wallSquares << (North * 2).y) & toBB) > 0) {
				d = North * 2;
				shiftedWall = wallSquares << d.y;
			} else if (GetRank(from) > Rank2 && ((wallSquares >> (-South * 2).y) & toBB) > 0) {
				d = South * 2;
				shiftedWall = wallSquares >> -d.y;
			}

			result[from][to] = shiftedWall;
		}
	}
	return result;
}

constexpr std::array<std::array<FieldIndex, 64>, 64> WallOperations::populateSlideDir() {
	std::array<std::array<FieldIndex, 64>, 64> result{};
	for (common::Square from = common::Square::BEGIN; from != common::Square::INVALID; ++from) {
		FieldIndex bottomRight{ Round(GetFile(from), 2), Round(GetRank(from), 2) };
		uint64_t wallSquares = SLIDE_FROM[from % 8][from / 8];

		for (common::Square to = common::Square::BEGIN; to != common::Square::INVALID; ++to) {
			FieldIndex d{ 0, 0 };
			uint64_t toBB = SquareBB(to);

			if (GetFile(from) > FileB && ((wallSquares >> (-East * 2).x) & toBB) > 0) {
				d = East * 2;
			} else if (GetFile(from) < FileMax - 2 && ((wallSquares << (West * 2).x) & toBB) > 0) {
				d = West * 2;
			} else if (GetRank(from) < RankMax - 2 && ((wallSquares << (North * 2).y) & toBB) > 0) {
				d = North * 2;
			} else if (GetRank(from) > Rank2 && ((wallSquares >> (-South * 2).y) & toBB) > 0) {
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
		FieldIndex bottomRight{ Round(GetFile(from), 2), Round(GetRank(from), 2) };
		uint64_t wallSquares = SLIDE_FROM[from % 8][from / 8];

		for (common::Square to = common::Square::BEGIN; to != common::Square::INVALID; ++to) {
			FieldIndex d{ 0, 0 };
			uint64_t toBB = SquareBB(to);

			if (GetFile(from) > FileB && ((wallSquares >> (-East * 2).x) & toBB) > 0) {
				d = East * 2;
			} else if (GetFile(from) < FileMax - 2 && ((wallSquares << (West * 2).x) & toBB) > 0) {
				d = West * 2;
			} else if (GetRank(from) < RankMax - 2 && ((wallSquares << (North * 2).y) & toBB) > 0) {
				d = North * 2;
			} else if (GetRank(from) > Rank2 && ((wallSquares >> (-South * 2).y) & toBB) > 0) {
				d = South * 2;
			}

			result[from][to] = common::Square(to - d.y);
		}
	}
	return result;
}

inline constexpr std::array<std::array<common::Bitset, 64>, 64> WallOperations::SLIDE_TO = WallOperations::populateSlideToBB();
inline constexpr std::array<std::array<FieldIndex, 64>, 64> WallOperations::SLIDE_DIR = WallOperations::populateSlideDir();
inline constexpr std::array<std::array<common::Square, 64>, 64> WallOperations::SLIDE_SQUARE = WallOperations::populateSlideSquare();

} //namespace phase4::engine::common

#endif
