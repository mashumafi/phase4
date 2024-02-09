#include <phase4/engine/board/position.h>

#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position_state.h>

#include <phase4/engine/common/game_phase.h>
#include <phase4/engine/common/math.h>
#include <phase4/engine/common/piece_color.h>

#include <doctest/doctest.h>

#include <optional>

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
