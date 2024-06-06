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

		if (context.session->position().colorPieceMask(context.session->position().colorToMove(), PieceType::KING) == 0) {
			++context.statistics.qLeafs;
			return board::SearchConstants::NO_KING_VALUE;
		}

		if (context.session->position().isKingChecked(context.session->position().colorToMove().invert())) {
			++context.statistics.qLeafs;
			return -board::SearchConstants::NO_KING_VALUE;
		}

		int32_t standPat = 0;

		const EvaluationHashTableEntry &evaluationEntry = context.session->m_hashTables.m_evaluationHashTable.get(context.session->position().hash().asBitboard());
		if (evaluationEntry.isKeyValid(context.session->position().hash().asBitboard())) {
			standPat = evaluationEntry.score();

#ifndef NDEBUG
			++context.statistics.evaluationStatistics.m_evaluationHashTableHits;
#endif
		} else {
			standPat = score::Evaluation::evaluate(*context.session, context.statistics.evaluationStatistics);
			context.session->m_hashTables.m_evaluationHashTable.add(context.session->position().hash().asBitboard(), static_cast<int16_t>(standPat));

#ifndef NDEBUG
			context.statistics.evaluationStatistics.m_evaluationHashTableNonHits++;

			if (evaluationEntry.key() != 0 || evaluationEntry.score() != 0) {
				++context.statistics.evaluationStatistics.m_evaluationHashTableReplacements;
			} else {
				++context.statistics.evaluationStatistics.m_evaluationHashTableAddedEntries;
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
#ifndef NDEBUG
				++context.statistics.qSeePrunes;
#endif
				break;
			}

			if (standPat + moveValues[moveIndex] + board::SearchConstants::QFUTILITY_PRUNING_MARGIN < alpha) {
#ifndef NDEBUG
				++context.statistics.qFutilityPrunes;
#endif
				break;
			}

			context.session->makeMove(moves[moveIndex]);
			const int32_t score = -findBestMove(context, depth - 1, ply + 1, -beta, -alpha);
			context.session->undoMove(moves[moveIndex]);

			if (score > alpha) {
				alpha = score;

				if (alpha >= beta) {
#ifndef NDEBUG
					if (moveIndex == 0) {
						++context.statistics.qBetaCutoffsAtFirstMove;
					} else {
						++context.statistics.qBetaCutoffsNotAtFirstMove;
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
