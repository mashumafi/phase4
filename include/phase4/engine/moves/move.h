#ifndef PHASE4_ENGINE_MOVES_MOVE_H
#define PHASE4_ENGINE_MOVES_MOVE_H

#include <phase4/engine/common/square.h>
#include <phase4/engine/moves/move_flags.h>

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
	uint16_t _data;

	friend bool operator==(const Move a, const Move b);
	friend bool operator!=(const Move a, const Move b);
};

constexpr common::Square Move::from() const {
	return common::Square(_data & 0x3F);
}

constexpr common::Square Move::to() const {
	return common::Square((_data >> 6) & 0x3F);
}

constexpr MoveFlags Move::flags() const {
	return MoveFlags(_data >> 12);
}

constexpr Move::Move() :
		_data(0) {}

inline constexpr Move Move::Empty;

constexpr Move::Move(common::Square from, common::Square to, MoveFlags flags) :
		_data(
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

std::ostream &operator<<(std::ostream &os, const Move &move) {
	os << common::Square(move.from()) << common::Square(move.to());
	if (move.flags().isPromotion()) {
		os << move.flags().getPromotionSymbol();
	}
	return os;
}

bool operator==(const Move a, const Move b) {
	return a._data == b._data;
}

bool operator!=(const Move a, const Move b) {
	return a._data != b._data;
}

} //namespace phase4::engine::moves

#endif
