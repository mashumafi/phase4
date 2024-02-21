#include <phase4/engine/ai/score/evaluators/mobility_evaluator.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/position.h>

#include <doctest/doctest.h>

#include <cstdint>

TEST_CASE("MobilityEvaluator white ahead 130") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("rn1q1rk1/pp2pnbp/2p3p1/3p4/5B2/1BN3QP/PPP2PP1/3RR1K1 b - - 3 15");
	REQUIRE(position);

	common::Bitboard fieldsAttackedByWhite;
	common::Bitboard fieldsAttackedByBlack;
	const int32_t score = ai::score::evaluators::MobilityEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0, fieldsAttackedByWhite, fieldsAttackedByBlack);

	CHECK(fieldsAttackedByWhite == 0b01000000'00101000'00011011'01011010'10111111'00111111'10111111'11111110);
	CHECK(fieldsAttackedByBlack == 0b01111111'10111100'11110101'10011010'00010000'00100000'00000000'00000000);

	const int32_t blackScore = board::EvaluationConstants::CENTER_MOBILITY_MODIFIER * 9 + board::EvaluationConstants::OUTSIDE_MOBILITY_MODIFIER * 24;
	const int32_t whiteScore = board::EvaluationConstants::CENTER_MOBILITY_MODIFIER * 19 + board::EvaluationConstants::OUTSIDE_MOBILITY_MODIFIER * 34;
	CHECK(score == whiteScore - blackScore);
}

TEST_CASE("MobilityEvaluator black ahead -164") {
	using namespace phase4::engine;

	auto position = fen::FenToPosition::parse("2k2b1q/1bpn1Q2/1p6/p1n2P2/8/1P6/P1PN2r1/1K1R4 b - - 0 31");
	REQUIRE(position);

	common::Bitboard fieldsAttackedByWhite;
	common::Bitboard fieldsAttackedByBlack;
	const int32_t score = ai::score::evaluators::MobilityEvaluator::evaluate(*position, common::PositionConstants::PHASE_RESOLUTION, 0, fieldsAttackedByWhite, fieldsAttackedByBlack);

	CHECK(fieldsAttackedByWhite == 0b00001110'00011011'00001110'00010101'00101000'01000100'00010000'01101111);
	CHECK(fieldsAttackedByBlack == 0b11100110'01011011'11111111'00111011'10011011'01110111'01011111'10000011);

	const int32_t blackScore = board::EvaluationConstants::CENTER_MOBILITY_MODIFIER * 16 + board::EvaluationConstants::OUTSIDE_MOBILITY_MODIFIER * 38;
	const int32_t whiteScore = board::EvaluationConstants::CENTER_MOBILITY_MODIFIER * 8 + board::EvaluationConstants::OUTSIDE_MOBILITY_MODIFIER * 20;
	CHECK(score == whiteScore - blackScore);
}
