#ifndef PHASE4_COMMON_POINT_H
#define PHASE4_COMMON_POINT_H

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace phase4::common {

struct Point {
	int16_t x;
	int16_t y;

	constexpr uint16_t maxCartesianDistance(const Point &other) const;
};

constexpr uint16_t Point::maxCartesianDistance(const Point &other) const {
	return std::max(std::abs(other.x - this->x), std::abs(other.y - this->y));
}

} //namespace phase4::common

#endif
