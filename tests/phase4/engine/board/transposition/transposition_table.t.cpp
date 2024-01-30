#include <phase4/engine/board/transposition/transposition_table.h>
#include <phase4/engine/board/transposition/transposition_table_entry.h>

#include <phase4/engine/common/bitset.h>

#include <doctest/doctest.h>

TEST_CASE("TranspositionTable references") {
	using namespace phase4::engine;
	using namespace phase4::engine::board::transposition;

	TranspositionTable<1> table;

	TranspositionTableEntry &entry = table.get(common::Bitset(123));
	entry.score() = 42;

	CHECK(entry.score() == 42);
	CHECK(table.get(common::Bitset(123)).score() == 42);

	table.clear();

	CHECK(entry.score() == 0);
	CHECK(table.get(common::Bitset(123)).score() == 0);
}