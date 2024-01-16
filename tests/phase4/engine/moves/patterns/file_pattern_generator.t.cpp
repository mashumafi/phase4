#include <phase4/engine/moves/patterns/file_pattern_generator.h>

#include <phase4/engine/board/board_constants.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("FilePatternGenerator getPatternForField") {
	using namespace phase4::engine::common;
	using namespace phase4::engine::moves::patterns;

	//                                                              HHHHHHHH GGGGGGGG FFFFFFFF EEEEEEEE DDDDDDDD CCCCCCCC BBBBBBBB AAAAAAAA
	//                                                              87654321 87654321 87654321 87654321 87654321 87654321 87654321 87654321
	CHECK(FilePatternGenerator::getPatternForField(Square::A1) == 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000000ull);
	CHECK(FilePatternGenerator::getPatternForField(Square::A2) == 0b00000010'00000010'00000010'00000010'00000010'00000010'00000010'00000000ull);
	CHECK(FilePatternGenerator::getPatternForField(Square::B2) == 0b00000010'00000010'00000010'00000010'00000010'00000010'00000000'00000010ull);
	CHECK(FilePatternGenerator::getPatternForField(Square::C5) == 0b00010000'00010000'00010000'00010000'00010000'00000000'00010000'00010000ull);
	CHECK(FilePatternGenerator::getPatternForField(Square::D8) == 0b10000000'10000000'10000000'10000000'00000000'10000000'10000000'10000000ull);
	CHECK(FilePatternGenerator::getPatternForField(Square::E1) == 0b00000001'00000001'00000001'00000000'00000001'00000001'00000001'00000001ull);
	CHECK(FilePatternGenerator::getPatternForField(Square::F4) == 0b00001000'00001000'00000000'00001000'00001000'00001000'00001000'00001000ull);
	CHECK(FilePatternGenerator::getPatternForField(Square::G7) == 0b01000000'00000000'01000000'01000000'01000000'01000000'01000000'01000000ull);
	CHECK(FilePatternGenerator::getPatternForField(Square::H2) == 0b00000000'00000010'00000010'00000010'00000010'00000010'00000010'00000010ull);
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
