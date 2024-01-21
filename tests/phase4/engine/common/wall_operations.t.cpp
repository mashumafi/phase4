#include <phase4/engine/common/wall_operations.h>

#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("WallOperations slide from") {
	using namespace phase4::engine::common;

	CHECK(WallOperations::SLIDE_FROM[0][0] == 771);
}

TEST_CASE("WallOperations slide to") {
	using namespace phase4::engine::common;

	CHECK(WallOperations::SLIDE_TO[0][0] == 771);
}

TEST_CASE("WallOperations slide direction") {
	using namespace phase4::engine::common;

	CHECK(WallOperations::SLIDE_DIR[0][0] == FieldIndex(0, 0));
}

TEST_CASE("WallOperations slide square") {
	using namespace phase4::engine::common;

	CHECK(WallOperations::SLIDE_SQUARE[0][0] == Square::A1);
}
