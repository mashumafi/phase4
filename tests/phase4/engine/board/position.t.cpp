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

	CHECK(position.m_colorToMove == common::PieceColor::WHITE);
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

		CHECK(hashes.insert(positionBegin.m_hash.asBitboard().get_raw_value()).second);

		moves::Moves moves;
		board::Operators::getAllMoves(positionBegin, moves);

		for (size_t i = 0; i < moves.size(); ++i) {
			board::Position position = positionBegin;
			const bool isPawnHash = position.m_pieceTable[moves[i].from()] == common::PieceType::PAWN || position.m_pieceTable[moves[i].to()] == common::PieceType::PAWN;
			board::PositionMoves::makeMove(position, moves[i]);
			if (isPawnHash) {
				CHECK(positionBegin.m_pawnHash != position.m_pawnHash);
			} else {
				CHECK(positionBegin.m_pawnHash == position.m_pawnHash);
			}
			CHECK(positionBegin.m_hash != position.m_hash);
			CHECK(positionBegin.m_hash != position.m_hash.changeSide());

			validate(position, ply - 1);
		}
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
}
