#ifndef PHASE4_ENGINE_BOARD_TRANSPOSITION_HASH_TABLES_H
#define PHASE4_ENGINE_BOARD_TRANSPOSITION_HASH_TABLES_H

#include <phase4/engine/board/transposition/evaluation_hash_table.h>
#include <phase4/engine/board/transposition/hash_table_constants.h>
#include <phase4/engine/board/transposition/pawn_hash_table.h>
#include <phase4/engine/board/transposition/transposition_table.h>

#include <phase4/engine/common/math.h>

namespace phase4::engine::board::transposition {

template <size_t MB = HashTableConstants::DEFAULT_HASH_TABLES_SIZE>
class HashTables {
public:
	static constexpr size_t PAWN_HASH_TABLE_SIZE = common::Math::max_size(1, MB / HashTableConstants::PAWN_HASH_TABLE_SIZE_DIVIDER);
	static constexpr size_t EVALUATION_HASH_TABLE_SIZE = common::Math::max_size(1, MB / HashTableConstants::EVALUATION_HASH_TABLE_SIZE_DIVIDER);
	static constexpr size_t TRANSPOSITION_TABLE_SIZE = MB - PAWN_HASH_TABLE_SIZE - EVALUATION_HASH_TABLE_SIZE;

	PawnHashTable<PAWN_HASH_TABLE_SIZE> m_pawnHashTable;
	EvaluationHashTable<EVALUATION_HASH_TABLE_SIZE> m_evaluationHashTable;
	TranspositionTable<TRANSPOSITION_TABLE_SIZE> m_transpositionTable;
};

} //namespace phase4::engine::board::transposition

#endif
