#ifndef PHASE4_ENGINE_MOVES_MOVE_H
#define PHASE4_ENGINE_MOVES_MOVE_H

#include <phase4/engine/common/fast_vector.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/moves/move_flags.h>

#include <sstream>

namespace phase4::engine::moves {

using UciNotation = std::array<char, 6>;

class Move {
public:
	static const Move EMPTY;

	constexpr common::Square from() const;

	constexpr common::Square to() const;

	constexpr MoveFlags flags() const;

	constexpr Move();

	constexpr Move(common::Square from, common::Square to, MoveFlags flags);

	constexpr Move(std::string_view textNotation);

	inline constexpr UciNotation asUciNotation() const noexcept;

	friend std::ostream &operator<<(std::ostream &os, const Move &move);

private:
#if NDEBUG
	uint16_t m_data;
#else
	common::Square m_from;
	common::Square m_to;
	MoveFlags m_flags;
#endif

	friend constexpr bool operator==(const Move a, const Move b);
	friend constexpr bool operator!=(const Move a, const Move b);
};

constexpr size_t MAX_MOVES_COUNT = 218;
using Moves = common::FastVector<Move, MAX_MOVES_COUNT>;
using MoveValues = common::FastVector<int32_t, MAX_MOVES_COUNT>;

constexpr common::Square Move::from() const {
#if NDEBUG
	return common::Square(m_data & 0x3F);
#else
	return m_from;
#endif
}

constexpr common::Square Move::to() const {
#if NDEBUG
	return common::Square((m_data >> 6) & 0x3F);
#else
	return m_to;
#endif
}

constexpr MoveFlags Move::flags() const {
#if NDEBUG
	return MoveFlags(m_data >> 12);
#else
	return m_flags;
#endif
}

constexpr Move::Move() :
#if NDEBUG
		m_data(0)
#else
		m_from(common::Square::BEGIN), m_to(common::Square::BEGIN), m_flags(MoveFlags::QUIET)
#endif
{
}

inline constexpr Move Move::EMPTY;

constexpr Move::Move(common::Square from, common::Square to, MoveFlags flags) :
#if NDEBUG
		m_data(
				static_cast<uint16_t>(from.get_raw_value()) |
				static_cast<uint16_t>(to.get_raw_value() << 6) |
				static_cast<uint16_t>(flags.get_raw_value() << 12))
#else
		m_from(from), m_to(to), m_flags(flags)
#endif
{
}

constexpr Move::Move(std::string_view textNotation) :
		Move(
				common::Square(textNotation.substr(0, 2)),
				common::Square(textNotation.substr(2, 2)),
				(textNotation.size() >= 5) ? MoveFlags::getPromotionSymbolFlags(textNotation[4]) : MoveFlags::QUIET) {
}

inline constexpr UciNotation Move::asUciNotation() const noexcept {
	UciNotation move = { 0 };
	const std::array<char, 3> &fromBuffer = from().asBuffer();
	const std::array<char, 3> &toBuffer = to().asBuffer();

	move[0] = fromBuffer[0];
	move[1] = fromBuffer[1];
	move[2] = toBuffer[0];
	move[3] = toBuffer[1];

	if (unlikely(flags().isPromotion())) {
		move[4] = flags().getPromotionSymbol();
		move[5] = '\0';
	} else {
		move[4] = '\0';
	}

	return move;
}

inline std::ostream &operator<<(std::ostream &os, const Move &move) {
	return os << move.asUciNotation().data();
}

inline constexpr bool operator==(const Move a, const Move b) {
#if NDEBUG
	return a.m_data == b.m_data;
#else
	return a.m_from == b.m_from && a.m_to == b.m_to && a.m_flags.get_raw_value() == b.m_flags.get_raw_value();
#endif
}

inline constexpr bool operator!=(const Move a, const Move b) {
#if NDEBUG
	return a.m_data != b.m_data;
#else
	return a.m_from != b.m_from || a.m_to != b.m_to || a.m_flags.get_raw_value() != b.m_flags.get_raw_value();
#endif
}

namespace literals {

inline Move operator""_m(const char *notation, size_t count) {
	return Move(std::string_view(notation, count));
}

} //namespace literals

} //namespace phase4::engine::moves

#endif
