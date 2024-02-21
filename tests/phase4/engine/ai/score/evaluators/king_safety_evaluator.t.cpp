#include <phase4/engine/ai/score/evaluators/king_safety_evaluator.h>

#include <phase4/engine/ai/score/evaluators/mobility_evaluator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>

#include <doctest/doctest.h>

#include <cstdint>

TEST_CASE("KingSafetyEvaluator white ahead 130") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("7r/3kRp2/p2P1R2/1p1b4/8/P2P3P/1Pr3PK/8 b - - 2 31");
	REQUIRE(position);

	common::Bitboard fieldsAttackedByWhite;
	common::Bitboard fieldsAttackedByBlack;
	ai::score::evaluators::MobilityEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0, fieldsAttackedByWhite, fieldsAttackedByBlack);

	const int32_t score = ai::score::evaluators::KingSafetyEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0, fieldsAttackedByWhite, fieldsAttackedByBlack);

	const int32_t blackScore = board::EvaluationConstants::KING_IN_DANGER * 4 + board::EvaluationConstants::PAWN_SHIELD * 0 + board::EvaluationConstants::OPEN_FILE_NEXT_TO_KING * 3;
	const int32_t whiteScore = board::EvaluationConstants::KING_IN_DANGER * 3 + board::EvaluationConstants::PAWN_SHIELD * 2 + board::EvaluationConstants::OPEN_FILE_NEXT_TO_KING * 0;
	CHECK(score == whiteScore - blackScore);
}

TEST_CASE("KingSafetyEvaluator black ahead -130") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("r5k1/pp3pp1/4p3/1Nbp3p/7P/6q1/P1P5/1R1QRK2 w - - 2 25");
	REQUIRE(position);

	common::Bitboard fieldsAttackedByWhite;
	common::Bitboard fieldsAttackedByBlack;
	ai::score::evaluators::MobilityEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0, fieldsAttackedByWhite, fieldsAttackedByBlack);

	const int32_t score = ai::score::evaluators::KingSafetyEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0, fieldsAttackedByWhite, fieldsAttackedByBlack);

	const int32_t blackScore = board::EvaluationConstants::KING_IN_DANGER * 0 + board::EvaluationConstants::PAWN_SHIELD * 2 + board::EvaluationConstants::OPEN_FILE_NEXT_TO_KING * 0;
	const int32_t whiteScore = board::EvaluationConstants::KING_IN_DANGER * 6 + board::EvaluationConstants::PAWN_SHIELD * 0 + board::EvaluationConstants::OPEN_FILE_NEXT_TO_KING * 3;
	CHECK(score == whiteScore - blackScore);
}
