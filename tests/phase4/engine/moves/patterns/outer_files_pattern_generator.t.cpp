#include <phase4/engine/moves/patterns/outer_files_pattern_generator.h>

#include <phase4/engine/common/position_constants.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("OuterFilesPatternGenerator getPatternForFile") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	CHECK(OuterFilesPatternGenerator::getPatternForFile(0) == (PositionConstants::FILE_G));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(1) == (PositionConstants::FILE_H | PositionConstants::FILE_F));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(2) == (PositionConstants::FILE_G | PositionConstants::FILE_E));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(3) == (PositionConstants::FILE_F | PositionConstants::FILE_D));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(4) == (PositionConstants::FILE_E | PositionConstants::FILE_C));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(5) == (PositionConstants::FILE_D | PositionConstants::FILE_B));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(6) == (PositionConstants::FILE_C | PositionConstants::FILE_A));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(7) == (PositionConstants::FILE_B));
}
