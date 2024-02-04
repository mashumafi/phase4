#ifndef PHASE4_ENGINE_AI_SEARCH_ITERATIVE_DEEPENING_H
#define PHASE4_ENGINE_AI_SEARCH_ITERATIVE_DEEPENING_H

#include <phase4/engine/ai/search/nega_max.h>
#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/board/search_constants.h>
#include <phase4/engine/board/session.h>
#include <phase4/engine/board/transposition/transposition_table_entry.h>
#include <phase4/engine/board/transposition/transposition_table_entry_flags.h>

#include <phase4/engine/moves/move.h>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <functional>
#include <thread>

namespace phase4::engine::ai::search {

class Stopwatch {
public:
	void start() {
		m_startTime = std::chrono::high_resolution_clock::now();
		m_isRunning = true;
	}

	void stop() {
		m_endTime = std::chrono::high_resolution_clock::now();
		m_isRunning = false;
	}

	int64_t elapsedMilliseconds() const {
		auto end = m_isRunning ? std::chrono::high_resolution_clock::now() : m_endTime;
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_startTime);
		return duration.count();
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;
	bool m_isRunning = false;
};

class IterativeDeepening {
public:
	using SearchUpdateCallback = std::function<void(const SearchStatistics &)>;
	static constexpr auto DEFAULT_SEARCH_UPDATE_CALLBACK = [](const ai::search::SearchStatistics &) {};

	static moves::Move findBestMove(SearchContext &context, const SearchUpdateCallback &searchUpdateCallback = DEFAULT_SEARCH_UPDATE_CALLBACK) {
		context.session->m_historyHeuristic.ageValues();
		context.session->m_killerHeuristic.ageKillers();

		int32_t expectedExecutionTime = 0;
		const int32_t alpha = board::SearchConstants::MIN_VALUE;
		const int32_t beta = board::SearchConstants::MAX_VALUE;
		uint32_t lastSearchTime = 0ul;
		moves::Move bestMove = moves::Move::Empty;
		Stopwatch stopwatch;
		stopwatch.start();

		context.statistics = SearchStatistics();

		for (int32_t depth = 1; shouldContinueDeepening(context, depth, expectedExecutionTime); ++depth) {
			context.statistics.session = context.session;
			context.statistics.depth = depth;
			context.statistics.score = NegaMax::findBestMove(context, depth, 0, alpha, beta);
			context.statistics.searchTime = stopwatch.elapsedMilliseconds();

			if (context.abortSearch) {
				break;
			}

			context.statistics.principalVariation.clear();
			getPrincipalVariation(*context.session, context.statistics.principalVariation);
			bestMove = context.statistics.principalVariation[0];

			searchUpdateCallback(context.statistics);

			if (lastSearchTime != 0) {
				float ratio = (float)context.statistics.searchTime / lastSearchTime;
				expectedExecutionTime = (int32_t)(context.statistics.searchTime * ratio);
			}

			lastSearchTime = context.statistics.searchTime;
		}

		while (context.waitForStopCommand) {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1000ms); // sleep 1 second
		}

		//context.AbortSearch = false; // TODO: Refactor aborting flag
		// context.cancellation.cancel(); // TODO: Figure out cancellation

		return bestMove;
	}

	static bool shouldContinueDeepening(const SearchContext &context, int32_t depth, int32_t expectedExecutionTime) {
		if (board::SearchConstants::isScoreNearCheckmate(context.statistics.score)) {
			if (depth - 1 >= getMovesToCheckmate(context.statistics.score) * 2) {
				return false;
			}
		}

		return depth < context.maxDepth && expectedExecutionTime <= context.maxTime;
	}

	static int32_t getMovesToCheckmate(int32_t score) {
		return (int32_t)std::ceil((float)std::abs(std::abs(score) - board::EvaluationConstants::CHECKMATE) / 2);
	}

private:
	static void getPrincipalVariation(board::Session &session, moves::Moves &moves) {
		using namespace board::transposition;

		const TranspositionTableEntry &entry = session.m_hashTables.m_transpositionTable.get(session.position().m_hash.asBitboard());
		if (entry.flags() == TranspositionTableEntryFlags::EXACT_SCORE && entry.isKeyValid(session.position().m_hash.asBitboard()) && moves.size() < board::SearchConstants::MAX_DEPTH) {
			if (!board::Operators::isMoveLegal(session.position(), entry.bestMove())) {
				return;
			}

			moves.push_back(entry.bestMove());
			session.makeMove(entry.bestMove());

			const common::PieceColor enemyColor = session.position().m_colorToMove.invert();
			const common::Bitset king = session.position().colorPieceMask(enemyColor, common::PieceType::KING);
			const common::Square kingField(king.bitScan());

			if (session.position().isFieldAttacked(enemyColor, kingField)) {
				session.undoMove(entry.bestMove());
				return;
			}

			getPrincipalVariation(session, moves);
			session.undoMove(entry.bestMove());
		}
	}
};

} //namespace phase4::engine::ai::search

#endif
