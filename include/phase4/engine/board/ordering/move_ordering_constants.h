#ifndef PHASE4_ENGINE_BOARD_ORDERING_MOVE_ORDERING_CONSTANTS_H
#define PHASE4_ENGINE_BOARD_ORDERING_MOVE_ORDERING_CONSTANTS_H

#include <cstdint>

namespace phase4::engine::board::ordering {

class MoveOrderingConstants {
public:
	static constexpr int16_t HASH_MOVE = 10000;
	static constexpr int16_t PROMOTION = 5000;
	static constexpr int16_t CASTLING = 1000;
	static constexpr int16_t PAWN_NEAR_PROMOTION = 300;
	static constexpr int16_t CAPTURE = 200;
	static constexpr int16_t EN_PASSANT = 100;
	static constexpr int16_t KILLERMOVE = 90;

	static constexpr uint32_t HISTORY_HEURISTIC_MAX_SCORE = 80;
	static constexpr int8_t KILLER_SLOTS = 3;
};

} //namespace phase4::engine::board::ordering

#endif
