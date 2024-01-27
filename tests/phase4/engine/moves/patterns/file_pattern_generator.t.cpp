#include <phase4/engine/moves/patterns/file_pattern_generator.h>

#include <phase4/engine/board/board_constants.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("FilePatternGenerator getPatternForField") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	//                                                              ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH
	//                                                              88888888 77777777 66666666 55555555 44444444 33333333 22222222 11111111
	CHECK(FilePatternGenerator::getPatternForField(Square::H1) == 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000000);
	CHECK(FilePatternGenerator::getPatternForField(Square::G1) == 0b00000010'00000010'00000010'00000010'00000010'00000010'00000010'00000000);
	CHECK(FilePatternGenerator::getPatternForField(Square::G2) == 0b00000010'00000010'00000010'00000010'00000010'00000010'00000000'00000010);
	CHECK(FilePatternGenerator::getPatternForField(Square::D3) == 0b00010000'00010000'00010000'00010000'00010000'00000000'00010000'00010000);
	CHECK(FilePatternGenerator::getPatternForField(Square::A4) == 0b10000000'10000000'10000000'10000000'00000000'10000000'10000000'10000000);
	CHECK(FilePatternGenerator::getPatternForField(Square::H5) == 0b00000001'00000001'00000001'00000000'00000001'00000001'00000001'00000001);
	CHECK(FilePatternGenerator::getPatternForField(Square::E6) == 0b00001000'00001000'00000000'00001000'00001000'00001000'00001000'00001000);
	CHECK(FilePatternGenerator::getPatternForField(Square::B7) == 0b01000000'00000000'01000000'01000000'01000000'01000000'01000000'01000000);
	CHECK(FilePatternGenerator::getPatternForField(Square::G8) == 0b00000000'00000010'00000010'00000010'00000010'00000010'00000010'00000010);
}

TEST_CASE("FilePatternGenerator getPatternForFile") {
	using namespace phase4::engine::board;
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	CHECK(FilePatternGenerator::getPatternForFile(0) == BoardConstants::FILE_H);
	CHECK(FilePatternGenerator::getPatternForFile(1) == BoardConstants::FILE_G);
	CHECK(FilePatternGenerator::getPatternForFile(2) == BoardConstants::FILE_F);
	CHECK(FilePatternGenerator::getPatternForFile(3) == BoardConstants::FILE_E);
	CHECK(FilePatternGenerator::getPatternForFile(4) == BoardConstants::FILE_D);
	CHECK(FilePatternGenerator::getPatternForFile(5) == BoardConstants::FILE_C);
	CHECK(FilePatternGenerator::getPatternForFile(6) == BoardConstants::FILE_B);
	CHECK(FilePatternGenerator::getPatternForFile(7) == BoardConstants::FILE_A);
}
