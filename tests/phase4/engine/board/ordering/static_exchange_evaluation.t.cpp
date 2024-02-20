#include <phase4/engine/board/ordering/static_exchange_evaluation.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/ordering/move_ordering.h>
#include <phase4/engine/board/ordering/move_ordering_constants.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>

#include <phase4/engine/moves/move.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>

#include <doctest/doctest.h>

#include <cstdint>

namespace {

constexpr int32_t P = phase4::engine::board::EvaluationConstants::pieceValue(phase4::engine::common::PieceType::PAWN);
constexpr int32_t N = phase4::engine::board::EvaluationConstants::pieceValue(phase4::engine::common::PieceType::KNIGHT);
constexpr int32_t B = phase4::engine::board::EvaluationConstants::pieceValue(phase4::engine::common::PieceType::BISHOP);
constexpr int32_t R = phase4::engine::board::EvaluationConstants::pieceValue(phase4::engine::common::PieceType::ROOK);
constexpr int32_t Q = phase4::engine::board::EvaluationConstants::pieceValue(phase4::engine::common::PieceType::QUEEN);
constexpr int32_t K = phase4::engine::board::EvaluationConstants::pieceValue(phase4::engine::common::PieceType::KING);

int32_t evaluate(const phase4::engine::board::Position &position, phase4::engine::moves::Move move) {
	using namespace phase4::engine;

	const std::optional<moves::Move> &realMove = board::PositionMoves::findRealMove(position, move);
	REQUIRE(realMove);
	REQUIRE(realMove->flags() == moves::MoveFlags::CAPTURE); // Only testing captures

	const common::PieceColor enemyColor = position.m_colorToMove.invert();

	const common::PieceType attackingPiece = position.m_pieceTable[realMove->from()];
	const common::PieceType capturedPiece = position.m_pieceTable[realMove->to()];

	const uint8_t attackers = board::ordering::SeePiece::getAttackingPiecesWithColor(position, position.m_colorToMove, realMove->to());
	const uint8_t defenders = board::ordering::SeePiece::getAttackingPiecesWithColor(position, enemyColor, realMove->to());
	return board::ordering::StaticExchangeEvaluation::evaluate(attackingPiece, capturedPiece, attackers, defenders);
}
} //namespace

