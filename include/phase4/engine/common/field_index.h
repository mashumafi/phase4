#ifndef PHASE4_ENGINE_COMMON_FIELD_INDEX_H
#define PHASE4_ENGINE_COMMON_FIELD_INDEX_H

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string_view>

namespace phase4::engine::common {

struct FieldIndex {
	int16_t x;
	int16_t y;

	constexpr FieldIndex(int16_t x, int16_t y) :
			x{ x },
			y{ y } {
	}

	constexpr FieldIndex(std::string_view move) :
			x{ static_cast<int16_t>('8' - move[1]) },
			y{ static_cast<int16_t>(move[0] - 'a') } {
	}

	constexpr uint16_t manhattanDistance(const FieldIndex &other) const;
};

/**
 * @brief Calculate the Manhattan distance between two points.
 *
 * The Manhattan distance, also known as the taxicab distance, is the sum of the absolute
 * differences of their coordinates. It represents the distance in a grid-like structure
 * where only horizontal and vertical movements are allowed.
 *
 * @param other The second point.
 * @return The Manhattan distance between the two points.
 */
constexpr uint16_t FieldIndex::manhattanDistance(const FieldIndex &other) const {
	return std::max(std::abs(other.x - this->x), std::abs(other.y - this->y));
}

inline bool operator==(const FieldIndex &left, const FieldIndex &right) {
	return left.x == right.x && left.y == right.y;
}

inline std::ostream &operator<<(std::ostream &os, const FieldIndex &fieldIndex) {
	return os << "x:" << fieldIndex.x << ", y:" << fieldIndex.y;
}

} //namespace phase4::engine::common

#endif
