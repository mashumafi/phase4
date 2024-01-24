#include <phase4/engine/common/bitset.h>

#include <sstream>

#include <doctest/doctest.h>

TEST_CASE("Bitset getLsb") {
	using namespace phase4::engine::common;

	CHECK(Bitset(0b00101).getLsb() == Bitset(0b001));
	CHECK(Bitset(0b10100).getLsb() == 0b100);
}

TEST_CASE("Bitset popLsb") {
	using namespace phase4::engine::common;

	CHECK(Bitset(0b00101).popLsb() == Bitset(0b00100));
	CHECK(Bitset(0b10100).popLsb() == 0b10000);
}

TEST_CASE("Bitset fastCount") {
	using namespace phase4::engine::common;

	CHECK(Bitset(0b00101).fastCount() == 2);
	CHECK(Bitset(0b10100).fastCount() == 2);
}

TEST_CASE("Bitset bitScan") {
	using namespace phase4::engine::common;

	CHECK(Bitset(0b00101).fastBitScan() == 0);
	CHECK(Bitset((0b10100)).fastBitScan() == 2);
}

TEST_CASE("Bitset output") {
	using namespace phase4::engine::common;

	Bitset bs = 0b1000000000000100000100000001000000000000000000000000000000000101;
	std::stringstream ss;
	ss << bs;
	CHECK(ss.str() == "1000000000000100000100000001000000000000000000000000000000000101");
}
