#include <phase4/engine/board/transposition/evaluation_hash_table.h>
#include <phase4/engine/board/transposition/evaluation_hash_table_entry.h>

#include <phase4/engine/common/bitboard.h>

#include <doctest/doctest.h>

#include <memory>

TEST_CASE("EvaluationHashTable add/get") {
	using namespace phase4::engine;
	using namespace phase4::engine::board::transposition;

	auto table = std::make_shared<EvaluationHashTable<1>>();

	const common::Bitboard key(0b10000000'00000001'10000000'00000001'10000000'00000001'10000000'00000001);
	const int16_t score = 1234;

	table->add(key, score);

	const EvaluationHashTableEntry &entry = table->get(key);
	CHECK(entry.isKeyValid(key));
	CHECK(entry.score() == score);

	table->clear();

	CHECK(!entry.isKeyValid(key));
	CHECK(entry.score() == 0);
}
