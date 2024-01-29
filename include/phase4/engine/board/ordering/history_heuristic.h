#ifndef PHASE4_ENGINE_BOARD_ORDERING_HISTORY_HEURISTIC_H
#define PHASE4_ENGINE_BOARD_ORDERING_HISTORY_HEURISTIC_H

#include <phase4/engine/common/math.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board::ordering {

class HistoryHeuristic {
public:
	using Array = std::array<std::array<std::array<uint32_t, 64>, 6>, 2>;

	inline void addHistoryMove(common::PieceColor color, common::PieceType piece, common::Square to, int depth) {
		const uint32_t newValue = m_historyMoves[color.get_raw_value()][piece.get_raw_value()][to] + static_cast<uint32_t>(depth * depth);

		m_max = common::Math::max_uint32(m_max, newValue);
		m_historyMoves[color.get_raw_value()][piece.get_raw_value()][to] = newValue;
	}

	inline int32_t getMoveValue(common::PieceColor color, common::PieceType piece, common::Square to, uint32_t scale) const {
		return static_cast<int32_t>(m_historyMoves[color.get_raw_value()][piece.get_raw_value()][to] * scale / m_max);
	}

	inline uint32_t getRawMoveValue(common::PieceColor color, common::PieceType piece, common::Square to) const {
		return m_historyMoves[color.get_raw_value()][piece.get_raw_value()][to];
	}

	uint32_t getMaxValue() const {
		return m_max;
	}

	inline void ageValues() {
		for (common::PieceColor color = common::PieceColor::WHITE; color != common::PieceColor::INVALID; ++color) {
			for (common::PieceType piece = common::PieceType::PAWN; piece != common::PieceType::INVALID; ++piece) {
				for (common::Square to = common::Square::BEGIN; to != common::Square::INVALID; ++to) {
					m_historyMoves[color.get_raw_value()][piece.get_raw_value()][to] /= 2;
				}
			}
		}

		m_max = common::Math::max_uint32(m_max / 2, 1);
	}

	inline void clear() {
		static constexpr Array blank = {};
		m_historyMoves = blank;

		m_max = 1;
	}

private:
	Array m_historyMoves;
	uint32_t m_max = 1;
};

} //namespace phase4::engine::board::ordering

#endif
