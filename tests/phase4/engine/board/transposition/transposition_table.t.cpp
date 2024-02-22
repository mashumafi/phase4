#include <phase4/engine/board/transposition/transposition_table.h>
#include <phase4/engine/board/transposition/transposition_table_entry.h>

#include <phase4/engine/common/bitboard.h>

#include <doctest/doctest.h>

#include <memory>

TEST_CASE("TranspositionTable add/get") {
	using namespace phase4::engine;
	using namespace phase4::engine::board::transposition;

	auto table = std::make_shared<TranspositionTable<1>>();

	const common::Bitboard key(0b10000000'00000001'10000000'00000001'10000000'00000001'10000000'00000001);
	const int32_t score = 42;
	const moves::Move bestMove = moves::Move("a2b2");

	table->add(key, TranspositionTableEntry(key, score, bestMove, 4, TranspositionTableEntryFlags::ALPHA_SCORE, 3));

	const TranspositionTableEntry &entry = table->get(key);
	CHECK(entry.isKeyValid(key));
	CHECK(entry.score() == score);
	CHECK(entry.bestMove() == bestMove);
	CHECK(entry.depth() == 4);
	CHECK(entry.flags() == TranspositionTableEntryFlags::ALPHA_SCORE);
	CHECK(entry.age() == 3);

	table->clear();

	CHECK(!entry.isKeyValid(key));
	CHECK(entry.score() == 0);
	CHECK(entry.bestMove() == moves::Move("h1h1"));
	CHECK(entry.depth() == 0);
	CHECK(entry.flags() == TranspositionTableEntryFlags::INVALID);
	CHECK(entry.age() == 0);
}
