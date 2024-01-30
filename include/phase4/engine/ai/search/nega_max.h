#ifndef PHASE4_ENGINE_AI_SEARCH_NEGA_MAX_H
#define PHASE4_ENGINE_AI_SEARCH_NEGA_MAX_H

#include <phase4/engine/ai/search/quiescence_search.h>
#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/board/ordering/move_ordering.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/common/piece_color.h>

#include <cstdint>

namespace phase4::engine::ai::search {

class NegaMax {
public:
	static int findBestMove(SearchContext &context, int depth, int ply, int alpha, int beta) {
		const bool friendlyKingInCheck = context.session->isKingChecked(context.session->m_position.m_colorToMove);
		return findBestMove(context, depth, ply, alpha, beta, true, friendlyKingInCheck, 0);
	}

	static int findBestMove(SearchContext &context, int depth, int ply, int alpha, int beta, bool allowNullMove, bool friendlyKingInCheck, int extensionsCount) {
		using namespace board::transposition;

		(void)allowNullMove;
		(void)friendlyKingInCheck;
		(void)extensionsCount;

		if (context.statistics.nodes >= context.maxNodesCount) {
			context.abortSearch = true;
			return 0;
		}

		if (context.abortSearch) {
			return 0;
		}

		context.statistics.nodes++;

		if (context.session->m_position.m_colorPieceMasks[context.session->m_position.m_colorToMove.get_raw_value()][common::PieceType::KING.get_raw_value()] == 0) {
			context.statistics.leafs++;
			return board::SearchConstants::NO_KING_VALUE;
		}

		if (context.session->m_position.isKingChecked(context.session->m_position.m_colorToMove.invert())) {
			context.statistics.leafs++;
			return -board::SearchConstants::NO_KING_VALUE;
		}

		if (context.session->isThreefoldRepetition()) {
			context.statistics.leafs++;
			return board::EvaluationConstants::THREEFOLD_REPETITION;
		}

		if (context.session->m_position.isInsufficientMaterial()) {
			const common::PieceColor enemyColor = context.session->m_position.m_colorToMove.invert();
			if (!friendlyKingInCheck && !context.session->m_position.isKingChecked(enemyColor)) {
				context.statistics.leafs++;
				return board::EvaluationConstants::INSUFFICIENT_MATERIAL;
			}
		}

		if (context.session->m_position.isFiftyMoveRuleDraw()) {
			context.statistics.leafs++;
			return board::EvaluationConstants::THREEFOLD_REPETITION;
		}

		if (depth <= 0) {
			context.statistics.leafs++;
			return QuiescenceSearch::findBestMove(context, depth, ply, alpha, beta);
		}

		//int originalAlpha = alpha;
		int pvNode = beta - alpha > 1;

		TranspositionTableEntry &entry = context.session->m_hashTables.m_transpositionTable.get(context.session->m_position.m_hash.asBitboard());
		moves::Move hashMove = moves::Move::Empty;
		moves::Move bestMove = moves::Move::Empty;

		if (entry.flags() != TranspositionTableEntryFlags::INVALID && entry.isKeyValid(context.session->m_position.m_hash.asBitboard())) {
#ifndef NDEBUG
			++context.statistics.transpositionTableHits;
#endif
			if (entry.flags() != TranspositionTableEntryFlags::ALPHA_SCORE) {
				const bool isMoveLegal = context.session->isMoveLegal(entry.bestMove());
				if (isMoveLegal) {
					hashMove = entry.bestMove();
					bestMove = entry.bestMove();
#ifndef NDEBUG
					context.statistics.transpositionTableValidMoves++;
#endif
				}
#ifndef NDEBUG
				else {
					context.statistics.transpositionTableInvalidMoves++;
				}
#endif
			}

			if (entry.depth() >= depth) {
				entry.score() = static_cast<int16_t>(TranspositionTable<0>::transpositionTableToRegularScore(entry.score(), ply));
				switch (entry.flags()) {
					case TranspositionTableEntryFlags::ALPHA_SCORE: {
						if (entry.score() < beta) {
							beta = entry.score();
						}

						break;
					}

					case TranspositionTableEntryFlags::EXACT_SCORE: {
						if (!pvNode || board::SearchConstants::isScoreNearCheckmate(entry.score())) {
							return entry.score();
						}

						break;
					}

					case TranspositionTableEntryFlags::BETA_SCORE: {
						if (entry.score() > alpha) {
							alpha = entry.score();
						}

						break;
					}

					case TranspositionTableEntryFlags::INVALID: {
						break;
					}
				}

				if (alpha >= beta) {
					++context.statistics.betaCutoffs;
					return entry.score();
				}
			}
		}
#ifndef NDEBUG
		else {
			entry = TranspositionTableEntry();
			context.statistics.transpositionTableNonHits++;
		}
#endif

		/*if (RazoringCanBeApplied(depth, context.statistics.Depth, friendlyKingInCheck, pvNode, alpha)) {
			var fastEvaluation = Evaluation.FastEvaluate(context.session->m_position, context.statistics.EvaluationStatistics);
			var margin = SearchConstants.RazoringMargin + (depth - SearchConstants.RazoringMinDepth) * SearchConstants.RazoringMarginMultiplier;
			var futileAlpha = alpha - margin;

			if (fastEvaluation < futileAlpha) {
				var result = QuiescenceSearch.FindBestMove(context, depth, ply, futileAlpha, futileAlpha + 1);
				if (result <= futileAlpha) {
#ifndef NDEBUG
					context.statistics.Razorings++;
#endif
					return futileAlpha;
				}
#ifndef NDEBUG
				else {
					context.statistics.RazoringsRejected++;
				}
#endif
			}
		}

		if (StaticNullMoveCanBeApplied(depth, context.statistics.Depth, friendlyKingInCheck, pvNode, beta)) {
			var fastEvaluation = Evaluation.FastEvaluate(context.session->m_position, context.statistics.EvaluationStatistics);
			var margin = SearchConstants.StaticNullMoveMargin + (depth - 1) * SearchConstants.StaticNullMoveMarginMultiplier;
			var score = fastEvaluation - margin;

			if (score >= beta) {
#ifndef NDEBUG
				context.statistics.StaticNullMovePrunes++;
#endif

				return score;
			}
		}

		if (NullMoveCanBeApplied(context.session->m_position, depth, allowNullMove, pvNode, friendlyKingInCheck)) {
			context.session->m_position.MakeNullMove();
			var score = -FindBestMove(context, depth - 1 - NullMoveGetReduction(depth), ply + 1, -beta, -beta + 1, false, false, extensionsCount);
			context.session->m_position.UndoNullMove();

			if (score >= beta) {
#ifndef NDEBUG
				context.statistics.NullMovePrunes++;
#endif

				return score;
			}
		}

		if (IIDCanBeApplied(depth, entry.Flags, hashMove)) {
			FindBestMove(context, depth - 1 - SearchConstants.IIDDepthReduction, ply, alpha, beta, allowNullMove, friendlyKingInCheck, extensionsCount);

			var iidEntry = TranspositionTable.Get(context.session->m_position.Hash);
			if (iidEntry.IsKeyValid(context.session->m_position.Hash)) {
				hashMove = iidEntry.BestMove;

#ifndef NDEBUG
				context.statistics.IIDHits++;
#endif
			}
		}

		var futilityPruningCanBeApplied = false;
		var futilityPruningEvaluation = 0;
		var futilityPruningMargin = 0;

		if (FutilityPruningCanBeApplied(depth, context.statistics.Depth, friendlyKingInCheck, pvNode, alpha)) {
			futilityPruningCanBeApplied = true;
			futilityPruningEvaluation = Evaluation.FastEvaluate(context.session->m_position, context.statistics.EvaluationStatistics);
			futilityPruningMargin = SearchConstants.FutilityPruningMargin + (depth - 1) * SearchConstants.FutilityPruningMarginMultiplier;
		}

		Span<Move> moves = stackalloc Move[SearchConstants.MaxMovesCount];
		Span<short> moveValues = stackalloc short[SearchConstants.MaxMovesCount];
		var movesCount = 0;

		var loudMovesGenerated = false;
		var quietMovesGenerated = false;

		var evasionMask = ulong.MaxValue;
		if (friendlyKingInCheck && !context.session->m_position.IsKingChecked(ColorOperations.Invert(context.session->m_position.m_colorToMove))) {
			var kingField = context.session->m_position.m_colorPieceMasks[context.session->m_position.m_colorToMove][common::PieceType::KING];
			var kingFieldIndex = BitOperations.BitScan(kingField);

			evasionMask = KnightMovesGenerator.GetMoves(kingFieldIndex) |
					QueenMovesGenerator.GetMoves(context.session->m_position.OccupancySummary, kingFieldIndex);
		}

		if (hashMove == Move.Empty) {
			movesCount = context.session->m_position.GetLoudMoves(moves, 0, evasionMask);
			MoveOrdering.AssignLoudValues(context.session->m_position, moves, moveValues, movesCount, depth, Move.Empty);
			loudMovesGenerated = true;

#ifndef NDEBUG
			context.statistics.LoudMovesGenerated++;
#endif

			if (movesCount == 0) {
				movesCount = context.session->m_position.GetQuietMoves(moves, 0, evasionMask);
				MoveOrdering.AssignQuietValues(context.session->m_position, moves, moveValues, 0, movesCount, ply);
				quietMovesGenerated = true;

#ifndef NDEBUG
				context.statistics.QuietMovesGenerated++;
#endif
			}
		} else {
			moves[0] = hashMove;
			moveValues[0] = MoveOrderingConstants.HashMove;
			movesCount = 1;
		}

		var pvs = true;
		var bestScore = 0;
		var allMovesPruned = true;

		for (var moveIndex = 0; moveIndex < movesCount; moveIndex++) {
			if (LMPCanBeApplied(context, depth, friendlyKingInCheck, quietMovesGenerated, moveIndex, movesCount, pvNode)) {
				break;
			}

			MoveOrdering.SortNextBestMove(moves, moveValues, movesCount, moveIndex);

			if (loudMovesGenerated && moves[moveIndex] == hashMove) {
				goto postLoopOperations;
			}

			if (loudMovesGenerated && !quietMovesGenerated && moveValues[moveIndex] < 100) {
				var loudMovesCount = movesCount;

				movesCount = context.session->m_position.GetQuietMoves(moves, movesCount, evasionMask);
				MoveOrdering.AssignQuietValues(context.session->m_position, moves, moveValues, loudMovesCount, movesCount, ply);
				MoveOrdering.SortNextBestMove(moves, moveValues, movesCount, moveIndex);
				quietMovesGenerated = true;

#ifndef NDEBUG
				context.statistics.QuietMovesGenerated++;
#endif

				if (moves[moveIndex] == hashMove) {
					goto postLoopOperations;
				}
			}

			if (context.MoveRestrictions != null && ply == 0) {
				if (!context.MoveRestrictions.Contains(moves[moveIndex])) {
					goto postLoopOperations;
				}
			}

			context.session->m_position.MakeMove(moves[moveIndex]);

			var enemyKingInCheck = context.session->m_position.IsKingChecked(context.session->m_position.m_colorToMove);
			var extension = GetExtensions(depth, extensionsCount, enemyKingInCheck);

#ifndef NDEBUG
			context.statistics.Extensions += extension;
#endif

			if (futilityPruningCanBeApplied && FutilityPruningCanBeAppliedForMove(context, moves[moveIndex], enemyKingInCheck, pvs)) {
				var gain = FutilityPruningGetGain(context, moves[moveIndex]);
				if (futilityPruningEvaluation + futilityPruningMargin + gain <= alpha) {
#ifndef NDEBUG
					context.statistics.FutilityPrunes++;
#endif

					context.session->m_position.UndoMove(moves[moveIndex]);
					goto postLoopOperations;
				}
			}

			allMovesPruned = false;

			if (pvs) {
				bestScore = -FindBestMove(context, depth - 1 + extension, ply + 1, -beta, -alpha, allowNullMove, enemyKingInCheck, extensionsCount + extension);
				pvs = false;
			} else {
				var lmrReduction = 0;
				if (LMRCanBeApplied(context, depth, friendlyKingInCheck, enemyKingInCheck, moveIndex, moves, moveValues)) {
					lmrReduction = LMRGetReduction(pvNode, moveIndex);
				}

				var score = -FindBestMove(context, depth - lmrReduction - 1 + extension, ply + 1, -alpha - 1, -alpha, allowNullMove, enemyKingInCheck, extensionsCount + extension);
				if (score > alpha) {
					if (pvNode) {
						score = -FindBestMove(context, depth - 1 + extension, ply + 1, -beta, -alpha, allowNullMove, enemyKingInCheck, extensionsCount + extension);
					} else {
						if (lmrReduction != 0) {
							score = -FindBestMove(context, depth - 1 + extension, ply + 1, -beta, -alpha, allowNullMove, enemyKingInCheck, extensionsCount + extension);
						}
					}
				}

				if (score > bestScore) {
					bestScore = score;
				}
			}

			context.session->m_position.UndoMove(moves[moveIndex]);

			if (bestScore > alpha) {
				alpha = bestScore;
				bestMove = moves[moveIndex];

				if (alpha >= beta) {
					if (moves[moveIndex].IsQuiet()) {
						KillerHeuristic.AddKillerMove(moves[moveIndex], context.session->m_position.m_colorToMove, ply);
						HistoryHeuristic.AddHistoryMove(context.session->m_position.m_colorToMove, context.session->m_position.PieceTable[moves[moveIndex].From], moves[moveIndex].To, depth);
					}

#ifndef NDEBUG
					if (moveIndex == 0) {
						context.statistics.BetaCutoffsAtFirstMove++;
					} else {
						context.statistics.BetaCutoffsNotAtFirstMove++;
					}
#endif

					context.statistics.BetaCutoffs++;
					break;
				}
			}

		postLoopOperations:
			if (!loudMovesGenerated) {
				movesCount = context.session->m_position.GetLoudMoves(moves, 0, evasionMask);
				MoveOrdering.AssignLoudValues(context.session->m_position, moves, moveValues, movesCount, depth, hashMove);
				moveIndex = -1;
				loudMovesGenerated = true;

#ifndef NDEBUG
				context.statistics.LoudMovesGenerated++;
#endif

				if (movesCount == 0) {
					movesCount = context.session->m_position.GetQuietMoves(moves, 0, evasionMask);
					MoveOrdering.AssignQuietValues(context.session->m_position, moves, moveValues, 0, movesCount, ply);
					quietMovesGenerated = true;

#ifndef NDEBUG
					context.statistics.QuietMovesGenerated++;
#endif
				}
			}

			if (!quietMovesGenerated && moveIndex == movesCount - 1) {
				var loudMovesCount = movesCount;

				movesCount = context.session->m_position.GetQuietMoves(moves, movesCount, evasionMask);
				MoveOrdering.AssignQuietValues(context.session->m_position, moves, moveValues, loudMovesCount, movesCount, ply);
				quietMovesGenerated = true;

#ifndef NDEBUG
				context.statistics.QuietMovesGenerated++;
#endif
			}
		}

		// Don't save invalid scores to the transposition table
		if (context.AbortSearch) {
			return 0;
		}

		if (allMovesPruned) {
			return alpha;
		}

		// Don't add invalid move (done after checkmate) to prevent strange behaviors
		if (bestScore == -board::SearchConstants::NO_KING_VALUE) {
			return bestScore;
		}

		// Return draw score or checkmate score as leafs
		if (bestScore == board::SearchConstants::NO_KING_VALUE) {
			if (friendlyKingInCheck) {
				return -EvaluationConstants.Checkmate + ply;
			}

			return 0;
		}

		if (entry.Flags == TranspositionTableEntryFlags.Invalid || alpha != originalAlpha) {
			if (entry.Age != context.TranspositionTableEntryAge || entry.Depth <= depth) {
				var valueToSave = alpha;
				var entryType = alpha <= originalAlpha ? TranspositionTableEntryFlags.AlphaScore : alpha >= beta ? TranspositionTableEntryFlags.BetaScore
																												 : TranspositionTableEntryFlags.ExactScore;

				valueToSave = TranspositionTable.RegularToTTScore(alpha, ply);

				TranspositionTable.Add(context.session->m_position.Hash, new TranspositionTableEntry(context.session->m_position.Hash, (short)valueToSave, bestMove, (byte)depth, entryType, (byte)context.TranspositionTableEntryAge));

#ifndef NDEBUG
				if (entry.Flags != TranspositionTableEntryFlags.Invalid) {
					context.statistics.TTReplacements++;
				}

				context.statistics.TTAddedEntries++;
#endif
			}
		}

		return bestScore;*/
		return 0;
	}

private:
	/*static bool RazoringCanBeApplied(int depth, int rootDepth, bool friendlyKingInCheck, bool pvNode, int alpha) {
		return !pvNode && depth >= SearchConstants.RazoringMinDepth && depth <= SearchConstants.RazoringMaxDepth && !friendlyKingInCheck && !IterativeDeepening.IsScoreNearCheckmate(alpha);
	}

	static bool StaticNullMoveCanBeApplied(int depth, int rootDepth, bool friendlyKingInCheck, bool pvNode, int beta) {
		var maxDepth = SearchConstants.StaticNullMoveMaxDepth + rootDepth / SearchConstants.StaticNullMoveMaxDepthDivider;
		return !pvNode && depth <= maxDepth && !friendlyKingInCheck && !IterativeDeepening.IsScoreNearCheckmate(beta);
	}

	static bool NullMoveCanBeApplied(BoardState board, int depth, bool allowNullMove, bool pvNode, bool friendlyKingInCheck) {
		return !pvNode && allowNullMove && depth >= SearchConstants.NullMoveMinDepth &&
				board.GetGamePhase() == GamePhase.Opening && !friendlyKingInCheck;
	}

	static int NullMoveGetReduction(int depth) {
		return SearchConstants.NullMoveDepthReduction + (depth - SearchConstants.NullMoveMinDepth) / SearchConstants.NullMoveDepthReductionDivider;
	}

	static bool IIDCanBeApplied(int depth, TranspositionTableEntryFlags ttEntryType, Move bestMove) {
		return ttEntryType == TranspositionTableEntryFlags.Invalid && depth >= SearchConstants.IIDMinDepth && bestMove == Move.Empty;
	}

	static bool FutilityPruningCanBeApplied(int depth, int rootDepth, bool friendlyKingInCheck, bool pvNode, int alpha) {
		var maxDepth = SearchConstants.FutilityPruningMaxDepth + rootDepth / SearchConstants.FutilityPruningMaxDepthDivisor;
		return !pvNode && depth <= maxDepth && !friendlyKingInCheck && !IterativeDeepening.IsScoreNearCheckmate(alpha);
	}

	static bool FutilityPruningCanBeAppliedForMove(SearchContext context, Move move, bool enemyKingInCheck, bool pvMove) {
		if (enemyKingInCheck) {
			return false;
		}

		if (context.session->m_position.PieceTable[move.From] == common::PieceType::PAWN) {
			if (context.session->m_position.IsFieldPassing(context.session->m_position.m_colorToMove, move.To)) {
				return false;
			}
		}

		return !pvMove && !move.IsPromotion();
	}

	static int FutilityPruningGetGain(SearchContext context, Move move) {
		if (move.IsCapture()) {
			var capturedPiece = context.session->m_position.PieceTable[move.To];
			return capturedPiece != -1 ? EvaluationConstants.Pieces[capturedPiece] : 100;
		}

		return 0;
	}

	static bool LMPCanBeApplied(SearchContext context, int depth, bool friendlyKingInCheck, bool quietMovesGenerated, int moveIndex, int movesCount, bool pvNode) {
		return depth <= SearchConstants.LMPMaxDepth && !pvNode && !friendlyKingInCheck && quietMovesGenerated &&
				moveIndex >= (SearchConstants.LMPBasePercentMovesToPrune + (depth - 1) * SearchConstants.LMPPercentIncreasePerDepth) * movesCount / 100;
	}

	static bool LMRCanBeApplied(SearchContext context, int depth, bool friendlyKingInCheck, bool enemyKingInCheck, int moveIndex, Span<Move> moves, Span<short> moveValues) {
		if (depth >= SearchConstants.LMRMinDepth && moveIndex >= SearchConstants.LMRMovesWithoutReduction &&
				(moves[moveIndex].IsQuiet() || (moves[moveIndex].IsCapture() && moveValues[moveIndex] < 0)) && !friendlyKingInCheck && !enemyKingInCheck) {
			var enemyColor = ColorOperations.Invert(context.session->m_position.m_colorToMove);
			var piece = context.session->m_position.PieceTable[moves[moveIndex].To];

			// TODO: Figure out why piece is sometimes `-1`
			if (piece != -1 && HistoryHeuristic.GetRawMoveValue(enemyColor, piece, moves[moveIndex].To) >= HistoryHeuristic.GetMaxValue() / SearchConstants.LMRMaxHistoryValueDivider) {
				return false;
			}

			if (piece == common::PieceType::PAWN && context.session->m_position.isFieldPassing(enemyColor, moves[moveIndex].To)) {
				return false;
			}

#ifndef NDEBUG
			context.statistics.LMRReductions++;
#endif

			return true;
		}

		return false;
	}

	static int LMRGetReduction(bool pvNode, int moveIndex) {
		var r = SearchConstants.LMRBaseReduction + (moveIndex - SearchConstants.LMRMovesWithoutReduction) / SearchConstants.LMRMoveIndexDivider;
		return Math.Min(pvNode ? SearchConstants.LMRPvNodeMaxReduction : SearchConstants.LMRNonPvNodeMaxReduction, r);
	}

	static int GetExtensions(int depth, int extensionsCount, bool enemyKingInCheck) {
		if (depth == 1 && extensionsCount == 0 && enemyKingInCheck) {
			return 1;
		}

		return 0;
	}*/
};

} //namespace phase4::engine::ai::search

#endif
