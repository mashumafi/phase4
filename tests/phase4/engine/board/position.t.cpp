#include <phase4/engine/board/position.h>

#include <phase4/engine/common/piece_color.h>

#include <doctest/doctest.h>

TEST_CASE("Position default") {
	using namespace phase4::engine;

	constexpr board::Position position;

	CHECK(position.m_colorToMove == common::PieceColor::WHITE);
}
