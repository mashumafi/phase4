#ifndef PHASE4_ENGINE_BOARD_OPERATORS_PAWN_OPERATOR_H
#define PHASE4_ENGINE_BOARD_OPERATORS_PAWN_OPERATOR_H

#include <phase4/engine/ai/score/evaluation_constants.h>
#include <phase4/engine/board/board_constants.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/common/wall_operations.h>
#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>

#include <cstdint>

namespace phase4::engine::board::operators {

class PawnOperator {
public:
	static void getLoudMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		PieceColor color = position.m_colorToMove;

		getSinglePush(position, moves, true, evasionMask);
		getDiagonalAttacks(position, color == PieceColor::WHITE ? 9 : 7, BoardConstants::FILE_A, moves, evasionMask);
		getDiagonalAttacks(position, color == PieceColor::WHITE ? 7 : 9, BoardConstants::FILE_H, moves, evasionMask);
	}

	static void getQuietMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		getSinglePush(position, moves, false, evasionMask);
		getDoublePush(position, moves, evasionMask);
	}

	static void getAvailableCaptureMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		PieceColor color = position.m_colorToMove;

		getDiagonalAttacks(position, color == PieceColor::WHITE ? 9 : 7, BoardConstants::FILE_A, moves, Bitset::MAX);
		getDiagonalAttacks(position, color == PieceColor::WHITE ? 7 : 9, BoardConstants::FILE_H, moves, Bitset::MAX);
	}

	static bool isMoveLegal(const Position &position, moves::Move move) {
		using namespace common;

		const PieceColor enemyColor = position.m_colorToMove.invert();
		const Bitset toField = move.to().asBitboard();

		if (!move.flags().isCapture()) {
			if (move.flags().isSinglePush() || move.flags().isPromotion()) {
				if ((position.m_occupancySummary & toField) == 0) {
					return true;
				}
			} else if (move.flags().isDoublePush()) {
				const Square middleField(1ull << ((move.from() + move.to()) / 2));
				if ((position.m_occupancySummary & middleField) == 0 && (position.m_occupancySummary & toField) == 0) {
					return true;
				}
			}
		} else {
			if (move.flags().isEnPassant()) {
				if ((position.m_enPassant & toField) != 0) {
					return true;
				}
			} else {
				int16_t difference = move.to() - move.from();
				int16_t colorDifference = -(position.m_colorToMove.get_raw_value() * 2 - 1) * difference;

				if ((position.m_occupancyByColor[enemyColor.get_raw_value()] & toField) != 0 && (colorDifference == 7 || colorDifference == 9)) {
					return true;
				}
			}
		}

		return false;
	}

