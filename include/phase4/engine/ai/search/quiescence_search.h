#ifndef PHASE4_ENGINE_AI_SEARCH_QUIESCENCE_SEARCH_H
#define PHASE4_ENGINE_AI_SEARCH_QUIESCENCE_SEARCH_H

#include <phase4/engine/ai/score/evaluation.h>
#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/board/operators.h>
#include <phase4/engine/board/ordering/move_ordering.h>
#include <phase4/engine/board/search_constants.h>
#include <phase4/engine/board/transposition/evaluation_hash_table.h>

#include <phase4/engine/moves/move.h>

#include <phase4/engine/common/piece_type.h>

#include <cstdint>

namespace phase4::engine::ai::search {

class QuiescenceSearch {
public:
	static int32_t findBestMove(SearchContext &context, int32_t depth, int32_t ply, int32_t alpha, int32_t beta) {
		using namespace common;
		using namespace board::transposition;

		++context.statistics.qNodes;

		if (ply > context.statistics.selectiveDepth) {
			context.statistics.selectiveDepth = ply;
		}

		if (context.session->position().colorPieceMask(context.session->position().m_colorToMove, PieceType::KING) == 0) {
			++context.statistics.qLeafs;
			return board::SearchConstants::NO_KING_VALUE;
		}

		if (context.session->position().isKingChecked(context.session->position().m_colorToMove.invert())) {
			++context.statistics.qLeafs;
			return -board::SearchConstants::NO_KING_VALUE;
		}

		int32_t standPat = 0;

		const EvaluationHashTableEntry &evaluationEntry = context.session->m_hashTables.m_evaluationHashTable.get(context.session->position().m_hash.asBitboard());
		if (evaluationEntry.isKeyValid(context.session->position().m_hash.asBitboard())) {
			standPat = evaluationEntry.score();

#if DEBUG
			context.Statistics.EvaluationStatistics.EHTHits++;
#endif
		} else {
			standPat = score::Evaluation::evaluate(*context.session, true, context.statistics.evaluationStatistics);
			context.session->m_hashTables.m_evaluationHashTable.add(context.session->position().m_hash.asBitboard(), static_cast<int16_t>(standPat));

#if DEBUG
			context.Statistics.EvaluationStatistics.EHTNonHits++;
			context.Statistics.EvaluationStatistics.EHTAddedEntries++;

			if (evaluationEntry.Key != 0 || evaluationEntry.Score != 0) {
				context.Statistics.EvaluationStatistics.EHTReplacements++;
			}
#endif
		}

		if (standPat >= beta) {
			++context.statistics.qLeafs;
			return standPat;
		}

		if (standPat > alpha) {
			alpha = standPat;
		}

		moves::Moves moves;
		moves::MoveValues moveValues;

		board::Operators::getAvailableCaptureMoves(context.session->position(), moves);
		board::ordering::MoveOrdering::assignQValues(context.session->position(), moves, moveValues);

		for (size_t moveIndex = 0; moveIndex < moves.size(); ++moveIndex) {
			board::ordering::MoveOrdering::sortNextBestMove(moves, moveValues, moveIndex);

			if (moveValues[moveIndex] < 0) {
#if DEBUG
				context.Statistics.QSEEPrunes++;
#endif
				break;
			}

			if (standPat + moveValues[moveIndex] + board::SearchConstants::QFUTILITY_PRUNING_MARGIN < alpha) {
#if DEBUG
				context.Statistics.QFutilityPrunes++;
#endif
				break;
			}

			context.session->makeMove(moves[moveIndex]);
			const int32_t score = -findBestMove(context, depth - 1, ply + 1, -beta, -alpha);
			context.session->undoMove(moves[moveIndex]);

			if (score > alpha) {
				alpha = score;

				if (alpha >= beta) {
#if DEBUG
					if (moveIndex == 0) {
						context.Statistics.QBetaCutoffsAtFirstMove++;
					} else {
						context.Statistics.QBetaCutoffsNotAtFirstMove++;
					}
#endif

					++context.statistics.qBetaCutoffs;
					break;
				}
			}
		}

		return alpha;
	}
};

} //namespace phase4::engine::ai::search

#endif
