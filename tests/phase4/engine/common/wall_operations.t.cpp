#include <phase4/engine/common/wall_operations.h>

#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("WallOperations slide from") {
	using namespace phase4::engine::common;

	CHECK(WallOperations::SLIDE_FROM[Square::H1] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000011'00000011);
	CHECK(WallOperations::SLIDE_FROM[Square::G1] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000011'00000011);
	CHECK(WallOperations::SLIDE_FROM[Square::F1] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00001100'00001100);
	CHECK(WallOperations::SLIDE_FROM[Square::D1] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00110000'00110000);
	CHECK(WallOperations::SLIDE_FROM[Square::B1] == 0b00000000'00000000'00000000'00000000'00000000'00000000'11000000'11000000);

	CHECK(WallOperations::SLIDE_FROM[Square::H2] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000011'00000011);
	CHECK(WallOperations::SLIDE_FROM[Square::G2] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000011'00000011);
	CHECK(WallOperations::SLIDE_FROM[Square::E2] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00001100'00001100);
	CHECK(WallOperations::SLIDE_FROM[Square::C2] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00110000'00110000);
	CHECK(WallOperations::SLIDE_FROM[Square::A2] == 0b00000000'00000000'00000000'00000000'00000000'00000000'11000000'11000000);

	CHECK(WallOperations::SLIDE_FROM[Square::G3] == 0b00000000'00000000'00000000'00000000'00000011'00000011'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::E3] == 0b00000000'00000000'00000000'00000000'00001100'00001100'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::C3] == 0b00000000'00000000'00000000'00000000'00110000'00110000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::A3] == 0b00000000'00000000'00000000'00000000'11000000'11000000'00000000'00000000);

	CHECK(WallOperations::SLIDE_FROM[Square::H4] == 0b00000000'00000000'00000000'00000000'00000011'00000011'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::F4] == 0b00000000'00000000'00000000'00000000'00001100'00001100'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::D4] == 0b00000000'00000000'00000000'00000000'00110000'00110000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::B4] == 0b00000000'00000000'00000000'00000000'11000000'11000000'00000000'00000000);

	CHECK(WallOperations::SLIDE_FROM[Square::G5] == 0b00000000'00000000'00000011'00000011'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::E5] == 0b00000000'00000000'00001100'00001100'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::C5] == 0b00000000'00000000'00110000'00110000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::A5] == 0b00000000'00000000'11000000'11000000'00000000'00000000'00000000'00000000);

	CHECK(WallOperations::SLIDE_FROM[Square::H6] == 0b00000000'00000000'00000011'00000011'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::F6] == 0b00000000'00000000'00001100'00001100'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::D6] == 0b00000000'00000000'00110000'00110000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::B6] == 0b00000000'00000000'11000000'11000000'00000000'00000000'00000000'00000000);

	CHECK(WallOperations::SLIDE_FROM[Square::G7] == 0b00000011'00000011'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::E7] == 0b00001100'00001100'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::C7] == 0b00110000'00110000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::A7] == 0b11000000'11000000'00000000'00000000'00000000'00000000'00000000'00000000);

	CHECK(WallOperations::SLIDE_FROM[Square::H8] == 0b00000011'00000011'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::F8] == 0b00001100'00001100'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::D8] == 0b00110000'00110000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_FROM[Square::B8] == 0b11000000'11000000'00000000'00000000'00000000'00000000'00000000'00000000);
}

