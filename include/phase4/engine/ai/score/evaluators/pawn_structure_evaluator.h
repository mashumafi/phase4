#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATORS_PAWN_STRUCTURE_EVALUATOR_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATORS_PAWN_STRUCTURE_EVALUATOR_H

#include <phase4/engine/ai/score/evaluation_statistics.h>
#include <phase4/engine/ai/score/tapered_evaluation.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/session.h>
#include <phase4/engine/board/transposition/pawn_hash_table.h>
#include <phase4/engine/board/transposition/pawn_hash_table_entry.h>

#include <phase4/engine/moves/patterns/chain_pattern_generator.h>
#include <phase4/engine/moves/patterns/file_pattern_generator.h>
#include <phase4/engine/moves/patterns/outer_files_pattern_generator.h>

#include <phase4/engine/common/game_phase.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>

#include <cstdint>
#include <tuple>

namespace phase4::engine::ai::score::evaluators {

class PawnStructureEvaluator {
public:
	static inline int32_t evaluate(board::Session &session, EvaluationStatistics &statistics, int32_t openingPhase, int32_t endingPhase) {
		(void)statistics;

		const board::transposition::PawnHashTableEntry& entry = session.m_hashTables.m_pawnHashTable.get(session.position().m_pawnHash.asBitboard());
		if (entry.isKeyValid(session.position().m_pawnHash.asBitboard())) {
#ifndef NDEBUG
			++statistics.m_pawnHashTableHits;
#endif
			return TaperedEvaluation::adjustToPhase(entry.openingScore(), entry.endingScore(), openingPhase, endingPhase);
		}
#ifndef NDEBUG
		else {
			++statistics.m_pawnHashTableNonHits;

			if (entry.key() != 0 || entry.openingScore() != 0 || entry.endingScore() != 0) {
				++statistics.m_pawnHashTableReplacements;
			} else {
				++statistics.m_pawnHashTableAddedEntries;
			}
		}
#endif

		const auto [openingWhiteScore, endingWhiteScore] = evaluate(session.position(), common::PieceColor::WHITE);
		const auto [openingBlackScore, endingBlackScore] = evaluate(session.position(), common::PieceColor::BLACK);

		const int32_t openingScore = openingWhiteScore - openingBlackScore;
		const int32_t endingScore = endingWhiteScore - endingBlackScore;
		const int32_t result = TaperedEvaluation::adjustToPhase(openingScore, endingScore, openingPhase, endingPhase);

		session.m_hashTables.m_pawnHashTable.add(session.position().m_pawnHash.asBitboard(), static_cast<int16_t>(openingScore), static_cast<int16_t>(endingScore));

		return result;
	}

	static inline int32_t evaluateWithoutCache(const board::Position &position, int32_t openingPhase, int32_t endingPhase) {
		const auto [openingWhiteScore, endingWhiteScore] = evaluate(position, common::PieceColor::WHITE);
		const auto [openingBlackScore, endingBlackScore] = evaluate(position, common::PieceColor::BLACK);

		const int32_t openingScore = openingWhiteScore - openingBlackScore;
		const int32_t endingScore = endingWhiteScore - endingBlackScore;

		return TaperedEvaluation::adjustToPhase(openingScore, endingScore, openingPhase, endingPhase);
	}

private:
	static inline std::tuple<int32_t, int32_t> evaluate(const board::Position &position, common::PieceColor color) {
		int32_t doubledPawns = 0;
		int32_t isolatedPawns = 0;
		int32_t chainedPawns = 0;
		int32_t passingPawns = 0;

		for (uint8_t file = 0; file < 8; ++file) {
			const common::Bitset friendlyPawnsOnInnerMask = position.colorPieceMask(color, common::PieceType::PAWN) & moves::patterns::FilePatternGenerator::getPatternForFile(file);
			const common::Bitset friendlyPawnsOnOuterMask = position.colorPieceMask(color, common::PieceType::PAWN) & moves::patterns::OuterFilesPatternGenerator::getPatternForFile(file);

			common::Bitset pawnsCount(friendlyPawnsOnInnerMask.fastCount());
			if (pawnsCount > 1) {
				doubledPawns += (pawnsCount.get_raw_value() - 1);
			}

			if (friendlyPawnsOnInnerMask != 0) {
				if (friendlyPawnsOnOuterMask == 0) {
					isolatedPawns += pawnsCount.fastCount(); // TODO: understand this
				}
			}
		}

		common::Bitset pieces = position.colorPieceMask(color, common::PieceType::PAWN);
		while (pieces != 0) {
			const common::Bitset lsb = pieces.getLsb(); // TODO: skip lsb
			const common::Square field(lsb.bitScan());
			pieces = pieces.popLsb();

			const common::Bitset chain = moves::patterns::ChainPatternGenerator::getPattern(field) & position.colorPieceMask(color, common::PieceType::PAWN);
			if (chain != 0) {
				chainedPawns += chain.fastCount();
			}

			if (position.isFieldPassing(color, field)) {
				passingPawns++;
			}
		}

		const int32_t doubledPawnsOpeningScore = doubledPawns * board::EvaluationConstants::DOUBLED_PAWNS[common::GamePhase::OPENING];
		const int32_t doubledPawnsEndingScore = doubledPawns * board::EvaluationConstants::DOUBLED_PAWNS[common::GamePhase::ENDING];

		const int32_t isolatedPawnsOpeningScore = isolatedPawns * board::EvaluationConstants::ISOLATED_PAWNS[common::GamePhase::OPENING];
		const int32_t isolatedPawnsEndingScore = isolatedPawns * board::EvaluationConstants::ISOLATED_PAWNS[common::GamePhase::ENDING];

		const int32_t chainedPawnsOpeningScore = chainedPawns * board::EvaluationConstants::CHAINED_PAWNS[common::GamePhase::OPENING];
		const int32_t chainedPawnsEndingScore = chainedPawns * board::EvaluationConstants::CHAINED_PAWNS[common::GamePhase::ENDING];

		const int32_t passingPawnsOpeningScore = passingPawns * board::EvaluationConstants::PASSING_PAWNS[common::GamePhase::OPENING];
		const int32_t passingPawnsEndingScore = passingPawns * board::EvaluationConstants::PASSING_PAWNS[common::GamePhase::ENDING];

		const int32_t openingScore = doubledPawnsOpeningScore + isolatedPawnsOpeningScore + chainedPawnsOpeningScore + passingPawnsOpeningScore;
		const int32_t endingScore = doubledPawnsEndingScore + isolatedPawnsEndingScore + chainedPawnsEndingScore + passingPawnsEndingScore;

		return std::make_tuple(openingScore, endingScore);
	}
};

} //namespace phase4::engine::ai::score::evaluators

#endif
