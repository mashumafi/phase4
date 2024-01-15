#include <phase4/engine/common/math.h>

#include <doctest/doctest.h>

TEST_CASE("Math abs") {
    using namespace phase4::engine::common;

    CHECK(Math::abs_int16(0) == 0);
    CHECK(Math::abs_int16(-3) == 3);
    CHECK(Math::abs_int16(6) == 6);
}

TEST_CASE("Math max") {
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