TEST_CASE("WallOperations slide to") {
	using namespace phase4::engine::common;

	WallOperations::populateSlideToBB();

	//                                                                   ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH
	//                             WALLS       LANDING                   88888888 77777777 66666666 55555555 44444444 33333333 22222222 11111111
	CHECK(WallOperations::SLIDE_TO[Square::H1][Square::H1.north(2)] == 0b00000000'00000000'00000000'00000000'00000011'00000011'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::G1][Square::G1.west(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00001100'00001100);
	CHECK(WallOperations::SLIDE_TO[Square::F1][Square::F1.east(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000011'00000011);
	CHECK(WallOperations::SLIDE_TO[Square::E1][Square::E1.north(2)] == 0b00000000'00000000'00000000'00000000'00001100'00001100'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::D1][Square::D1.east(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00001100'00001100);
	CHECK(WallOperations::SLIDE_TO[Square::C1][Square::C1.west(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'11000000'11000000);
	CHECK(WallOperations::SLIDE_TO[Square::B1][Square::B1.north(2)] == 0b00000000'00000000'00000000'00000000'11000000'11000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::A1][Square::A1.east(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00110000'00110000);

	CHECK(WallOperations::SLIDE_TO[Square::H2][Square::H2.west(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00001100'00001100);
	CHECK(WallOperations::SLIDE_TO[Square::G2][Square::G2.north(2)] == 0b00000000'00000000'00000000'00000000'00000011'00000011'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::F2][Square::F2.west(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00110000'00110000);
	CHECK(WallOperations::SLIDE_TO[Square::E2][Square::E2.east(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00000011'00000011);
	CHECK(WallOperations::SLIDE_TO[Square::D2][Square::D2.north(2)] == 0b00000000'00000000'00000000'00000000'00110000'00110000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::C2][Square::C2.east(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00001100'00001100);
	CHECK(WallOperations::SLIDE_TO[Square::B2][Square::B2.north(2)] == 0b00000000'00000000'00000000'00000000'11000000'11000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::A2][Square::A2.north(2)] == 0b00000000'00000000'00000000'00000000'11000000'11000000'00000000'00000000);

	CHECK(WallOperations::SLIDE_TO[Square::H3][Square::H3.west(2)] == 0b00000000'00000000'00000000'00000000'00001100'00001100'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::G3][Square::G3.north(2)] == 0b00000000'00000000'00000011'00000011'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::F3][Square::F3.north(2)] == 0b00000000'00000000'00001100'00001100'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::E3][Square::E3.south(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00001100'00001100);
	CHECK(WallOperations::SLIDE_TO[Square::D3][Square::D3.east(2)] == 0b00000000'00000000'00000000'00000000'00001100'00001100'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::C3][Square::C3.west(2)] == 0b00000000'00000000'00000000'00000000'11000000'11000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::B3][Square::B3.south(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'11000000'11000000);
	CHECK(WallOperations::SLIDE_TO[Square::A3][Square::A3.north(2)] == 0b00000000'00000000'11000000'11000000'00000000'00000000'00000000'00000000);

	CHECK(WallOperations::SLIDE_TO[Square::H4][Square::H4.west(2)] == 0b00000000'00000000'00000000'00000000'00001100'00001100'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::G4][Square::G4.north(2)] == 0b00000000'00000000'00000011'00000011'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::F4][Square::F4.south(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00001100'00001100);
	CHECK(WallOperations::SLIDE_TO[Square::E4][Square::E4.east(2)] == 0b00000000'00000000'00000000'00000000'00000011'00000011'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::D4][Square::D4.west(2)] == 0b00000000'00000000'00000000'00000000'11000000'11000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::C4][Square::C4.south(2)] == 0b00000000'00000000'00000000'00000000'00000000'00000000'00110000'00110000);
	CHECK(WallOperations::SLIDE_TO[Square::B4][Square::B4.north(2)] == 0b00000000'00000000'11000000'11000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::A4][Square::A4.east(2)] == 0b00000000'00000000'00000000'00000000'00110000'00110000'00000000'00000000);

	CHECK(WallOperations::SLIDE_TO[Square::H5][Square::H5.west(2)] == 0b00000000'00000000'00001100'00001100'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::G5][Square::G5.south(2)] == 0b00000000'00000000'00000000'00000000'00000011'00000011'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::F5][Square::F5.north(2)] == 0b00001100'00001100'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::E5][Square::E5.west(2)] == 0b00000000'00000000'00110000'00110000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::D5][Square::D5.south(2)] == 0b00000000'00000000'00000000'00000000'00110000'00110000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::C5][Square::C5.east(2)] == 0b00000000'00000000'00001100'00001100'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::B5][Square::B5.south(2)] == 0b00000000'00000000'00000000'00000000'11000000'11000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::A5][Square::A5.north(2)] == 0b11000000'11000000'00000000'00000000'00000000'00000000'00000000'00000000);

	CHECK(WallOperations::SLIDE_TO[Square::H6][Square::H6.north(2)] == 0b00000011'00000011'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::G6][Square::G6.west(2)] == 0b00000000'00000000'00001100'00001100'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::F6][Square::F6.east(2)] == 0b00000000'00000000'00000011'00000011'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::E6][Square::E6.north(2)] == 0b00001100'00001100'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::D6][Square::D6.south(2)] == 0b00000000'00000000'00000000'00000000'00110000'00110000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::C6][Square::C6.west(2)] == 0b00000000'00000000'11000000'11000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::B6][Square::B6.east(2)] == 0b00000000'00000000'00110000'00110000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::A6][Square::A6.south(2)] == 0b00000000'00000000'00000000'00000000'11000000'11000000'00000000'00000000);

	CHECK(WallOperations::SLIDE_TO[Square::H7][Square::H7.west(2)] == 0b00001100'00001100'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::G7][Square::G7.south(2)] == 0b00000000'00000000'00000011'00000011'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::F7][Square::F7.east(2)] == 0b00000011'00000011'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::E7][Square::E7.west(2)] == 0b00110000'00110000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::D7][Square::D7.south(2)] == 0b00000000'00000000'00110000'00110000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::C7][Square::C7.west(2)] == 0b11000000'11000000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::B7][Square::B7.east(2)] == 0b00110000'00110000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::A7][Square::A7.south(2)] == 0b00000000'00000000'11000000'11000000'00000000'00000000'00000000'00000000);

	CHECK(WallOperations::SLIDE_TO[Square::H8][Square::H8.west(2)] == 0b00001100'00001100'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::G8][Square::G8.south(2)] == 0b00000000'00000000'00000011'00000011'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::F8][Square::F8.east(2)] == 0b00000011'00000011'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::E8][Square::E8.west(2)] == 0b00110000'00110000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::D8][Square::D8.south(2)] == 0b00000000'00000000'00110000'00110000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::C8][Square::C8.west(2)] == 0b11000000'11000000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::B8][Square::B8.east(2)] == 0b00110000'00110000'00000000'00000000'00000000'00000000'00000000'00000000);
	CHECK(WallOperations::SLIDE_TO[Square::A8][Square::A8.south(2)] == 0b00000000'00000000'11000000'11000000'00000000'00000000'00000000'00000000);
}

TEST_CASE("WallOperations slide direction") {
	using namespace phase4::engine::common;

	constexpr auto NORTH = WallOperations::NORTH * 2;
	constexpr auto EAST = WallOperations::EAST * 2;
	constexpr auto SOUTH = WallOperations::SOUTH * 2;
	constexpr auto WEST = WallOperations::WEST * 2;

	//                              WALLS       LANDING
	CHECK(WallOperations::SLIDE_DIR[Square::H1][Square::H1.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::G1][Square::G1.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::F1][Square::F1.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::E1][Square::E1.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::D1][Square::D1.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::C1][Square::C1.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::B1][Square::B1.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::A1][Square::A1.east(2)] == EAST);

	CHECK(WallOperations::SLIDE_DIR[Square::H2][Square::H2.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::G2][Square::G2.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::F2][Square::F2.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::E2][Square::E2.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::D2][Square::D2.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::C2][Square::C2.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::B2][Square::B2.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::A2][Square::A2.north(2)] == NORTH);

	CHECK(WallOperations::SLIDE_DIR[Square::H3][Square::H3.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::G3][Square::G3.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::F3][Square::F3.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::E3][Square::E3.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::D3][Square::D3.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::C3][Square::C3.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::B3][Square::B3.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::A3][Square::A3.north(2)] == NORTH);

	CHECK(WallOperations::SLIDE_DIR[Square::H4][Square::H4.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::G4][Square::G4.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::F4][Square::F4.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::E4][Square::E4.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::D4][Square::D4.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::C4][Square::C4.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::B4][Square::B4.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::A4][Square::A4.east(2)] == EAST);

	CHECK(WallOperations::SLIDE_DIR[Square::H5][Square::H5.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::G5][Square::G5.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::F5][Square::F5.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::E5][Square::E5.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::D5][Square::D5.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::C5][Square::C5.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::B5][Square::B5.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::A5][Square::A5.north(2)] == NORTH);

	CHECK(WallOperations::SLIDE_DIR[Square::H6][Square::H6.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::G6][Square::G6.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::F6][Square::F6.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::E6][Square::E6.north(2)] == NORTH);
	CHECK(WallOperations::SLIDE_DIR[Square::D6][Square::D6.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::C6][Square::C6.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::B6][Square::B6.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::A6][Square::A6.south(2)] == SOUTH);

	CHECK(WallOperations::SLIDE_DIR[Square::H7][Square::H7.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::G7][Square::G7.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::F7][Square::F7.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::E7][Square::E7.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::D7][Square::D7.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::C7][Square::C7.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::B7][Square::B7.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::A7][Square::A7.south(2)] == SOUTH);

	CHECK(WallOperations::SLIDE_DIR[Square::H8][Square::H8.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::G8][Square::G8.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::F8][Square::F8.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::E8][Square::E8.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::D8][Square::D8.south(2)] == SOUTH);
	CHECK(WallOperations::SLIDE_DIR[Square::C8][Square::C8.west(2)] == WEST);
	CHECK(WallOperations::SLIDE_DIR[Square::B8][Square::B8.east(2)] == EAST);
	CHECK(WallOperations::SLIDE_DIR[Square::A8][Square::A8.south(2)] == SOUTH);
}

TEST_CASE("WallOperations slide square") {
	using namespace phase4::engine::common;

	//                                 WALLS       LANDING
	CHECK(WallOperations::SLIDE_SQUARE[Square::H1][Square::H1.north(2)] == Square::H1);
	CHECK(WallOperations::SLIDE_SQUARE[Square::G1][Square::G1.west(2)] == Square::G1);
	CHECK(WallOperations::SLIDE_SQUARE[Square::F1][Square::F1.east(2)] == Square::F1);
	CHECK(WallOperations::SLIDE_SQUARE[Square::E1][Square::E1.north(2)] == Square::E1);
	CHECK(WallOperations::SLIDE_SQUARE[Square::D1][Square::D1.east(2)] == Square::D1);
	CHECK(WallOperations::SLIDE_SQUARE[Square::C1][Square::C1.west(2)] == Square::C1);
	CHECK(WallOperations::SLIDE_SQUARE[Square::B1][Square::B1.north(2)] == Square::B1);
	CHECK(WallOperations::SLIDE_SQUARE[Square::A1][Square::A1.east(2)] == Square::A1);

	CHECK(WallOperations::SLIDE_SQUARE[Square::H2][Square::H2.west(2)] == Square::H2);
	CHECK(WallOperations::SLIDE_SQUARE[Square::G2][Square::G2.north(2)] == Square::G2);
	CHECK(WallOperations::SLIDE_SQUARE[Square::F2][Square::F2.west(2)] == Square::F2);
	CHECK(WallOperations::SLIDE_SQUARE[Square::E2][Square::E2.east(2)] == Square::E2);
	CHECK(WallOperations::SLIDE_SQUARE[Square::D2][Square::D2.north(2)] == Square::D2);
	CHECK(WallOperations::SLIDE_SQUARE[Square::C2][Square::C2.east(2)] == Square::C2);
	CHECK(WallOperations::SLIDE_SQUARE[Square::B2][Square::B2.north(2)] == Square::B2);
	CHECK(WallOperations::SLIDE_SQUARE[Square::A2][Square::A2.north(2)] == Square::A2);

	CHECK(WallOperations::SLIDE_SQUARE[Square::H3][Square::H3.west(2)] == Square::H3);
	CHECK(WallOperations::SLIDE_SQUARE[Square::G3][Square::G3.north(2)] == Square::G3);
	CHECK(WallOperations::SLIDE_SQUARE[Square::F3][Square::F3.north(2)] == Square::F3);
	CHECK(WallOperations::SLIDE_SQUARE[Square::E3][Square::E3.south(2)] == Square::E3);
	CHECK(WallOperations::SLIDE_SQUARE[Square::D3][Square::D3.east(2)] == Square::D3);
	CHECK(WallOperations::SLIDE_SQUARE[Square::C3][Square::C3.west(2)] == Square::C3);
	CHECK(WallOperations::SLIDE_SQUARE[Square::B3][Square::B3.south(2)] == Square::B3);
	CHECK(WallOperations::SLIDE_SQUARE[Square::A3][Square::A3.north(2)] == Square::A3);

	CHECK(WallOperations::SLIDE_SQUARE[Square::H4][Square::H4.west(2)] == Square::H4);
	CHECK(WallOperations::SLIDE_SQUARE[Square::G4][Square::G4.north(2)] == Square::G4);
	CHECK(WallOperations::SLIDE_SQUARE[Square::F4][Square::F4.south(2)] == Square::F4);
	CHECK(WallOperations::SLIDE_SQUARE[Square::E4][Square::E4.east(2)] == Square::E4);
	CHECK(WallOperations::SLIDE_SQUARE[Square::D4][Square::D4.west(2)] == Square::D4);
	CHECK(WallOperations::SLIDE_SQUARE[Square::C4][Square::C4.south(2)] == Square::C4);
	CHECK(WallOperations::SLIDE_SQUARE[Square::B4][Square::B4.north(2)] == Square::B4);
	CHECK(WallOperations::SLIDE_SQUARE[Square::A4][Square::A4.east(2)] == Square::A4);

	CHECK(WallOperations::SLIDE_SQUARE[Square::H5][Square::H5.west(2)] == Square::H5);
	CHECK(WallOperations::SLIDE_SQUARE[Square::G5][Square::G5.south(2)] == Square::G5);
	CHECK(WallOperations::SLIDE_SQUARE[Square::F5][Square::F5.north(2)] == Square::F5);
	CHECK(WallOperations::SLIDE_SQUARE[Square::E5][Square::E5.west(2)] == Square::E5);
	CHECK(WallOperations::SLIDE_SQUARE[Square::D5][Square::D5.south(2)] == Square::D5);
	CHECK(WallOperations::SLIDE_SQUARE[Square::C5][Square::C5.east(2)] == Square::C5);
	CHECK(WallOperations::SLIDE_SQUARE[Square::B5][Square::B5.south(2)] == Square::B5);
	CHECK(WallOperations::SLIDE_SQUARE[Square::A5][Square::A5.north(2)] == Square::A5);

	CHECK(WallOperations::SLIDE_SQUARE[Square::H6][Square::H6.north(2)] == Square::H6);
	CHECK(WallOperations::SLIDE_SQUARE[Square::G6][Square::G6.west(2)] == Square::G6);
	CHECK(WallOperations::SLIDE_SQUARE[Square::F6][Square::F6.east(2)] == Square::F6);
	CHECK(WallOperations::SLIDE_SQUARE[Square::E6][Square::E6.north(2)] == Square::E6);
	CHECK(WallOperations::SLIDE_SQUARE[Square::D6][Square::D6.south(2)] == Square::D6);
	CHECK(WallOperations::SLIDE_SQUARE[Square::C6][Square::C6.west(2)] == Square::C6);
	CHECK(WallOperations::SLIDE_SQUARE[Square::B6][Square::B6.east(2)] == Square::B6);
	CHECK(WallOperations::SLIDE_SQUARE[Square::A6][Square::A6.south(2)] == Square::A6);

	CHECK(WallOperations::SLIDE_SQUARE[Square::H7][Square::H7.west(2)] == Square::H7);
	CHECK(WallOperations::SLIDE_SQUARE[Square::G7][Square::G7.south(2)] == Square::G7);
	CHECK(WallOperations::SLIDE_SQUARE[Square::F7][Square::F7.east(2)] == Square::F7);
	CHECK(WallOperations::SLIDE_SQUARE[Square::E7][Square::E7.west(2)] == Square::E7);
	CHECK(WallOperations::SLIDE_SQUARE[Square::D7][Square::D7.south(2)] == Square::D7);
	CHECK(WallOperations::SLIDE_SQUARE[Square::C7][Square::C7.west(2)] == Square::C7);
	CHECK(WallOperations::SLIDE_SQUARE[Square::B7][Square::B7.east(2)] == Square::B7);
	CHECK(WallOperations::SLIDE_SQUARE[Square::A7][Square::A7.south(2)] == Square::A7);

	CHECK(WallOperations::SLIDE_SQUARE[Square::H8][Square::H8.west(2)] == Square::H8);
	CHECK(WallOperations::SLIDE_SQUARE[Square::G8][Square::G8.south(2)] == Square::G8);
	CHECK(WallOperations::SLIDE_SQUARE[Square::F8][Square::F8.east(2)] == Square::F8);
	CHECK(WallOperations::SLIDE_SQUARE[Square::E8][Square::E8.west(2)] == Square::E8);
	CHECK(WallOperations::SLIDE_SQUARE[Square::D8][Square::D8.south(2)] == Square::D8);
	CHECK(WallOperations::SLIDE_SQUARE[Square::C8][Square::C8.west(2)] == Square::C8);
	CHECK(WallOperations::SLIDE_SQUARE[Square::B8][Square::B8.east(2)] == Square::B8);
	CHECK(WallOperations::SLIDE_SQUARE[Square::A8][Square::A8.south(2)] == Square::A8);
}
