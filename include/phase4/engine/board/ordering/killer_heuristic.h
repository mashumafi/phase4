#ifndef PHASE4_ENGINE_BOARD_ORDERING_KILLER_HEURISTIC_H
#define PHASE4_ENGINE_BOARD_ORDERING_KILLER_HEURISTIC_H

#include <phase4/engine/board/ordering/move_ordering_constants.h>
#include <phase4/engine/board/search_constants.h>

#include <phase4/engine/moves/move.h>

#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/util.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board::ordering {

class KillerHeuristic {
public:
	using KillerMovesArray = std::array<moves::Move, ordering::MoveOrderingConstants::KILLER_SLOTS>;
	using ColorKillerMovesArray = std::array<std::array<KillerMovesArray, SearchConstants::MAX_DEPTH>, 2>;

	void addKillerMove(moves::Move move, common::PieceColor color, int32_t ply) {
		// TODO: use size()
		for (int8_t slot = MoveOrderingConstants::KILLER_SLOTS - 2; slot >= 0; slot--) {
			m_killerMoves[color.get_raw_value()][ply][slot + 1] = m_killerMoves[color.get_raw_value()][ply][slot];
		}

		m_killerMoves[color.get_raw_value()][ply][0] = move;
	}

	bool killerMoveExists(moves::Move move, common::PieceColor color, int32_t ply) const {
		// TODO: use size()
		for (int8_t slot = 0; slot < ordering::MoveOrderingConstants::KILLER_SLOTS; slot++) {
			if (m_killerMoves[color.get_raw_value()][ply][slot] == move) {
				return true;
			}
		}

		return false;
	}

	void ageKillers() {
		// TODO: use size()
		for (common::PieceColor color = common::PieceColor::WHITE; color != common::PieceColor::INVALID; ++color) {
			for (int8_t ply = 0; ply < SearchConstants::MAX_DEPTH - 2; ply++) {
				m_killerMoves[color.get_raw_value()][ply] = m_killerMoves[color.get_raw_value()][ply + 2];
			}
		}

		// TODO: use size()
		for (common::PieceColor color = common::PieceColor::WHITE; color != common::PieceColor::INVALID; ++color) {
			for (int8_t ply = SearchConstants::MAX_DEPTH - 2; ply < SearchConstants::MAX_DEPTH; ply++) {
				common::util::clear(m_killerMoves[color.get_raw_value()][ply]);
			}
		}
	}

	void clear() {
		common::util::clear(m_killerMoves);
	}

private:
	ColorKillerMovesArray m_killerMoves;
};

} //namespace phase4::engine::board::ordering

#endif