TEST_CASE("StaticExchangeEvaluation computeResult") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("r2q1rk1/1b3pb1/1p2p1pB/p2p4/n7/5N1P/BPPQ1PP1/2KR3R w - - 0 19");
	REQUIRE(position);

	const std::optional<moves::Move> &realMove = board::PositionMoves::findRealMove(*position, moves::Move("d2d5"));
	CHECK(realMove);
	CHECK(realMove->flags() == moves::MoveFlags::CAPTURE); // Only testing captures

	const common::PieceColor enemyColor = position->m_colorToMove.invert();

	const common::PieceType attackingPiece = position->m_pieceTable[realMove->from()];
	CHECK(attackingPiece == common::PieceType::QUEEN);
	const common::PieceType capturedPiece = position->m_pieceTable[realMove->to()];
	CHECK(capturedPiece == common::PieceType::PAWN);

	common::Bitset attackers = board::ordering::SeePiece::getAttackingPiecesWithColor(*position, position->m_colorToMove, realMove->to());
	common::Bitset defenders = board::ordering::SeePiece::getAttackingPiecesWithColor(*position, enemyColor, realMove->to());

	CHECK(attackers == 0b01011000);
	CHECK(defenders == 0b01001001);

	attackers &= 0b10111111; // Remove queen as an attacker

	common::SafeVector<int16_t, 7> gainList = {};

	common::PieceType currentPieceOnField = attackingPiece;
	int16_t result = 0;

	gainList.push_back(result);

	{
		REQUIRE(defenders != 0);
		common::PieceType leastValuableDefenderPiece = board::ordering::StaticExchangeEvaluation::getLeastValuablePiece(defenders);
		CHECK(leastValuableDefenderPiece == common::PieceType::PAWN);
		defenders = defenders.popLsb();

		result -= board::EvaluationConstants::pieceValue(currentPieceOnField);
		currentPieceOnField = leastValuableDefenderPiece;

		gainList.push_back(result);
	}

	// while
	{
		REQUIRE(attackers != 0);
		const common::PieceType leastValuableAttackerPiece = board::ordering::StaticExchangeEvaluation::getLeastValuablePiece(attackers);
		CHECK(leastValuableAttackerPiece == common::PieceType::BISHOP);
		attackers = attackers.popLsb();

		result += board::EvaluationConstants::pieceValue(currentPieceOnField);
		currentPieceOnField = leastValuableAttackerPiece;

		gainList.push_back(result);

		CHECK(gainList.peek() <= gainList.peek(2));
	}

	{
		REQUIRE(defenders != 0);

		const common::PieceType leastValuableDefenderPiece = board::ordering::StaticExchangeEvaluation::getLeastValuablePiece(defenders);
		CHECK(leastValuableDefenderPiece == common::PieceType::BISHOP);
		defenders = defenders.popLsb();

		result -= board::EvaluationConstants::pieceValue(currentPieceOnField);
		currentPieceOnField = leastValuableDefenderPiece;

		gainList.push_back(result);

		CHECK(gainList.peek() < gainList.peek(2));
		result = gainList.peek(2);
	}

	CHECK(attackers == 0b00010000); // White Rook
	CHECK(defenders == 0b01000000); // Black Queen

	result += board::EvaluationConstants::pieceValue(capturedPiece);

	const int32_t blackSee = Q;
	const int32_t whiteSee = P;
	CHECK(result == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation evaluate") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("8/pp3pk1/2p3Pr/4q3/8/6RP/PPQ2PRK/4r3 b - - 0 26");
	REQUIRE(position);
	CHECK(position->m_colorToMove == common::PieceColor::BLACK);

	const std::optional<moves::Move> &realMove = board::PositionMoves::findRealMove(*position, moves::Move("g7g6"));
	CHECK(realMove);
	CHECK(realMove->flags() == moves::MoveFlags::CAPTURE); // Only testing captures

	const common::PieceColor enemyColor = position->m_colorToMove.invert();
	CHECK(enemyColor == common::PieceColor::WHITE);

	const common::PieceType attackingPiece = position->m_pieceTable[realMove->from()];
	const common::PieceType capturedPiece = position->m_pieceTable[realMove->to()];
	CHECK(attackingPiece == common::PieceType::KING);
	CHECK(capturedPiece == common::PieceType::PAWN);

	const uint8_t attackers = board::ordering::SeePiece::getAttackingPiecesWithColor(*position, position->m_colorToMove, realMove->to());
	const uint8_t defenders = board::ordering::SeePiece::getAttackingPiecesWithColor(*position, enemyColor, realMove->to());
	CHECK(attackers == 0b10010001);
	CHECK(defenders == 0b1110000);
	const int32_t seeEvaluation = board::ordering::StaticExchangeEvaluation::evaluate(attackingPiece, capturedPiece, attackers, defenders);

	const int32_t blackSee = K;
	const int32_t whiteSee = R + P;
	CHECK(seeEvaluation == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation -19330 g7g6 8/pp3pk1/2p3Pr/4q3/8/6RP/PPQ2PRK/4r3 b - - 0 26") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("8/pp3pk1/2p3Pr/4q3/8/6RP/PPQ2PRK/4r3 b - - 0 26");
	REQUIRE(position);

	const int32_t seeEvaluation = evaluate(*position, moves::Move("g7g6"));
	const int32_t blackSee = K;
	const int32_t whiteSee = R + P;
	CHECK(seeEvaluation == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation -1090 d2d5 r2q1rk1/1b3pb1/1p2p1pB/p2p4/n7/5N1P/BPPQ1PP1/2KR3R w - - 0 19") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("r2q1rk1/1b3pb1/1p2p1pB/p2p4/n7/5N1P/BPPQ1PP1/2KR3R w - - 0 19");
	REQUIRE(position);

	const int32_t seeEvaluation = evaluate(*position, moves::Move("d2d5"));
	const int32_t blackSee = Q;
	const int32_t whiteSee = P;
	CHECK(seeEvaluation == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation -840 e5g3 5rk1/p3p2p/1p1p2p1/2pPq2n/4Q3/P1P1BrNP/1P3P1K/R5R1 b - - 7 22") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("5rk1/p3p2p/1p1p2p1/2pPq2n/4Q3/P1P1BrNP/1P3P1K/R5R1 b - - 7 22");
	REQUIRE(position);

	const int32_t seeEvaluation = evaluate(*position, moves::Move("e5g3"));
	const int32_t blackSee = Q;
	const int32_t whiteSee = N;
	CHECK(seeEvaluation == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation -470 f4f3 4r3/p6k/1p1p2p1/1P1Rn2p/4Pr2/1BQ2PqP/PP2KRP1/8 b - - 1 35") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("4r3/p6k/1p1p2p1/1P1Rn2p/4Pr2/1BQ2PqP/PP2KRP1/8 b - - 1 35");
	REQUIRE(position);

	const int32_t seeEvaluation = evaluate(*position, moves::Move("f4f3"));
	const int32_t blackSee = R;
	const int32_t whiteSee = P;
	CHECK(seeEvaluation == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation -250 g5e6 r1b1rn1k/pp2bp1B/2p1p2Q/q5N1/2PP2n1/2N1R3/PP3PPP/2KR4 w - - 6 17") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("r1b1rn1k/pp2bp1B/2p1p2Q/q5N1/2PP2n1/2N1R3/PP3PPP/2KR4 w - - 6 17");
	REQUIRE(position);

	const int32_t seeEvaluation = evaluate(*position, moves::Move("g5e6"));
	const int32_t blackSee = N;
	const int32_t whiteSee = P;
	CHECK(seeEvaluation == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation 150 d5d3 5rk1/1p2bppp/pq6/3rPb2/1n3P2/3N1R2/PP1BB1PP/RQ5K b - - 3 20") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("5rk1/1p2bppp/pq6/3rPb2/1n3P2/3N1R2/PP1BB1PP/RQ5K b - - 3 20");
	REQUIRE(position);

	const int32_t seeEvaluation = evaluate(*position, moves::Move("d5d3"));
	const int32_t blackSee = R;
	const int32_t whiteSee = N + B;
	CHECK(seeEvaluation == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation 350 f5d3 5rk1/1p2bppp/pq6/3rPb2/1n3P2/3N1R2/PP1BB1PP/RQ5K b - - 3 20") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("5rk1/1p2bppp/pq6/3rPb2/1n3P2/3N1R2/PP1BB1PP/RQ5K b - - 3 20");
	REQUIRE(position);

	const int32_t seeEvaluation = evaluate(*position, moves::Move("f5d3"));
	const int32_t blackSee = 0;
	const int32_t whiteSee = N;
	CHECK(seeEvaluation == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation 570 e6e8 r2qrn2/pp3kp1/2n1Rp2/3p4/2pP1P2/P1P4P/2PBQ1P1/4R1K1 w - - 1 20") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("r2qrn2/pp3kp1/2n1Rp2/3p4/2pP1P2/P1P4P/2PBQ1P1/4R1K1 w - - 1 20");
	REQUIRE(position);

	const int32_t seeEvaluation = evaluate(*position, moves::Move("e6e8"));
	const int32_t blackSee = 0;
	const int32_t whiteSee = R;
	CHECK(seeEvaluation == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation 840 e7g8 r5Qq/kbp1np1P/p1p1pN2/2P2p2/3P4/8/PP3P1P/R5RK b - - 4 30") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("r5Qq/kbp1np1P/p1p1pN2/2P2p2/3P4/8/PP3P1P/R5RK b - - 4 30");
	REQUIRE(position);

	const int32_t seeEvaluation = evaluate(*position, moves::Move("e7g8"));
	const int32_t blackSee = N;
	const int32_t whiteSee = Q;
	CHECK(seeEvaluation == whiteSee - blackSee);
}

TEST_CASE("StaticExchangeEvaluation 970 f7c7 2kr3r/Q1q2Rp1/Np5p/p2np3/P7/4P2P/6PK/2q5 w - - 2 33") {
	using namespace phase4::engine;

	const auto position = fen::FenToPosition::parse("2kr3r/Q1q2Rp1/Np5p/p2np3/P7/4P2P/6PK/2q5 w - - 2 33");
	REQUIRE(position);

	const int32_t seeEvaluation = evaluate(*position, moves::Move("f7c7"));
	const int32_t blackSee = R;
	const int32_t whiteSee = Q + N;
	CHECK(seeEvaluation == whiteSee - blackSee);
}
