#ifndef PHASE4_ENGINE_COMMON_FIELD_INDEX_H
#define PHASE4_ENGINE_COMMON_FIELD_INDEX_H

#include <phase4/engine/common/math.h>

#include <cstdint>
#include <iostream>
#include <string_view>

namespace phase4::engine::common {

struct FieldIndex {
	int16_t x;
	int16_t y;

	static const FieldIndex ZERO;

	constexpr FieldIndex() noexcept;

	constexpr FieldIndex(int16_t x, int16_t y) noexcept;

	constexpr FieldIndex(std::string_view move) noexcept;

	constexpr uint16_t maxCartesianDistance(FieldIndex other) const;

	constexpr bool isValid() const;

	constexpr int16_t offset() const;

	constexpr FieldIndex operator+=(FieldIndex that);
};

constexpr FieldIndex::FieldIndex(int16_t x, int16_t y) noexcept :
		x{ x },
		y{ y } {
}

inline constexpr FieldIndex FieldIndex::ZERO(0, 0);

constexpr FieldIndex::FieldIndex() noexcept :
		FieldIndex(0, 0) {
}

constexpr FieldIndex::FieldIndex(std::string_view move) noexcept :
		x{ static_cast<int16_t>(move[0] - 'a') },
		y{ static_cast<int16_t>(move[1] - '1') } {
}

/**
 * @brief Calculate the max cartesian distance between two points.
 *
 * The cartesian distance is the max of the absolute differences of their coordinates.
 * It represents either the max distance along either the x-axis or y-axis.
 *
 * @param other The second point.
 * @return The max cartesian distance between the two points.
 */
constexpr uint16_t FieldIndex::maxCartesianDistance(FieldIndex other) const {
	return Math::max_int16(Math::abs_int16(other.x - this->x), Math::abs_int16(other.y - this->y));
}

constexpr bool FieldIndex::isValid() const {
	return x >= 0 && x <= 7 && y >= 0 && y <= 7;
}

constexpr int16_t FieldIndex::offset() const {
	return x + y * 8;
}

constexpr bool operator==(const FieldIndex left, const FieldIndex right) {
	return left.x == right.x && left.y == right.y;
}

constexpr bool operator!=(const FieldIndex left, const FieldIndex right) {
	return left.x != right.x || left.y != right.y;
}

constexpr FieldIndex operator+(FieldIndex left, FieldIndex right) {
	return FieldIndex(left.x + right.x, left.y + right.y);
}

constexpr FieldIndex FieldIndex::operator+=(FieldIndex that) {
	x += that.x;
	y += that.y;
	return *this;
}

constexpr FieldIndex operator-(FieldIndex that) {
	return FieldIndex(-that.x, -that.y);
}

constexpr FieldIndex operator*(FieldIndex that, int16_t scalar) {
	return FieldIndex(that.x * scalar, that.y * scalar);
}

inline std::ostream &operator<<(std::ostream &os, const FieldIndex fieldIndex) {
	return os << "x:" << fieldIndex.x << ", y:" << fieldIndex.y;
}

} //namespace phase4::engine::common

#endif
