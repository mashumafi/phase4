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

	int depth;
	int selectiveDepth;
	int score;
	uint64_t searchTime;

	uint64_t nodes;
	uint64_t qNodes;
	uint64_t totalNodes() const { return nodes + qNodes; }

	uint64_t leafs;
	uint64_t qLeafs;
	uint64_t totalLeafs() const { return leafs + qLeafs; }

	uint64_t totalNodesPerSecond() const { return totalNodes() / (searchTime / 1000); }
	float secondsPerNode() const { return static_cast<float>(searchTime) / 1000 / nodes; }

	float branchingFactor() const { return static_cast<float>(nodes) / (nodes - leafs); }
	float qBranchingFactor() const { return static_cast<float>(qNodes) / (qNodes - qLeafs); }
	float totalBranchingFactor() const { return static_cast<float>(totalNodes()) / (totalNodes() - totalLeafs()); }

	uint64_t betaCutoffs;
	uint64_t qBetaCutoffs;
	uint64_t totalBetaCutoffs() const { return betaCutoffs + qBetaCutoffs; }

	uint64_t transpositionTableAddedEntries;
	uint64_t transpositionTableReplacements;
	uint64_t transpositionTableHits;
	uint64_t transpositionTableNonHits;
	uint64_t transpositionTableInvalidMoves;
	uint64_t transpositionTableValidMoves;
	float transpositionTableHitsPercent() const { return static_cast<float>(transpositionTableHits) * 100 / (transpositionTableHits + transpositionTableNonHits); }
	float transpositionTableReplacesPercent() const { return static_cast<float>(transpositionTableReplacements) * 100 / transpositionTableAddedEntries; }

	int betaCutoffsAtFirstMove;
	int qBetaCutoffsAtFirstMove;
	int totalBetaCutoffsAtFirstMove() const { return betaCutoffsAtFirstMove + qBetaCutoffsAtFirstMove; }

	int betaCutoffsNotAtFirstMove;
	int qBetaCutoffsNotAtFirstMove;
	int totalBetaCutoffsAtNotFirstMove() const { return betaCutoffsNotAtFirstMove + qBetaCutoffsNotAtFirstMove; }

	float betaCutoffsAtFirstMovePercent() const {
		return static_cast<float>(betaCutoffsAtFirstMove) * 100 / (betaCutoffsAtFirstMove + betaCutoffsNotAtFirstMove);
	}

	float qBetaCutoffsAtFirstMovePercent() const {
		return static_cast<float>(qBetaCutoffsAtFirstMove) * 100 / (qBetaCutoffsAtFirstMove + qBetaCutoffsNotAtFirstMove);
	}

	int32_t iidHits;
	int32_t loudMovesGenerated;
	int32_t quietMovesGenerated;
	int32_t extensions;
	int32_t nullMovePrunes;
	int32_t staticNullMovePrunes;
	int32_t futilityPrunes;
	int32_t qSeePrunes;
	int32_t qFutilityPrunes;
	int32_t razorings;
	int32_t razoringsRejected;
	int32_t lmrReductions;

	moves::Moves principalVariation;

	SearchStatistics() :
			evaluationStatistics() {}
};

} //namespace phase4::engine::ai::search

#endif
