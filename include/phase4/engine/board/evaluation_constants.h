#ifndef PHASE4_ENGINE_BOARD_EVALUATION_CONSTANTS_H
#define PHASE4_ENGINE_BOARD_EVALUATION_CONSTANTS_H

#include <phase4/engine/board/evaluation_constants.h>

#include <phase4/engine/common/piece_type.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board {

struct EvaluationConstants {
	static inline constexpr int16_t pieceValue(common::PieceType piece);

	static const int32_t CHECKMATE;
	static const int32_t THREEFOLD_REPETITION;
	static const int32_t INSUFFICIENT_MATERIAL;

	static const std::array<int32_t, 2> DOUBLED_PAWNS;
	static const std::array<int32_t, 2> ISOLATED_PAWNS;
	static const std::array<int32_t, 2> CHAINED_PAWNS;
	static const std::array<int32_t, 2> PASSING_PAWNS;

	static const int32_t CENTER_MOBILITY_MODIFIER;
	static const int32_t OUTSIDE_MOBILITY_MODIFIER;

	static const int32_t KING_IN_DANGER;
	static const int32_t PAWN_SHIELD;
	static const int32_t OPEN_FILE_NEXT_TO_KING;

	static const int32_t DOUBLED_ROOKS;
	static const int32_t ROOK_ON_OPEN_FILE;

	static const int32_t PAIR_OF_BISHOPS;
	static const int32_t FIANCHETTO;
	static const int32_t FIANCHETTO_WITHOUT_BISHOP;

	static const int32_t OPENING_ENDGAME_EDGE;

	static const uint64_t CENTER;
	static const uint64_t EXTENDED_CENTER;
	static const uint64_t EXTENDED_CENTER_RING;
	static const uint64_t OUTSIDE;

	static constexpr int32_t calculateMaterialAtOpening();

	static const int32_t MATERIAL_AT_OPENING;

private:
	static const std::array<int16_t, 6> PIECE_VALUES;
};

inline constexpr std::array<int16_t, 6> EvaluationConstants::PIECE_VALUES = { 100, 350, 370, 570, 1190, 20000 };

inline constexpr int16_t EvaluationConstants::pieceValue(common::PieceType piece) {
	return EvaluationConstants::PIECE_VALUES[piece.get_raw_value()];
}

inline constexpr int32_t EvaluationConstants::calculateMaterialAtOpening() {
	return EvaluationConstants::pieceValue(common::PieceType::KING) +
			EvaluationConstants::pieceValue(common::PieceType::QUEEN) +
			EvaluationConstants::pieceValue(common::PieceType::ROOK) * 2 +
			EvaluationConstants::pieceValue(common::PieceType::BISHOP) * 2 +
			EvaluationConstants::pieceValue(common::PieceType::KNIGHT) * 2 +
			EvaluationConstants::pieceValue(common::PieceType::PAWN) * 8;
}

inline constexpr int32_t EvaluationConstants::CHECKMATE = 32000;
inline constexpr int32_t EvaluationConstants::THREEFOLD_REPETITION = 0;
inline constexpr int32_t EvaluationConstants::INSUFFICIENT_MATERIAL = 0;

inline constexpr std::array<int32_t, 2> EvaluationConstants::DOUBLED_PAWNS = { -10, -30 };
inline constexpr std::array<int32_t, 2> EvaluationConstants::ISOLATED_PAWNS = { -25, -5 };
inline constexpr std::array<int32_t, 2> EvaluationConstants::CHAINED_PAWNS = { 4, 4 };
inline constexpr std::array<int32_t, 2> EvaluationConstants::PASSING_PAWNS = { 0, 30 };

inline constexpr int32_t EvaluationConstants::CENTER_MOBILITY_MODIFIER = 7;
inline constexpr int32_t EvaluationConstants::OUTSIDE_MOBILITY_MODIFIER = 6;

inline constexpr int32_t EvaluationConstants::KING_IN_DANGER = -20;
inline constexpr int32_t EvaluationConstants::PAWN_SHIELD = 20;
inline constexpr int32_t EvaluationConstants::OPEN_FILE_NEXT_TO_KING = -30;

inline constexpr int32_t EvaluationConstants::DOUBLED_ROOKS = 40;
inline constexpr int32_t EvaluationConstants::ROOK_ON_OPEN_FILE = 50;

inline constexpr int32_t EvaluationConstants::PAIR_OF_BISHOPS = 50;
inline constexpr int32_t EvaluationConstants::FIANCHETTO = 25;
inline constexpr int32_t EvaluationConstants::FIANCHETTO_WITHOUT_BISHOP = -25;

inline constexpr int32_t EvaluationConstants::OPENING_ENDGAME_EDGE = 20500;

inline constexpr uint64_t EvaluationConstants::CENTER = 0x1818000000;
inline constexpr uint64_t EvaluationConstants::EXTENDED_CENTER = 0x3c3c3c3c0000;
inline constexpr uint64_t EvaluationConstants::EXTENDED_CENTER_RING = EvaluationConstants::EXTENDED_CENTER & ~EvaluationConstants::CENTER;
inline constexpr uint64_t EvaluationConstants::OUTSIDE = 0xffffc3c3c3c3ffff;

inline constexpr int32_t EvaluationConstants::MATERIAL_AT_OPENING = calculateMaterialAtOpening();
static_assert(EvaluationConstants::MATERIAL_AT_OPENING == 24570); // Check for overflow

} //namespace phase4::engine::board
#endif
