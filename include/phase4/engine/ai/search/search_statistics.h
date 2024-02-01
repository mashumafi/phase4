#ifndef PHASE4_ENGINE_AI_SEARCH_SEARCH_STATISTICS_H
#define PHASE4_ENGINE_AI_SEARCH_SEARCH_STATISTICS_H

#include <phase4/engine/ai/score/evaluation_statistics.h>

#include <phase4/engine/board/session.h>

#include <cstdint>

namespace phase4::engine::ai::search {

class SearchStatistics {
public:
	board::Session *session;
	ai::score::EvaluationStatistics evaluationStatistics;

	int32_t depth = 0;
	int32_t selectiveDepth = 0;
	int32_t score = 0;
	uint64_t searchTime = 0;

	uint64_t nodes = 0;
	uint64_t qNodes = 0;
	uint64_t totalNodes() const { return nodes + qNodes; }

	uint64_t leafs = 0;
	uint64_t qLeafs = 0;
	uint64_t totalLeafs() const { return leafs + qLeafs; }

	uint64_t totalNodesPerSecond() const { return totalNodes() / (searchTime / 1000); }
	float secondsPerNode() const { return static_cast<float>(searchTime) / 1000 / nodes; }

	float branchingFactor() const { return static_cast<float>(nodes) / (nodes - leafs); }
	float qBranchingFactor() const { return static_cast<float>(qNodes) / (qNodes - qLeafs); }
	float totalBranchingFactor() const { return static_cast<float>(totalNodes()) / (totalNodes() - totalLeafs()); }

	uint64_t betaCutoffs = 0;
	uint64_t qBetaCutoffs = 0;
	uint64_t totalBetaCutoffs() const { return betaCutoffs + qBetaCutoffs; }

	uint64_t transpositionTableAddedEntries = 0;
	uint64_t transpositionTableReplacements = 0;
	uint64_t transpositionTableHits = 0;
	uint64_t transpositionTableNonHits = 0;
	uint64_t transpositionTableInvalidMoves = 0;
	uint64_t transpositionTableValidMoves = 0;
	float transpositionTableHitsPercent() const { return static_cast<float>(transpositionTableHits) * 100 / (transpositionTableHits + transpositionTableNonHits); }
	float transpositionTableReplacesPercent() const { return static_cast<float>(transpositionTableReplacements) * 100 / transpositionTableAddedEntries; }

	int32_t betaCutoffsAtFirstMove = 0;
	int32_t qBetaCutoffsAtFirstMove = 0;
	int32_t totalBetaCutoffsAtFirstMove() const { return betaCutoffsAtFirstMove + qBetaCutoffsAtFirstMove; }

	int32_t betaCutoffsNotAtFirstMove = 0;
	int32_t qBetaCutoffsNotAtFirstMove = 0;
	int32_t totalBetaCutoffsAtNotFirstMove() const { return betaCutoffsNotAtFirstMove + qBetaCutoffsNotAtFirstMove; }

	float betaCutoffsAtFirstMovePercent() const {
		return static_cast<float>(betaCutoffsAtFirstMove) * 100 / (betaCutoffsAtFirstMove + betaCutoffsNotAtFirstMove);
	}

	float qBetaCutoffsAtFirstMovePercent() const {
		return static_cast<float>(qBetaCutoffsAtFirstMove) * 100 / (qBetaCutoffsAtFirstMove + qBetaCutoffsNotAtFirstMove);
	}

	int32_t iidHits = 0;
	int32_t loudMovesGenerated = 0;
	int32_t quietMovesGenerated = 0;
	int32_t extensions = 0;
	int32_t nullMovePrunes = 0;
	int32_t staticNullMovePrunes = 0;
	int32_t futilityPrunes = 0;
	int32_t qSeePrunes = 0;
	int32_t qFutilityPrunes = 0;
	int32_t razorings = 0;
	int32_t razoringsRejected = 0;
	int32_t lmrReductions = 0;

	moves::Moves principalVariation;

	SearchStatistics() :
			evaluationStatistics() {}
};

} //namespace phase4::engine::ai::search

#endif
