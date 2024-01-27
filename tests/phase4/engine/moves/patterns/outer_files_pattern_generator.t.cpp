#include <phase4/engine/moves/patterns/outer_files_pattern_generator.h>

#include <phase4/engine/board/board_constants.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("OuterFilesPatternGenerator getPatternForFile") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	CHECK(OuterFilesPatternGenerator::getPatternForFile(0) == (BoardConstants::FILE_G));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(1) == (BoardConstants::FILE_H | BoardConstants::FILE_F));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(2) == (BoardConstants::FILE_G | BoardConstants::FILE_E));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(3) == (BoardConstants::FILE_F | BoardConstants::FILE_D));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(4) == (BoardConstants::FILE_E | BoardConstants::FILE_C));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(5) == (BoardConstants::FILE_D | BoardConstants::FILE_B));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(6) == (BoardConstants::FILE_C | BoardConstants::FILE_A));
	CHECK(OuterFilesPatternGenerator::getPatternForFile(7) == (BoardConstants::FILE_B));
}
