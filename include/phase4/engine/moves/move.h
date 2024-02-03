#ifndef PHASE4_ENGINE_MOVES_MOVE_H
#define PHASE4_ENGINE_MOVES_MOVE_H

#include <phase4/engine/common/fast_vector.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/moves/move_flags.h>

#include <sstream>

namespace phase4::engine::moves {

class Move {
public:
	static const Move Empty;

	constexpr common::Square from() const;

	constexpr common::Square to() const;

	constexpr MoveFlags flags() const;

	constexpr Move();

	constexpr Move(common::Square from, common::Square to, MoveFlags flags);

	constexpr Move(std::string_view textNotation);

	friend std::ostream &operator<<(std::ostream &os, const Move &move);

private:
	uint16_t m_data;

	friend constexpr bool operator==(const Move a, const Move b);
	friend constexpr bool operator!=(const Move a, const Move b);
};

constexpr size_t MAX_MOVES_COUNT = 218;
using Moves = common::FastVector<Move, MAX_MOVES_COUNT>;
using MoveValues = common::FastVector<int16_t, MAX_MOVES_COUNT>;

constexpr common::Square Move::from() const {
	return common::Square(m_data & 0x3F);
}

constexpr common::Square Move::to() const {
	return common::Square((m_data >> 6) & 0x3F);
}

constexpr MoveFlags Move::flags() const {
	return MoveFlags(m_data >> 12);
}

constexpr Move::Move() :
		m_data(0) {}

inline constexpr Move Move::Empty;

constexpr Move::Move(common::Square from, common::Square to, MoveFlags flags) :
		m_data(
				static_cast<uint16_t>(from.get_raw_value()) |
				static_cast<uint16_t>(to.get_raw_value() << 6) |
				static_cast<uint16_t>(flags.get_raw_value() << 12)) {
}

constexpr Move::Move(std::string_view textNotation) :
		Move(
				common::Square(textNotation.substr(0, 2)),
				common::Square(textNotation.substr(2, 2)),
				(textNotation.size() >= 5) ? MoveFlags::getPromotionSymbolFlags(textNotation[4]) : MoveFlags::QUIET) {
}

inline std::ostream &operator<<(std::ostream &os, const Move &move) {
	os << common::Square(move.from()) << common::Square(move.to());
	if (unlikely(move.flags().isPromotion())) {
		os << move.flags().getPromotionSymbol();
	}
	return os;
}

inline constexpr bool operator==(const Move a, const Move b) {
	return a.m_data == b.m_data;
}

inline constexpr bool operator!=(const Move a, const Move b) {
	return a.m_data != b.m_data;
}

} //namespace phase4::engine::moves

#endif
