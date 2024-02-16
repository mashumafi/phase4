#ifndef PHASE4_ENGINE_AI_SEARCH_NEGA_MAX_H
#define PHASE4_ENGINE_AI_SEARCH_NEGA_MAX_H

#include <phase4/engine/ai/score/evaluation.h>
#include <phase4/engine/ai/search/quiescence_search.h>
#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/operators.h>
#include <phase4/engine/board/ordering/move_ordering.h>
#include <phase4/engine/board/ordering/move_ordering_constants.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/util.h>

#include <cstdint>

namespace phase4::engine::ai::search {

class NegaMax {
public:
	static int32_t findBestMove(SearchContext &context, int32_t depth, int32_t ply, int32_t alpha, int32_t beta) {
		const bool friendlyKingInCheck = context.session->isKingChecked(context.session->position().m_colorToMove);
		return findBestMove(context, depth, ply, alpha, beta, true, friendlyKingInCheck, 0);
	}

	static int32_t findBestMove(SearchContext &context, int32_t depth, int32_t ply, int32_t alpha, int32_t beta, bool allowNullMove, bool friendlyKingInCheck, int32_t extensionsCount) {
		using namespace common;
		using namespace board::transposition;

		if (context.statistics.nodes >= context.maxNodesCount) {
			context.abortSearch = true;
			return 0;
		}

		if (context.abortSearch) {
			return 0;
		}

		context.statistics.nodes++;

		if (context.session->position().colorPieceMask(context.session->position().m_colorToMove, common::PieceType::KING) == 0) {
			context.statistics.leafs++;
			return board::SearchConstants::NO_KING_VALUE;
		}

		if (context.session->position().isKingChecked(context.session->position().m_colorToMove.invert())) {
			context.statistics.leafs++;
			return -board::SearchConstants::NO_KING_VALUE;
		}

		if (context.session->isThreefoldRepetition()) {
			context.statistics.leafs++;
			return board::EvaluationConstants::THREEFOLD_REPETITION;
		}

		if (context.session->position().isInsufficientMaterial()) {
			const common::PieceColor enemyColor = context.session->position().m_colorToMove.invert();
			if (!friendlyKingInCheck && !context.session->position().isKingChecked(enemyColor)) {
				context.statistics.leafs++;
				return board::EvaluationConstants::INSUFFICIENT_MATERIAL;
			}
		}

		if (context.session->position().isFiftyMoveRuleDraw()) {
			context.statistics.leafs++;
			return board::EvaluationConstants::THREEFOLD_REPETITION;
		}

		if (depth <= 0) {
			context.statistics.leafs++;
			return QuiescenceSearch::findBestMove(context, depth, ply, alpha, beta);
		}

		const int32_t originalAlpha = alpha;
		const bool pvNode = beta - alpha > 1;

		TranspositionTableEntry &entry = context.session->m_hashTables.m_transpositionTable.get(context.session->position().m_hash.asBitboard());
		moves::Move hashMove = moves::Move::Empty;
		moves::Move bestMove = moves::Move::Empty;

		if (entry.flags() != TranspositionTableEntryFlags::INVALID && entry.isKeyValid(context.session->position().m_hash.asBitboard())) {
#ifndef NDEBUG
			++context.statistics.transpositionTableHits;
#endif
			if (entry.flags() != TranspositionTableEntryFlags::ALPHA_SCORE) {
				const bool isMoveLegal = board::Operators::isMoveLegal(context.session->position(), entry.bestMove());
				if (isMoveLegal) {
					hashMove = entry.bestMove();
					bestMove = entry.bestMove();
#ifndef NDEBUG
					++context.statistics.transpositionTableValidMoves;
#endif
				}
#ifndef NDEBUG
				else {
					++context.statistics.transpositionTableInvalidMoves;
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
			++context.statistics.transpositionTableNonHits;
		}
#endif

		if (razoringCanBeApplied(depth, friendlyKingInCheck, pvNode, alpha)) {
			const int32_t fastEvaluation = score::Evaluation::fastEvaluate(*context.session, context.statistics.evaluationStatistics);
			const int32_t margin = board::SearchConstants::RAZORING_MARGIN + (depth - board::SearchConstants::RAZORING_MIN_DEPTH) * board::SearchConstants::RAZORING_MARGIN_MULTIPLIER;
			const int32_t futileAlpha = alpha - margin;

			if (fastEvaluation < futileAlpha) {
				const int32_t result = QuiescenceSearch::findBestMove(context, depth, ply, futileAlpha, futileAlpha + 1);
				if (result <= futileAlpha) {
#ifndef NDEBUG
					++context.statistics.razorings;
#endif
					return futileAlpha;
				}
#ifndef NDEBUG
				else {
					++context.statistics.razoringsRejected;
				}
#endif
			}
		}

		if (staticNullMoveCanBeApplied(depth, context.statistics.depth, friendlyKingInCheck, pvNode, beta)) {
			const int32_t fastEvaluation = score::Evaluation::fastEvaluate(*context.session, context.statistics.evaluationStatistics);
			const int32_t margin = board::SearchConstants::STATIC_NULL_MOVE_MARGIN + (depth - 1) * board::SearchConstants::STATIC_NULL_MOVE_MARGIN_MULTIPLIER;
			const int32_t score = fastEvaluation - margin;

			if (score >= beta) {
#ifndef NDEBUG
				++context.statistics.staticNullMovePrunes;
#endif

				return score;
			}
		}

		if (nullMoveCanBeApplied(*context.session, depth, allowNullMove, pvNode, friendlyKingInCheck)) {
			context.session->makeNullMove();
			const int32_t score = -findBestMove(context, depth - 1 - nullMoveGetReduction(depth), ply + 1, -beta, -beta + 1, false, false, extensionsCount);
			context.session->undoNullMove();

			if (score >= beta) {
#ifndef NDEBUG
				context.statistics.nullMovePrunes++;
#endif

				return score;
			}
		}

		if (internalIterativeDeepeningCanBeApplied(depth, entry.flags(), hashMove)) {
			findBestMove(context, depth - 1 - board::SearchConstants::INTERNAL_ITERATIVE_DEEPENING_DEPTH_REDUCTION, ply, alpha, beta, allowNullMove, friendlyKingInCheck, extensionsCount);

			const TranspositionTableEntry &internalIterativeDeepeningEntry = context.session->m_hashTables.m_transpositionTable.get(context.session->position().m_hash.asBitboard());
			if (internalIterativeDeepeningEntry.isKeyValid(context.session->position().m_hash.asBitboard())) {
				hashMove = internalIterativeDeepeningEntry.bestMove();

#ifndef NDEBUG
				++context.statistics.internalIterativeDeepeningHits;
#endif
			}
		}

		bool futilityPruningCanBeAppliedResult = false;
		int32_t futilityPruningEvaluation = 0;
		int32_t futilityPruningMargin = 0;

		if (futilityPruningCanBeApplied(depth, context.statistics.depth, friendlyKingInCheck, pvNode, alpha)) {
			futilityPruningCanBeAppliedResult = true;
			futilityPruningEvaluation = score::Evaluation::fastEvaluate(*context.session, context.statistics.evaluationStatistics);
			futilityPruningMargin = board::SearchConstants::FUTILITY_PRUNING_MARGIN + (depth - 1) * board::SearchConstants::FUTILITY_PRUNING_MARGIN_MULTIPLIER;
		}

		moves::Moves moves;
		moves::MoveValues moveValues;

		bool loudMovesGenerated = false;
		bool quietMovesGenerated = false;

		Bitset evasionMask = Bitset::MAX;
		if (friendlyKingInCheck && !context.session->position().isKingChecked(context.session->position().m_colorToMove.invert())) {
			const Bitset kingField = context.session->position().colorPieceMask(context.session->position().m_colorToMove, common::PieceType::KING);
			const Square kingFieldIndex(kingField);

			evasionMask = moves::MovesGenerator::getKnightMoves(kingFieldIndex) |
					moves::MovesGenerator::getQueenMoves(context.session->position().m_occupancySummary, kingFieldIndex);
		}

		if (hashMove == moves::Move::Empty) {
			board::Operators::getLoudMoves(context.session->position(), moves, evasionMask);
			board::ordering::MoveOrdering::assignLoudValues(context.session->position(), moves, moveValues, moves::Move::Empty);
			loudMovesGenerated = true;

#ifndef NDEBUG
			++context.statistics.loudMovesGenerated;
#endif

			if (moves.is_empty()) {
				board::Operators::getQuietMoves(context.session->position(), moves, evasionMask);
				board::ordering::MoveOrdering::assignQuietValues(*context.session, moves, moveValues, 0, ply);
				quietMovesGenerated = true;

#ifndef NDEBUG
				++context.statistics.quietMovesGenerated;
#endif
			}
		} else {
			assert(moves.is_empty()); // TODO: remove assert
			assert(moveValues.is_empty()); // TODO: remove assert
			moves.push_back(hashMove);
			moveValues.push_back(board::ordering::MoveOrderingConstants::HASH_MOVE);
		}

		bool pvs = true;
		int32_t bestScore = 0;
		bool allMovesPruned = true;

		for (size_t moveIndex = 0; moveIndex < moves.size(); ++moveIndex) {
			const bool postLoopOperations = std::invoke([&]() -> bool {
				if (lateMovePruningCanBeApplied(depth, friendlyKingInCheck, quietMovesGenerated, moveIndex, moves.size(), pvNode)) {
					return false;
				}

				board::ordering::MoveOrdering::sortNextBestMove(moves, moveValues, moveIndex);

				if (loudMovesGenerated && moves[moveIndex] == hashMove) {
					return true;
				}

				if (loudMovesGenerated && !quietMovesGenerated && moveValues[moveIndex] < 100) {
					const size_t loudMovesCount = moves.size();

					board::Operators::getQuietMoves(context.session->position(), moves, evasionMask);
					board::ordering::MoveOrdering::assignQuietValues(*context.session, moves, moveValues, loudMovesCount, ply);
					board::ordering::MoveOrdering::sortNextBestMove(moves, moveValues, moveIndex);
					quietMovesGenerated = true;

#ifndef NDEBUG
					++context.statistics.quietMovesGenerated;
#endif

					if (moves[moveIndex] == hashMove) {
						return true;
					}
				}

				if (!context.moveRestrictions.is_empty() && ply == 0) {
					if (!context.moveRestrictions.contains(moves[moveIndex])) {
						return true;
					}
				}

				context.session->makeMove(moves[moveIndex]);

				const bool enemyKingInCheck = context.session->position().isKingChecked(context.session->position().m_colorToMove);
				const int32_t extension = getExtensions(depth, extensionsCount, enemyKingInCheck);

#ifndef NDEBUG
				context.statistics.extensions += extension;
#endif

				if (futilityPruningCanBeAppliedResult && futilityPruningCanBeAppliedForMove(context, moves[moveIndex], enemyKingInCheck, pvs)) {
					int32_t gain = futilityPruningGetGain(context, moves[moveIndex]);
					if (futilityPruningEvaluation + futilityPruningMargin + gain <= alpha) {
#ifndef NDEBUG
						++context.statistics.futilityPrunes;
#endif

						context.session->undoMove(moves[moveIndex]);
						return true;
					}
				}

				allMovesPruned = false;

				if (pvs) {
					bestScore = -findBestMove(context, depth - 1 + extension, ply + 1, -beta, -alpha, allowNullMove, enemyKingInCheck, extensionsCount + extension);
					pvs = false;
				} else {
					int32_t lateMoveReductionsReduction = 0;
					if (lateMoveReductionsCanBeApplied(context, depth, friendlyKingInCheck, enemyKingInCheck, moveIndex, moves, moveValues)) {
						lateMoveReductionsReduction = lateMoveReductionsGetReduction(pvNode, moveIndex);
					}

					int32_t score = -findBestMove(context, depth - lateMoveReductionsReduction - 1 + extension, ply + 1, -alpha - 1, -alpha, allowNullMove, enemyKingInCheck, extensionsCount + extension);
					if (score > alpha) {
						if (pvNode) {
							score = -findBestMove(context, depth - 1 + extension, ply + 1, -beta, -alpha, allowNullMove, enemyKingInCheck, extensionsCount + extension);
						} else {
							if (lateMoveReductionsReduction != 0) {
								score = -findBestMove(context, depth - 1 + extension, ply + 1, -beta, -alpha, allowNullMove, enemyKingInCheck, extensionsCount + extension);
							}
						}
					}

					if (score > bestScore) {
						bestScore = score;
					}
				}

				context.session->undoMove(moves[moveIndex]);

				if (bestScore > alpha) {
					alpha = bestScore;
					bestMove = moves[moveIndex];

					if (alpha >= beta) {
						if (moves[moveIndex].flags().isQuiet()) {
							context.session->m_killerHeuristic.addKillerMove(moves[moveIndex], context.session->position().m_colorToMove, ply);
							context.session->m_historyHeuristic.addHistoryMove(context.session->position().m_colorToMove, context.session->position().m_pieceTable[moves[moveIndex].from()], moves[moveIndex].to(), depth);
						}

#ifndef NDEBUG
						if (moveIndex == 0) {
							++context.statistics.betaCutoffsAtFirstMove;
						} else {
							++context.statistics.betaCutoffsNotAtFirstMove;
						}
#endif

						++context.statistics.betaCutoffs;
						return false;
					}
				}

				return true;
			});

			if (!postLoopOperations)
				break;

			if (!loudMovesGenerated) {
				moves.clear(); // Move values gets resized and set below
				board::Operators::getLoudMoves(context.session->position(), moves, evasionMask);
				board::ordering::MoveOrdering::assignLoudValues(context.session->position(), moves, moveValues, hashMove);
				moveIndex = common::util::back_index(0); // restart iteration
				loudMovesGenerated = true;

#ifndef NDEBUG
				++context.statistics.loudMovesGenerated;
#endif

				if (moves.is_empty()) {
					board::Operators::getQuietMoves(context.session->position(), moves, evasionMask);
					board::ordering::MoveOrdering::assignQuietValues(*context.session, moves, moveValues, 0, ply);
					quietMovesGenerated = true;

#ifndef NDEBUG
					++context.statistics.quietMovesGenerated;
#endif
				}
			}

			if (!quietMovesGenerated && moveIndex == moves.size() - 1) {
				const size_t loudMovesCount = moves.size();

				board::Operators::getQuietMoves(context.session->position(), moves, evasionMask);
				board::ordering::MoveOrdering::assignQuietValues(*context.session, moves, moveValues, loudMovesCount, ply);
				quietMovesGenerated = true;

#ifndef NDEBUG
				++context.statistics.quietMovesGenerated;
#endif
			}
		}

		// Don't save invalid scores to the transposition table
		if (context.abortSearch) {
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
				return -board::EvaluationConstants::CHECKMATE + ply;
			}

			return 0;
		}

		if (entry.flags() == TranspositionTableEntryFlags::INVALID || alpha != originalAlpha) {
			if (entry.age() != context.transpositionTableEntryAge || entry.depth() <= depth) {
				const TranspositionTableEntryFlags entryType = std::invoke([originalAlpha, alpha, beta]() -> TranspositionTableEntryFlags {
					if (alpha <= originalAlpha) {
						return TranspositionTableEntryFlags::ALPHA_SCORE;
					}

					if (alpha >= beta) {
						return TranspositionTableEntryFlags::BETA_SCORE;
					}

					return TranspositionTableEntryFlags::EXACT_SCORE;
				});

#ifndef NDEBUG
				if (entry.flags() != TranspositionTableEntryFlags::INVALID) {
					++context.statistics.transpositionTableReplacements;
				} else {
					++context.statistics.transpositionTableAddedEntries;
				}
#endif

				const int32_t valueToSave = TranspositionTable<0>::regularToTranspositionTableScore(alpha, ply);
				const TranspositionTableEntry newEntry(context.session->position().m_hash.asBitboard(), static_cast<int16_t>(valueToSave), bestMove, static_cast<uint8_t>(depth), entryType, static_cast<uint8_t>(context.transpositionTableEntryAge));
				context.session->m_hashTables.m_transpositionTable.add(context.session->position().m_hash.asBitboard(), newEntry);
			}
		}

		return bestScore;
	}

private:
	static bool razoringCanBeApplied(int32_t depth, bool friendlyKingInCheck, bool pvNode, int32_t alpha) {
		return !pvNode && depth >= board::SearchConstants::RAZORING_MIN_DEPTH && depth <= board::SearchConstants::RAZORING_MAX_DEPTH && !friendlyKingInCheck && !board::SearchConstants::isScoreNearCheckmate(alpha);
	}

	static bool staticNullMoveCanBeApplied(int32_t depth, int32_t rootDepth, bool friendlyKingInCheck, bool pvNode, int32_t beta) {
		int32_t maxDepth = board::SearchConstants::STATIC_NULL_MOVE_MAX_DEPTH + rootDepth / board::SearchConstants::STATIC_NULL_MOVE_MAX_DEPTH_DIVIDER;
		return !pvNode && depth <= maxDepth && !friendlyKingInCheck && !board::SearchConstants::isScoreNearCheckmate(beta);
	}

	static bool nullMoveCanBeApplied(const board::Session &session, int32_t depth, bool allowNullMove, bool pvNode, bool friendlyKingInCheck) {
		return !pvNode && allowNullMove && depth >= board::SearchConstants::NULL_MOVE_MIN_DEPTH &&
				session.position().getGamePhase() == common::GamePhase::OPENING && !friendlyKingInCheck;
	}

	static int32_t nullMoveGetReduction(int32_t depth) {
		return board::SearchConstants::NULL_MOVE_DEPTH_REDUCTION + (depth - board::SearchConstants::NULL_MOVE_MIN_DEPTH) / board::SearchConstants::NULL_MOVE_DEPTH_REDUCTION_DIVIDER;
	}

	static bool internalIterativeDeepeningCanBeApplied(int32_t depth, board::transposition::TranspositionTableEntryFlags transpositionTableEntryType, moves::Move bestMove) {
		return transpositionTableEntryType == board::transposition::TranspositionTableEntryFlags::INVALID && depth >= board::SearchConstants::INTERNAL_ITERATIVE_DEEPENING_MIN_DEPTH && bestMove == moves::Move::Empty;
	}

	static bool futilityPruningCanBeApplied(int32_t depth, int32_t rootDepth, bool friendlyKingInCheck, bool pvNode, int32_t alpha) {
		const int32_t maxDepth = board::SearchConstants::FUTILITY_PRUNING_MAX_DEPTH + rootDepth / board::SearchConstants::FUTILITY_PRUNING_MAX_DEPTH_DIVISOR;
		return !pvNode && depth <= maxDepth && !friendlyKingInCheck && !board::SearchConstants::isScoreNearCheckmate(alpha);
	}

	static bool futilityPruningCanBeAppliedForMove(const SearchContext &context, moves::Move move, bool enemyKingInCheck, bool pvMove) {
		if (enemyKingInCheck) {
			return false;
		}

		if (context.session->position().m_pieceTable[move.from()] == common::PieceType::PAWN) {
			if (context.session->position().isFieldPassing(context.session->position().m_colorToMove, move.to())) {
				return false;
			}
		}

		return !pvMove && !move.flags().isPromotion();
	}

	static int32_t futilityPruningGetGain(const SearchContext &context, moves::Move move) {
		if (move.flags().isCapture()) {
			const common::PieceType capturedPiece = context.session->position().m_pieceTable[move.to()];
			if (capturedPiece != common::PieceType::INVALID) {
				return board::EvaluationConstants::pieceValue(capturedPiece);
			} else {
				return 100;
			}
		}

		return 0;
	}

	static bool lateMovePruningCanBeApplied(int32_t depth, bool friendlyKingInCheck, bool quietMovesGenerated, size_t moveIndex, size_t movesCount, bool pvNode) {
		return depth <= board::SearchConstants::LATE_MOVE_PRUNING_MAX_DEPTH && !pvNode && !friendlyKingInCheck && quietMovesGenerated &&
				moveIndex >= (board::SearchConstants::LATE_MOVE_PRUNING_BASE_PERCENT_MOVES_TO_PRUNE + (depth - 1) * board::SearchConstants::LATE_MOVE_PRUNING_PERCENT_INCREASE_PER_DEPTH) * movesCount / 100;
	}

	static bool lateMoveReductionsCanBeApplied(SearchContext &context, int32_t depth, bool friendlyKingInCheck, bool enemyKingInCheck, size_t moveIndex, const moves::Moves &moves, const moves::MoveValues &moveValues) {
		if (depth >= board::SearchConstants::LATE_MOVE_REDUCTIONS_MIN_DEPTH && moveIndex >= board::SearchConstants::LATE_MOVE_REDUCTIONS_MOVES_WITHOUT_REDUCTION &&
				(moves[moveIndex].flags().isQuiet() || (moves[moveIndex].flags().isCapture() && moveValues[moveIndex] < 0)) && !friendlyKingInCheck && !enemyKingInCheck) {
			const common::PieceColor enemyColor = context.session->position().m_colorToMove.invert();
			const common::PieceType piece = context.session->position().m_pieceTable[moves[moveIndex].to()];

			// TODO: Figure out why piece is sometimes INVALID
			if (piece != common::PieceType::INVALID && context.session->m_historyHeuristic.getRawMoveValue(enemyColor, piece, moves[moveIndex].to()) >= context.session->m_historyHeuristic.getMaxValue() / board::SearchConstants::LATE_MOVE_REDUCTIONS_MAX_HISTORY_VALUE_DIVIDER) {
				return false;
			}

			if (piece == common::PieceType::PAWN && context.session->position().isFieldPassing(enemyColor, moves[moveIndex].to())) {
				return false;
			}

#ifndef NDEBUG
			++context.statistics.lateMoveReductionsReductions;
#endif

			return true;
		}

		return false;
	}

	static int32_t lateMoveReductionsGetReduction(bool pvNode, size_t moveIndex) {
		const int32_t reductions = board::SearchConstants::LATE_MOVE_REDUCTIONS_BASE_REDUCTION + (moveIndex - board::SearchConstants::LATE_MOVE_REDUCTIONS_MOVES_WITHOUT_REDUCTION) / board::SearchConstants::LATE_MOVE_REDUCTIONS_MOVE_INDEX_DIVIDER;
		return common::Math::min_int32(pvNode ? board::SearchConstants::LATE_MOVE_REDUCTIONS_PV_NODE_MAX_REDUCTION : board::SearchConstants::LATE_MOVE_REDUCTIONS_NON_PV_NODE_MAX_REDUCTION, reductions);
	}

	static int32_t getExtensions(int32_t depth, int32_t extensionsCount, bool enemyKingInCheck) {
		if (depth == 1 && extensionsCount == 0 && enemyKingInCheck) {
			return 1;
		}

		return 0;
	}
};

} //namespace phase4::engine::ai::search

#endif
