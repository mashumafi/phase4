#include <phase4/engine/board/position.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/operators.h>
#include <phase4/engine/board/position_moves.h>
#include <phase4/engine/board/position_state.h>

#include <phase4/engine/moves/move.h>

#include <phase4/engine/common/game_phase.h>
#include <phase4/engine/common/math.h>
#include <phase4/engine/common/piece_color.h>

#include <doctest/doctest.h>

#include <cstdint>
#include <optional>
#include <unordered_set>

TEST_CASE("Position default") {
	using namespace phase4::engine;

	constexpr board::Position position;

	CHECK(position.colorToMove() == common::PieceColor::WHITE);
}

TEST_CASE("Position opening phase") {
	using namespace phase4::engine;

	board::Position position = board::PositionState::DEFAULT;

	CHECK(position.material(common::PieceColor::WHITE) == position.material(common::PieceColor::BLACK));
	CHECK(position.getPhaseRatio() == common::PositionConstants::PHASE_RESOLUTION);
	CHECK(position.getGamePhase() == common::GamePhase::OPENING);
}

TEST_CASE("Position ending phase") {
	using namespace phase4::engine;

	std::optional<board::Position> position = fen::FenToPosition::parse("8/7p/1p5k/4p3/4P3/P5PP/P2K4/8 b - - 0 33");
	REQUIRE(position);

	CHECK(position->material(common::PieceColor::WHITE) > position->material(common::PieceColor::BLACK));
	CHECK(common::Math::min_int32(position->material(common::PieceColor::WHITE), position->material(common::PieceColor::BLACK)) == position->material(common::PieceColor::BLACK));
	CHECK(position->getPhaseRatio() == 0);
	CHECK(position->getGamePhase() == common::GamePhase::ENDING);
}

class ZobristValidator {
	std::unordered_set<uint64_t> hashes;

public:
	void validate(const phase4::engine::board::Position &positionBegin, int32_t ply = 2) {
		using namespace phase4::engine;

		if (ply == 0) {
			return;
		}

		CHECK(hashes.insert(positionBegin.hash().asBitboard().get_raw_value()).second);

		moves::Moves moves;
		board::Operators::getAllMoves(positionBegin, moves);

		for (size_t i = 0; i < moves.size(); ++i) {
			board::Position position = positionBegin;
			const bool isPawnHash = position.pieceTable(moves[i].from()) == common::PieceType::PAWN || position.pieceTable(moves[i].to()) == common::PieceType::PAWN;
			board::PositionMoves::makeMove(position, moves[i]);
			if (isPawnHash) {
				CHECK(positionBegin.pawnHash() != position.pawnHash());
			} else {
				CHECK(positionBegin.pawnHash() == position.pawnHash());
			}
			CHECK(positionBegin.hash() != position.hash());
			CHECK(positionBegin.hash() != position.hash().changeSide());

			validate(position, ply - 1);
		}
	}

	size_t checkCount() const {
		return hashes.size();
	}
};

TEST_CASE("Position Zobrist hash") {
	using namespace phase4::engine;

	ZobristValidator validator;
	validator.validate(board::PositionState::DEFAULT);

	const auto longCastlingPosition = fen::FenToPosition::parse("r3kbnr/pbp3pp/1pnqP3/8/8/1PN5/PBPQ1PPP/R3KBNR b KQkq - 0 9");
	REQUIRE(longCastlingPosition);
	validator.validate(*longCastlingPosition);

	const auto shortCastlingPosition = fen::FenToPosition::parse("rnbqk2r/ppp2pbp/4P1p1/8/6n1/5N2/PP1PPPBP/RNBQK2R b KQkq - 0 7");
	REQUIRE(shortCastlingPosition);
	validator.validate(*shortCastlingPosition);

	const auto promotionCapturesPosition = fen::FenToPosition::parse("2kr1b1r/pbp1P1p1/1pn2n2/1P1N4/2P5/8/PB3PpP/R1K2BNR b - - 0 16");
	REQUIRE(promotionCapturesPosition);
	validator.validate(*promotionCapturesPosition);

	const auto promotionPosition = fen::FenToPosition::parse("2k4r/pbp1P1b1/1pn2n2/1P1r3P/2P3pR/3B4/PB3Pp1/R1K3N1 b - - 2 20");
	REQUIRE(promotionPosition);
	validator.validate(*promotionPosition);

	CHECK(validator.checkCount() == 197);
}
