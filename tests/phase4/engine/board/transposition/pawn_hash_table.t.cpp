#include <phase4/engine/board/transposition/pawn_hash_table.h>
#include <phase4/engine/board/transposition/pawn_hash_table_entry.h>

#include <phase4/engine/common/bitboard.h>

#include <doctest/doctest.h>

#include <memory>

TEST_CASE("PawnHashTable add/get") {
	using namespace phase4::engine;
	using namespace phase4::engine::board::transposition;

	auto table = std::make_shared<PawnHashTable<1>>();

	const common::Bitboard key(0b10000000'00000001'10000000'00000001'10000000'00000001'10000000'00000001);
	const int16_t openingScore = 123;
	const int16_t endingScore = -456;

	table->add(key, openingScore, endingScore);

	const PawnHashTableEntry &entry = table->get(key);
	CHECK(entry.isKeyValid(key));
	CHECK(entry.openingScore() == openingScore);
	CHECK(entry.endingScore() == endingScore);

	table->clear();

	CHECK(!entry.isKeyValid(key));
	CHECK(entry.openingScore() == 0);
	CHECK(entry.endingScore() == 0);
}
