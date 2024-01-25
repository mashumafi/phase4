#include <phase4/engine/board/position.h>

#include <phase4/engine/common/piece_color.h>

#include <doctest/doctest.h>

constexpr phase4::engine::board::Position position;

TEST_CASE("Position constexpr") {
	using namespace phase4::engine;

	CHECK(position.m_colorToMove == common::PieceColor::WHITE);
}