private:
	static void getSinglePush(const Position &position, moves::Moves &moves, bool promotionsMode, common::Bitset evasionMask) {
		using namespace common;

		int shift;
		Bitset promotionRank, pawns;
		const PieceColor color = position.m_colorToMove;

		if (color == PieceColor::WHITE) {
			shift = 8;
			promotionRank = BoardConstants::RANK_8;
			pawns = position.m_colorPieceMasks[PieceColor::WHITE.get_raw_value()][PieceType::PAWN.get_raw_value()];

			if (promotionsMode) {
				pawns &= BoardConstants::NEAR_PROMOTION_AREA_WHITE;
			} else {
				pawns &= ~BoardConstants::NEAR_PROMOTION_AREA_WHITE;
			}

			pawns = (pawns << 8) & ~position.m_occupancySummary;
		} else {
			shift = -8;
			promotionRank = BoardConstants::RANK_1;
			pawns = position.m_colorPieceMasks[PieceColor::BLACK.get_raw_value()][PieceType::PAWN.get_raw_value()];

			if (promotionsMode) {
				pawns &= BoardConstants::NEAR_PROMOTION_AREA_BLACK;
			} else {
				pawns &= ~BoardConstants::NEAR_PROMOTION_AREA_BLACK;
			}

			pawns = (pawns >> 8) & ~position.m_occupancySummary;
		}

		pawns &= evasionMask;
		while (pawns != 0) {
			const Bitset piece = pawns.getLsb();
			pawns = pawns.popLsb();

			const Square from(piece.bitScan() - shift);
			const Square to(piece.bitScan());

			// Note: Special case to handle promotion after sliding
			Bitset slideSquare = position.m_walls > 0 ? (WallOperations::SLIDE_SQUARE[position.m_walls.bitScan()][to].asBitboard()) : 0;

			if (promotionsMode && ((piece & promotionRank) != 0 || (slideSquare & promotionRank) != 0)) {
				moves.emplace_back(from, to, moves::MoveFlags::QUEEN_PROMOTION);
				moves.emplace_back(from, to, moves::MoveFlags::ROOK_PROMOTION);
				moves.emplace_back(from, to, moves::MoveFlags::KNIGHT_PROMOTION);
				moves.emplace_back(from, to, moves::MoveFlags::BISHOP_PROMOTION);
			} else {
				moves.emplace_back(from, to, moves::MoveFlags::QUIET);
			}
		}
	}

	static void getDoublePush(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		int shift;
		Bitset startRank, pawns;
		const PieceColor color = position.m_colorToMove;

		if (color == PieceColor::WHITE) {
			shift = 16;
			startRank = BoardConstants::RANK_2;
			pawns = position.m_colorPieceMasks[PieceColor::WHITE.get_raw_value()][PieceType::PAWN.get_raw_value()];
			pawns = ((pawns & startRank) << 8) & ~position.m_occupancySummary;
			pawns = (pawns << 8) & ~position.m_occupancySummary;
		} else {
			shift = -16;
			startRank = BoardConstants::RANK_7;
			pawns = position.m_colorPieceMasks[PieceColor::BLACK.get_raw_value()][PieceType::PAWN.get_raw_value()];
			pawns = ((pawns & startRank) >> 8) & ~position.m_occupancySummary;
			pawns = (pawns >> 8) & ~position.m_occupancySummary;
		}

		pawns &= evasionMask;
		while (pawns != 0) {
			const Bitset piece = pawns.getLsb();
			pawns = pawns.popLsb();

			const Square from(piece.bitScan() - shift);
			const Square to(piece.bitScan());

			moves.emplace_back(from, to, moves::MoveFlags::DOUBLE_PUSH);
		}
	}

	static void getDiagonalAttacks(const Position &position, int dir, common::Bitset prohibitedFile, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		int shift;
		Bitset promotionRank, enemyOccupancy, pawns;
		const PieceColor color = position.m_colorToMove;

		if (color == PieceColor::WHITE) {
			shift = dir;
			promotionRank = BoardConstants::RANK_8;
			enemyOccupancy = position.m_occupancyByColor[PieceColor::BLACK.get_raw_value()] | position.m_enPassant;
			pawns = position.m_colorPieceMasks[PieceColor::WHITE.get_raw_value()][PieceType::PAWN.get_raw_value()];
			pawns = ((pawns & ~prohibitedFile) << dir) & enemyOccupancy;
		} else {
			shift = -dir;
			promotionRank = BoardConstants::RANK_1;
			enemyOccupancy = position.m_occupancyByColor[PieceColor::WHITE.get_raw_value()] | position.m_enPassant;
			pawns = position.m_colorPieceMasks[PieceColor::BLACK.get_raw_value()][PieceType::PAWN.get_raw_value()];
			pawns = ((pawns & ~prohibitedFile) >> dir) & enemyOccupancy;
		}

		pawns &= evasionMask;
		while (pawns != 0) {
			const Bitset piece = pawns.getLsb();
			pawns = pawns.popLsb();

			const Square from(piece.bitScan() - shift);
			const Square to(piece.bitScan());

			// Note: Special case to handle en passant after sliding
			Bitset slideRank = WallOperations::SLIDE_SQUARE[position.m_walls.bitScan()][to].asBitboard();

			if ((piece & promotionRank) != 0 || (slideRank & promotionRank) != 0) {
				moves.emplace_back(from, to, moves::MoveFlags::QUEEN_PROMOTION_CAPTURE);
				moves.emplace_back(from, to, moves::MoveFlags::ROOK_PROMOTION_CAPTURE);
				moves.emplace_back(from, to, moves::MoveFlags::KNIGHT_PROMOTION_CAPTURE);
				moves.emplace_back(from, to, moves::MoveFlags::BISHOP_PROMOTION_CAPTURE);
			} else {
				if ((piece & position.m_enPassant) != 0) {
					moves.emplace_back(from, to, moves::MoveFlags::EN_PASSANT);
				} else {
					moves.emplace_back(from, to, moves::MoveFlags::CAPTURE);
				}
			}
		}
	}
};

} //namespace phase4::engine::board::operators

#endif
