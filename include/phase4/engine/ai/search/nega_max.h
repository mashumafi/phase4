#ifndef PHASE4_ENGINE_AI_SEARCH_NEGA_MAX_H
#define PHASE4_ENGINE_AI_SEARCH_NEGA_MAX_H

#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/board/ordering/move_ordering.h>

#include <phase4/engine/board/session.h>

#include <cstdint>

namespace phase4::engine::ai::search {

class NegaMax {
public:
	static int findBestMove(SearchContext &context, int depth, int ply, int alpha, int beta) {
		(void)context;
		(void)depth;
		(void)ply;
		(void)alpha;
		(void)beta;
		return 0;
	}
};

} //namespace phase4::engine::ai::search

#endif
