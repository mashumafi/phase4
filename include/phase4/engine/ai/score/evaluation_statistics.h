#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATION_STATISTICS_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATION_STATISTICS_H

#include <cstddef>
#include <cstdint>

namespace phase4::engine::ai::score {

class EvaluationStatistics {
public:
	std::size_t m_pawnHashTableAddedEntries = 0;
	std::size_t m_pawnHashTableReplacements = 0;
	std::size_t m_pawnHashTableHits = 0;
	std::size_t m_pawnHashTableNonHits = 0;

	float pawnHashTableHitsPercent() const {
		return (float)m_pawnHashTableHits * 100 / (m_pawnHashTableHits + m_pawnHashTableNonHits);
	}

	float m_pawnHashTableReplacesPercent() const {
		return (float)m_pawnHashTableReplacements * 100 / m_pawnHashTableAddedEntries;
	}

	std::size_t m_evaluationHashTableAddedEntries = 0;
	std::size_t m_evaluationHashTableReplacements = 0;
	std::size_t m_evaluationHashTableHits = 0;
	std::size_t m_evaluationHashTableNonHits = 0;

	float m_evaluationHashTableHitsPercent() const {
		return (float)m_evaluationHashTableHits * 100 / (m_evaluationHashTableHits + m_evaluationHashTableNonHits);
	}

	float m_evaluationHashTableReplacesPercent() const {
		return (float)m_evaluationHashTableReplacements * 100 / m_evaluationHashTableAddedEntries;
	}
};

} //namespace phase4::engine::ai::score

#endif
