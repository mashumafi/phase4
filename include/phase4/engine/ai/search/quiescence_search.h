#ifndef PHASE4_ENGINE_AI_SEARCH_QUIESCENCE_SEARCH_H
#define PHASE4_ENGINE_AI_SEARCH_QUIESCENCE_SEARCH_H

#include <phase4/engine/ai/search/search_context.h>

#include <cstdint>

namespace phase4::engine::ai::search {

class QuiescenceSearch {
public:
	static int32_t findBestMove(SearchContext &context, int32_t depth, int32_t ply, int32_t alpha, int32_t beta) {
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
