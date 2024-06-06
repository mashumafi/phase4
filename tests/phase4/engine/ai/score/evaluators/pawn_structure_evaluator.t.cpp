#include <phase4/engine/ai/score/evaluators/pawn_structure_evaluator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/ai/score/evaluation_statistics.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/session.h>

#include <doctest/doctest.h>

#include <cstdint>
#include <memory>

TEST_CASE("PawnStructureEvaluators Good structure black") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("8/2p5/pp1k3p/3P1p1P/P1PK1Pp1/8/5P2/8 b - - 5 40");
	REQUIRE(position);

	auto session = std::make_unique<board::Session>(*position);

	ai::score::EvaluationStatistics statistics;
	const int32_t openingScore = ai::score::evaluators::PawnStructureEvaluator::evaluate(*session, statistics, common::PositionConstants::PHASE_RESOLUTION, 0);

	const int32_t whiteDoubledPawnsOpeningScore = 1 * board::EvaluationConstants::DOUBLED_PAWNS[common::GamePhase::OPENING];
	const int32_t whiteDoubledPawnsEndingScore = 1 * board::EvaluationConstants::DOUBLED_PAWNS[common::GamePhase::ENDING];
	const int32_t whiteIsolatedPawnsOpeningScore = 3 * board::EvaluationConstants::ISOLATED_PAWNS[common::GamePhase::OPENING];
	const int32_t whiteIsolatedPawnsEndingScore = 3 * board::EvaluationConstants::ISOLATED_PAWNS[common::GamePhase::ENDING];
	const int32_t whiteChainedPawnsOpeningScore = 2 * board::EvaluationConstants::CHAINED_PAWNS[common::GamePhase::OPENING];
	const int32_t whiteChainedPawnsEndingScore = 2 * board::EvaluationConstants::CHAINED_PAWNS[common::GamePhase::ENDING];

	const int32_t blackChainedPawnsOpeningScore = 4 * board::EvaluationConstants::CHAINED_PAWNS[common::GamePhase::OPENING];
	const int32_t blackChainedPawnsEndingScore = 4 * board::EvaluationConstants::CHAINED_PAWNS[common::GamePhase::ENDING];

	const int32_t blackOpeningScore = blackChainedPawnsOpeningScore;
	const int32_t whiteOpeningScore = whiteDoubledPawnsOpeningScore + whiteIsolatedPawnsOpeningScore + whiteChainedPawnsOpeningScore;
	CHECK(openingScore == whiteOpeningScore - blackOpeningScore);

	CHECK(openingScore == ai::score::evaluators::PawnStructureEvaluator::evaluateWithoutCache(session->position(), common::PositionConstants::PHASE_RESOLUTION, 0));

	const int32_t blackEndingScore = blackChainedPawnsEndingScore;
	const int32_t whiteEndingScore = whiteDoubledPawnsEndingScore + whiteIsolatedPawnsEndingScore + whiteChainedPawnsEndingScore;

	const auto &entry = session->m_hashTables.m_pawnHashTable.get(session->position().pawnHash().asBitboard());
	CHECK(entry.isKeyValid(session->position().pawnHash().asBitboard()));
	CHECK(entry.openingScore() == whiteOpeningScore - blackOpeningScore);
	CHECK(entry.endingScore() == whiteEndingScore - blackEndingScore);

	CHECK(entry.endingScore() == ai::score::evaluators::PawnStructureEvaluator::evaluateWithoutCache(session->position(), 0, common::PositionConstants::PHASE_RESOLUTION));

	ai::score::evaluators::PawnStructureEvaluator::evaluate(*session, statistics, common::PositionConstants::PHASE_RESOLUTION, 0);

#ifndef NDEBUG
	CHECK(statistics.m_pawnHashTableAddedEntries == 1);
	CHECK(statistics.m_pawnHashTableHits == 1);
	CHECK(statistics.m_pawnHashTableNonHits == 1);
	CHECK(statistics.m_pawnHashTableReplacements == 0);
#endif
}

