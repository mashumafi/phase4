#include <phase4/engine/common/random.h>

#include <doctest/doctest.h>

TEST_CASE("Random seed next") {
	using namespace phase4::engine::common;

	Random rand(42);

	CHECK(rand.next() == 9039304369631583587u);
	CHECK(rand.next() == 8647191391818483560u);
	CHECK(rand.next() == 10334312345110439241u);
}
