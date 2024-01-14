#include <phase4/engine/common/random.h>

#include <doctest/doctest.h>

TEST_CASE("Random seed next") {
	using namespace phase4::engine::common;

	Random rand(42);

	CHECK(rand.next() == 13930160852258120406);
}