TEST_CASE("PawnStructureEvaluators Good structure white") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("r4rk1/R4p2/Q4p1p/6q1/3p4/3BP1Pb/P4P1P/4R1K1 b - - 0 21");
	REQUIRE(position);

	auto session = std::make_unique<board::Session>(*position);

	ai::score::EvaluationStatistics statistics;
	const int32_t openingScore = ai::score::evaluators::PawnStructureEvaluator::evaluate(*session, statistics, common::PositionConstants::PHASE_RESOLUTION, 0);

	const int32_t whiteIsolatedPawnsOpeningScore = 1 * board::EvaluationConstants::ISOLATED_PAWNS[common::GamePhase::OPENING];
	const int32_t whiteIsolatedPawnsEndingScore = 1 * board::EvaluationConstants::ISOLATED_PAWNS[common::GamePhase::ENDING];
	const int32_t whiteChainedPawnsOpeningScore = 6 * board::EvaluationConstants::CHAINED_PAWNS[common::GamePhase::OPENING];
	const int32_t whiteChainedPawnsEndingScore = 6 * board::EvaluationConstants::CHAINED_PAWNS[common::GamePhase::ENDING];
	const int32_t whitePassingPawnsOpeningScore = 1 * board::EvaluationConstants::PASSING_PAWNS[common::GamePhase::OPENING];
	const int32_t whitePassingPawnsEndingScore = 1 * board::EvaluationConstants::PASSING_PAWNS[common::GamePhase::ENDING];

	const int32_t blackDoubledPawnsOpeningScore = 1 * board::EvaluationConstants::DOUBLED_PAWNS[common::GamePhase::OPENING];
	const int32_t blackDoubledPawnsEndingScore = 1 * board::EvaluationConstants::DOUBLED_PAWNS[common::GamePhase::ENDING];
	const int32_t blackIsolatedPawnsOpeningScore = 3 * board::EvaluationConstants::ISOLATED_PAWNS[common::GamePhase::OPENING];
	const int32_t blackIsolatedPawnsEndingScore = 3 * board::EvaluationConstants::ISOLATED_PAWNS[common::GamePhase::ENDING];

	const int32_t blackOpeningScore = blackDoubledPawnsOpeningScore + blackIsolatedPawnsOpeningScore;
	const int32_t whiteOpeningScore = whiteIsolatedPawnsOpeningScore + whiteChainedPawnsOpeningScore + whitePassingPawnsOpeningScore;
	CHECK(openingScore == whiteOpeningScore - blackOpeningScore);

	CHECK(openingScore == ai::score::evaluators::PawnStructureEvaluator::evaluateWithoutCache(session->position(), common::PositionConstants::PHASE_RESOLUTION, 0));

	const int32_t blackEndingScore = blackDoubledPawnsEndingScore + blackIsolatedPawnsEndingScore;
	const int32_t whiteEndingScore = whiteIsolatedPawnsEndingScore + whiteChainedPawnsEndingScore + whitePassingPawnsEndingScore;

	const auto &entry = session->m_hashTables.m_pawnHashTable.get(session->position().pawnHash().asBitboard());
	CHECK(entry.isKeyValid(session->position().pawnHash().asBitboard()));
	CHECK(entry.openingScore() == whiteOpeningScore - blackOpeningScore);
	CHECK(entry.endingScore() == whiteEndingScore - blackEndingScore);

	CHECK(entry.endingScore() == ai::score::evaluators::PawnStructureEvaluator::evaluateWithoutCache(session->position(), 0, common::PositionConstants::PHASE_RESOLUTION));

	ai::score::evaluators::PawnStructureEvaluator::evaluate(*session, statistics, common::PositionConstants::PHASE_RESOLUTION, 0);

#ifndef NDEBUG
	CHECK(statistics.m_pawnHashTableAddedEntries == 1);
	CHECK(statistics.m_pawnHashTableHits == 1);
	CHECK(statistics.m_pawnHashTableNonHits == 1);
	CHECK(statistics.m_pawnHashTableReplacements == 0);
#endif
}
