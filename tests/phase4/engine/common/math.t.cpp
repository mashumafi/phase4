#include <phase4/engine/common/math.h>

#include <phase4/engine/common/util.h>

#include <doctest/doctest.h>

TEST_CASE("Math i16 abs") {
	using namespace phase4::engine::common;

	CHECK(Math::abs_int16(0) == 0);
	CHECK(Math::abs_int16(-3) == 3);
	CHECK(Math::abs_int16(6) == 6);
}

TEST_CASE("Math i16 min") {
	using namespace phase4::engine::common;

	CHECK(Math::min_int16(0, 0) == 0);
	CHECK(Math::min_int16(1, -3) == -3);
	CHECK(Math::min_int16(-2, 4) == -2);
	CHECK(Math::min_int16(-3, -9) == -9);
	CHECK(Math::min_int16(3, 4) == 3);
	CHECK(Math::min_int16(0, 4) == 0);
	CHECK(Math::min_int16(5, 0) == 0);
	CHECK(Math::min_int16(0, -8) == -8);
	CHECK(Math::min_int16(-7, 0) == -7);
}

TEST_CASE("Math i16 max") {
	using namespace phase4::engine::common;

	CHECK(Math::max_int16(0, 0) == 0);
	CHECK(Math::max_int16(1, -3) == 1);
	CHECK(Math::max_int16(-2, 4) == 4);
	CHECK(Math::max_int16(-3, -9) == -3);
	CHECK(Math::max_int16(3, 4) == 4);
	CHECK(Math::max_int16(0, 4) == 4);
	CHECK(Math::max_int16(5, 0) == 5);
	CHECK(Math::max_int16(0, -8) == 0);
	CHECK(Math::max_int16(-7, 0) == 0);
}

TEST_CASE("Math i32 abs") {
	using namespace phase4::engine::common;

	CHECK(Math::abs_int32(0) == 0);
	CHECK(Math::abs_int32(-3) == 3);
	CHECK(Math::abs_int32(6) == 6);
}

TEST_CASE("Math i32 min") {
	using namespace phase4::engine::common;

	CHECK(Math::min_int32(0, 0) == 0);
	CHECK(Math::min_int32(1, -3) == -3);
	CHECK(Math::min_int32(-2, 4) == -2);
	CHECK(Math::min_int32(-3, -9) == -9);
	CHECK(Math::min_int32(3, 4) == 3);
	CHECK(Math::min_int32(0, 4) == 0);
	CHECK(Math::min_int32(5, 0) == 0);
	CHECK(Math::min_int32(0, -8) == -8);
	CHECK(Math::min_int32(-7, 0) == -7);
}

TEST_CASE("Math i32 max") {
	using namespace phase4::engine::common;

	CHECK(Math::max_int32(0, 0) == 0);
	CHECK(Math::max_int32(1, -3) == 1);
	CHECK(Math::max_int32(-2, 4) == 4);
	CHECK(Math::max_int32(-3, -9) == -3);
	CHECK(Math::max_int32(3, 4) == 4);
	CHECK(Math::max_int32(0, 4) == 4);
	CHECK(Math::max_int32(5, 0) == 5);
	CHECK(Math::max_int32(0, -8) == 0);
	CHECK(Math::max_int32(-7, 0) == 0);
}

TEST_CASE("Math size min") {
	using namespace phase4::engine::common;
	using namespace util;

	CHECK(Math::min_size(0, 0) == 0);
	CHECK(Math::min_size(1, back_index(3)) == 1);
	CHECK(Math::min_size(back_index(2), 4) == 4);
	CHECK(Math::min_size(back_index(3), back_index(9)) == back_index(9));
	CHECK(Math::min_size(3, 4) == 3);
	CHECK(Math::min_size(0, 4) == 0);
	CHECK(Math::min_size(5, 0) == 0);
	CHECK(Math::min_size(0, back_index(8)) == 0);
	CHECK(Math::min_size(back_index(7), 0) == 0);
}

TEST_CASE("Math size max") {
	using namespace phase4::engine::common;
	using namespace util;

	CHECK(Math::max_size(0, 0) == 0);
	CHECK(Math::max_size(1, back_index(3)) == back_index(3));
	CHECK(Math::max_size(back_index(2), 4) == back_index(2));
	CHECK(Math::max_size(back_index(3), back_index(9)) == back_index(3));
	CHECK(Math::max_size(3, 4) == 4);
	CHECK(Math::max_size(0, 4) == 4);
	CHECK(Math::max_size(5, 0) == 5);
	CHECK(Math::max_size(0, back_index(8)) == back_index(8));
	CHECK(Math::max_size(back_index(7), 0) == back_index(7));
}
