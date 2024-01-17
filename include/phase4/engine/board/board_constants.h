#ifndef PHASE4_ENGINE_BOARD_BOARD_CONSTANTS_H
#define PHASE4_ENGINE_BOARD_BOARD_CONSTANTS_H

#include <array>
#include <cstdint>

namespace phase4::engine::board {

class BoardConstants {
public:
	static const uint64_t FULL;
	static const uint64_t EMPTY;

	static const uint64_t FILE_A;
	static const uint64_t FILE_B;
	static const uint64_t FILE_C;
	static const uint64_t FILE_D;
	static const uint64_t FILE_E;
	static const uint64_t FILE_F;
	static const uint64_t FILE_G;
	static const uint64_t FILE_H;

	static const uint64_t RANK_1;
	static const uint64_t RANK_2;
	static const uint64_t RANK_3;
	static const uint64_t RANK_4;
	static const uint64_t RANK_5;
	static const uint64_t RANK_6;
	static const uint64_t RANK_7;
	static const uint64_t RANK_8;

	static const uint64_t EDGES;
	static const uint64_t BOARD_WITHOUT_EDGES;
	static const uint64_t RIGHT_LEFT_EDGE;
	static const uint64_t TOP_BOTTOM_EDGE;
	static const uint64_t CORNERS;

	static const uint64_t NEAR_PROMOTION_AREA_WHITE;
	static const uint64_t NEAR_PROMOTION_AREA_BLACK;

	static const std::array<int, 64> DISTANCE_FROM_CENTER;

	static const int PHASE_RESOLUTION;
};

inline constexpr uint64_t BoardConstants::FULL = UINT64_MAX;
inline constexpr uint64_t BoardConstants::EMPTY = 0;

inline constexpr uint64_t BoardConstants::FILE_A = 0x8080808080808080;
inline constexpr uint64_t BoardConstants::FILE_B = 0x4040404040404040;
inline constexpr uint64_t BoardConstants::FILE_C = 0x2020202020202020;
inline constexpr uint64_t BoardConstants::FILE_D = 0x1010101010101010;
inline constexpr uint64_t BoardConstants::FILE_E = 0x0808080808080808;
inline constexpr uint64_t BoardConstants::FILE_F = 0x0404040404040404;
inline constexpr uint64_t BoardConstants::FILE_G = 0x0202020202020202;
inline constexpr uint64_t BoardConstants::FILE_H = 0x0101010101010101;

inline constexpr uint64_t BoardConstants::RANK_1 = 0x00000000000000FF;
inline constexpr uint64_t BoardConstants::RANK_2 = 0x000000000000FF00;
inline constexpr uint64_t BoardConstants::RANK_3 = 0x0000000000FF0000;
inline constexpr uint64_t BoardConstants::RANK_4 = 0x00000000FF000000;
inline constexpr uint64_t BoardConstants::RANK_5 = 0x000000FF00000000;
inline constexpr uint64_t BoardConstants::RANK_6 = 0x0000FF0000000000;
inline constexpr uint64_t BoardConstants::RANK_7 = 0x00FF000000000000;
inline constexpr uint64_t BoardConstants::RANK_8 = 0xFF00000000000000;

inline constexpr uint64_t BoardConstants::EDGES = FILE_A | FILE_H | RANK_1 | RANK_8;
inline constexpr uint64_t BoardConstants::BOARD_WITHOUT_EDGES = FULL & ~EDGES;
inline constexpr uint64_t BoardConstants::RIGHT_LEFT_EDGE = FILE_A | FILE_H;
inline constexpr uint64_t BoardConstants::TOP_BOTTOM_EDGE = RANK_1 | RANK_8;
inline constexpr uint64_t BoardConstants::CORNERS = 0x8100000000000081;

inline constexpr uint64_t BoardConstants::NEAR_PROMOTION_AREA_WHITE = 0x00ffffff00000000;
inline constexpr uint64_t BoardConstants::NEAR_PROMOTION_AREA_BLACK = 0x00000000ffffff00;

inline constexpr std::array<int, 64> BoardConstants::DISTANCE_FROM_CENTER = {
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 2, 2, 2, 2, 2, 2, 3,
	3, 2, 1, 1, 1, 1, 2, 3,
	3, 2, 1, 0, 0, 1, 2, 3,
	3, 2, 1, 0, 0, 1, 2, 3,
	3, 2, 1, 1, 1, 1, 2, 3,
	3, 2, 2, 2, 2, 2, 2, 3,
	3, 3, 3, 3, 3, 3, 3, 3
};

inline constexpr int BoardConstants::PHASE_RESOLUTION = 1024;

} //namespace phase4::engine::board

#endif
