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

const phase4::engine::board::ordering::StaticExchangeEvaluation::Array pawnTable = phase4::engine::board::ordering::StaticExchangeEvaluation::populate(phase4::engine::common::PieceType::PAWN);
const phase4::engine::board::ordering::StaticExchangeEvaluation::Array knightTable = phase4::engine::board::ordering::StaticExchangeEvaluation::populate(phase4::engine::common::PieceType::KNIGHT);
const phase4::engine::board::ordering::StaticExchangeEvaluation::Array bishopTable = phase4::engine::board::ordering::StaticExchangeEvaluation::populate(phase4::engine::common::PieceType::BISHOP);
const phase4::engine::board::ordering::StaticExchangeEvaluation::Array rookTable = phase4::engine::board::ordering::StaticExchangeEvaluation::populate(phase4::engine::common::PieceType::ROOK);
const phase4::engine::board::ordering::StaticExchangeEvaluation::Array queenTable = phase4::engine::board::ordering::StaticExchangeEvaluation::populate(phase4::engine::common::PieceType::QUEEN);
const phase4::engine::board::ordering::StaticExchangeEvaluation::Array kingTable = phase4::engine::board::ordering::StaticExchangeEvaluation::populate(phase4::engine::common::PieceType::KING);

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

	return board::ordering::MoveOrdering::seeEvaluate(position, move);
}

} //namespace

TEST_CASE("StaticExchangeEvaluation evaluate") {
	using namespace phase4::engine;
	using SEE = board::ordering::StaticExchangeEvaluation;

	CHECK(SEE::evaluate(common::PieceType::PAWN, common::PieceType::PAWN, 0b01010101, 0b01010101) == 0);
	CHECK(SEE::evaluate(common::PieceType::KNIGHT, common::PieceType::PAWN, 0b01010101, 0b01010101) == -150);
	CHECK(SEE::evaluate(common::PieceType::BISHOP, common::PieceType::PAWN, 0b01010101, 0b01010101) == -170);
	CHECK(SEE::evaluate(common::PieceType::ROOK, common::PieceType::PAWN, 0b01010101, 0b01010101) == -370);
	CHECK(SEE::evaluate(common::PieceType::QUEEN, common::PieceType::PAWN, 0b01010101, 0b01010101) == -990);
	CHECK(SEE::evaluate(common::PieceType::KING, common::PieceType::PAWN, 0b01010101, 0b01010101) == -19800);

	CHECK(SEE::evaluate(common::PieceType::PAWN, common::PieceType::BISHOP, 0b01010101, 0b01010101) == 270);
	CHECK(SEE::evaluate(common::PieceType::KNIGHT, common::PieceType::BISHOP, 0b01010101, 0b01010101) == 120);
	CHECK(SEE::evaluate(common::PieceType::BISHOP, common::PieceType::BISHOP, 0b01010101, 0b01010101) == 100);
	CHECK(SEE::evaluate(common::PieceType::ROOK, common::PieceType::BISHOP, 0b01010101, 0b01010101) == -100);
	CHECK(SEE::evaluate(common::PieceType::QUEEN, common::PieceType::BISHOP, 0b01010101, 0b01010101) == -720);
	CHECK(SEE::evaluate(common::PieceType::KING, common::PieceType::BISHOP, 0b01010101, 0b01010101) == -19530);
}

TEST_CASE("StaticExchangeEvaluation populate") {
	using namespace phase4::engine;

	{
		CHECK(pawnTable[0b01010101][0b01010101] == -100);

		CHECK(pawnTable[0b01010100][0b01010101] == -100);
		CHECK(pawnTable[0b01010101][0b01010100] == 0);

		CHECK(pawnTable[0b00010101][0b01010101] == -100);
		CHECK(pawnTable[0b01010101][0b00010101] == -100);
	}

	{
		CHECK(knightTable[0b01010101][0b01010101] == -250);

		CHECK(knightTable[0b01010100][0b01010101] == -350);
		CHECK(knightTable[0b01010101][0b01010100] == 0);

		CHECK(knightTable[0b00010101][0b01010101] == -250);
		CHECK(knightTable[0b01010101][0b00010101] == -250);
	}

	{
		CHECK(bishopTable[0b01010101][0b01010101] == -270);

		CHECK(bishopTable[0b01010100][0b01010101] == -370);
		CHECK(bishopTable[0b01010101][0b01010100] == -20);

		CHECK(bishopTable[0b00010101][0b01010101] == -270);
		CHECK(bishopTable[0b01010101][0b00010101] == -270);
	}

	{
		CHECK(rookTable[0b01010101][0b01010101] == -470);

		CHECK(rookTable[0b01010100][0b01010101] == -570);
		CHECK(rookTable[0b01010101][0b01010100] == -220);

		CHECK(rookTable[0b00010101][0b01010101] == -470);
		CHECK(rookTable[0b01010101][0b00010101] == -470);
	}

	{
		CHECK(queenTable[0b01010101][0b01010101] == -1090);

		CHECK(queenTable[0b01010100][0b01010101] == -1190);
		CHECK(queenTable[0b01010101][0b01010100] == -840);

		CHECK(queenTable[0b00010101][0b01010101] == -1090);
		CHECK(queenTable[0b01010101][0b00010101] == -1090);
	}

	{
		CHECK(kingTable[0b01010101][0b01010101] == -19900);

		CHECK(kingTable[0b01010100][0b01010101] == -20000);
		CHECK(kingTable[0b01010101][0b01010100] == -19650);

		CHECK(kingTable[0b00010101][0b01010101] == -19900);
		CHECK(kingTable[0b01010101][0b00010101] == -19900);
	}
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
