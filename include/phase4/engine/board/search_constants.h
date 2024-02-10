#ifndef PHASE4_ENGINE_BOARD_SEARCH_CONSTANTS_H
#define PHASE4_ENGINE_BOARD_SEARCH_CONSTANTS_H

#include <phase4/engine/board/evaluation_constants.h>

#include <phase4/engine/common/math.h>

#include <cstdint>
#include <limits>

namespace phase4::engine::board {

class SearchConstants {
public:
	static constexpr int32_t MIN_VALUE = std::numeric_limits<int16_t>::min();
	static constexpr int32_t MAX_VALUE = std::numeric_limits<int16_t>::max();
	static constexpr int32_t NO_KING_VALUE = MIN_VALUE - 10;
	static constexpr int8_t MAX_DEPTH = 32;
	static constexpr int32_t MAX_MOVES_COUNT = 218;
	static constexpr float DEADLINE_FACTOR = 1.5f;

	static constexpr int32_t INTERNAL_ITERATIVE_DEEPENING_MIN_DEPTH = 4;
	static constexpr int32_t INTERNAL_ITERATIVE_DEEPENING_DEPTH_REDUCTION = 2;

	static constexpr int32_t RAZORING_MIN_DEPTH = 1;
	static constexpr int32_t RAZORING_MAX_DEPTH = 3;
	static constexpr int32_t RAZORING_MARGIN = 70;
	static constexpr int32_t RAZORING_MARGIN_MULTIPLIER = 150;

	static constexpr int32_t STATIC_NULL_MOVE_MAX_DEPTH = 3;
	static constexpr int32_t STATIC_NULL_MOVE_MAX_DEPTH_DIVIDER = 7;
	static constexpr int32_t STATIC_NULL_MOVE_MARGIN = 300;
	static constexpr int32_t STATIC_NULL_MOVE_MARGIN_MULTIPLIER = 200;

	static constexpr int32_t NULL_MOVE_MIN_DEPTH = 2;
	static constexpr int32_t NULL_MOVE_DEPTH_REDUCTION = 3;
	static constexpr int32_t NULL_MOVE_DEPTH_REDUCTION_DIVIDER = 4;

	static constexpr int32_t FUTILITY_PRUNING_MAX_DEPTH = 3;
	static constexpr int32_t FUTILITY_PRUNING_MAX_DEPTH_DIVISOR = 7;
	static constexpr int32_t FUTILITY_PRUNING_MARGIN = 200;
	static constexpr int32_t FUTILITY_PRUNING_MARGIN_MULTIPLIER = 100;
	static constexpr int32_t QFUTILITY_PRUNING_MARGIN = 100;

	static constexpr int32_t LATE_MOVE_REDUCTIONS_MIN_DEPTH = 2;
	static constexpr int32_t LATE_MOVE_REDUCTIONS_MOVES_WITHOUT_REDUCTION = 3;
	static constexpr int32_t LATE_MOVE_REDUCTIONS_BASE_REDUCTION = 1;
	static constexpr int32_t LATE_MOVE_REDUCTIONS_MOVE_INDEX_DIVIDER = 4;
	static constexpr int32_t LATE_MOVE_REDUCTIONS_PV_NODE_MAX_REDUCTION = 2;
	static constexpr int32_t LATE_MOVE_REDUCTIONS_NON_PV_NODE_MAX_REDUCTION = 4;
	static constexpr int32_t LATE_MOVE_REDUCTIONS_MAX_HISTORY_VALUE_DIVIDER = 2;

	static constexpr int32_t LATE_MOVE_PRUNING_MAX_DEPTH = 2;
	static constexpr int32_t LATE_MOVE_PRUNING_BASE_PERCENT_MOVES_TO_PRUNE = 50;
	static constexpr int32_t LATE_MOVE_PRUNING_PERCENT_INCREASE_PER_DEPTH = 25;

	static constexpr bool isScoreNearCheckmate(int32_t score) {
		const int32_t scoreAbs = common::Math::abs_int32(score);
		return scoreAbs >= EvaluationConstants::CHECKMATE - MAX_DEPTH &&
				scoreAbs <= EvaluationConstants::CHECKMATE + MAX_DEPTH;
	}
};

} //namespace phase4::engine::board

#endif
