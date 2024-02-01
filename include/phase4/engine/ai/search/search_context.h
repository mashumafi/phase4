#ifndef PHASE4_ENGINE_AI_SEARCH_SEARCH_CONTEXT_H
#define PHASE4_ENGINE_AI_SEARCH_SEARCH_CONTEXT_H

#include <phase4/engine/ai/search/search_statistics.h>

#include <phase4/engine/board/search_constants.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/move.h>

#include <atomic>
#include <climits>
#include <cstdint>

namespace phase4::engine::ai::search {

class CancellationToken {
public:
	CancellationToken() :
			m_canceled(false) {}

	bool isCanceled() const {
		return m_canceled.load();
	}

	void cancel() {
		m_canceled.store(true);
	}

private:
	std::atomic<bool> m_canceled;
};

struct SearchContext {
public:
	board::Session *session;
	SearchStatistics statistics;
	CancellationToken cancellation;
	uint8_t maxDepth;
	uint32_t maxTime;
	bool abortSearch = false;
	bool waitForStopCommand;
	uint64_t maxNodesCount;
	moves::Moves moveRestrictions;
	uint16_t transpositionTableEntryAge;

	SearchContext(board::Session *session) :
			session(session),
			statistics(),
			maxDepth(board::SearchConstants::MAX_DEPTH),
			maxTime(std::numeric_limits<uint32_t>::max()),
			waitForStopCommand(false),
			maxNodesCount(std::numeric_limits<uint64_t>::max()),
			moveRestrictions(),
			transpositionTableEntryAge(session->m_position.m_movesCount) {
	}
};

} //namespace phase4::engine::ai::search

#endif
