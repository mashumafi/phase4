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

		PieceColor color = position.ColorToMove;

		GetSinglePush(position, moves, true, evasionMask);
		GetDiagonalAttacks(position, color == PieceColor::WHITE ? 9 : 7, BoardConstants::FILE_A, moves, evasionMask);
		GetDiagonalAttacks(position, color == PieceColor::WHITE ? 7 : 9, BoardConstants::FILE_H, moves, evasionMask);
	}

	static void getQuietMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		GetSinglePush(position, moves, false, evasionMask);
		GetDoublePush(position, moves, evasionMask);
	}

	static void getAvailableCaptureMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		PieceColor color = position.ColorToMove;

		GetDiagonalAttacks(position, color == PieceColor::WHITE ? 9 : 7, BoardConstants::FILE_A, moves, Bitset::MAX);
		GetDiagonalAttacks(position, color == PieceColor::WHITE ? 7 : 9, BoardConstants::FILE_H, moves, Bitset::MAX);
	}

	static bool isMoveLegal(const Position &position, moves::Move move) {
		using namespace common;

		const PieceColor enemyColor = position.ColorToMove.invert();
		const Bitset toField = move.to().asBitboard();

		if (!move.flags().isCapture()) {
			if (move.flags().isSinglePush() || move.flags().isPromotion()) {
				if ((position.OccupancySummary & toField) == 0) {
					return true;
				}
			} else if (move.flags().isDoublePush()) {
				const Square middleField(1ull << ((move.from() + move.to()) / 2));
				if ((position.OccupancySummary & middleField) == 0 && (position.OccupancySummary & toField) == 0) {
					return true;
				}
			}
		} else {
			if (move.flags().isEnPassant()) {
				if ((position.EnPassant & toField) != 0) {
					return true;
				}
			} else {
				int16_t difference = move.to() - move.from();
				int16_t colorDifference = -(position.ColorToMove.get_raw_value() * 2 - 1) * difference;

				if ((position.Occupancy[enemyColor.get_raw_value()] & toField) != 0 && (colorDifference == 7 || colorDifference == 9)) {
					return true;
				}
			}
		}

		return false;
	}

private:
	static void GetSinglePush(const Position &position, moves::Moves &moves, bool promotionsMode, common::Bitset evasionMask) {
		using namespace common;

		int shift;
		Bitset promotionRank, pawns;
		const PieceColor color = position.ColorToMove;

		if (color == PieceColor::WHITE) {
			shift = 8;
			promotionRank = BoardConstants::RANK_8;
			pawns = position.Pieces[PieceColor::WHITE.get_raw_value()][PieceType::PAWN.get_raw_value()];

			if (promotionsMode) {
				pawns &= BoardConstants::NEAR_PROMOTION_AREA_WHITE;
			} else {
				pawns &= ~BoardConstants::NEAR_PROMOTION_AREA_WHITE;
			}

			pawns = (pawns << 8) & ~position.OccupancySummary;
		} else {
			shift = -8;
			promotionRank = BoardConstants::RANK_1;
			pawns = position.Pieces[PieceColor::BLACK.get_raw_value()][PieceType::PAWN.get_raw_value()];

			if (promotionsMode) {
				pawns &= BoardConstants::NEAR_PROMOTION_AREA_BLACK;
			} else {
				pawns &= ~BoardConstants::NEAR_PROMOTION_AREA_BLACK;
			}

			pawns = (pawns >> 8) & ~position.OccupancySummary;
		}

		pawns &= evasionMask;
		while (pawns != 0) {
			const Bitset piece = pawns.getLsb();
			pawns = pawns.popLsb();

			const Square from(piece.bitScan() - shift);
			const Square to(piece.bitScan());

			// Note: Special case to handle promotion after sliding
			Bitset slideSquare = position.Walls > 0 ? (WallOperations::SLIDE_SQUARE[position.Walls.bitScan()][to].asBitboard()) : 0;

			if (promotionsMode && ((piece & promotionRank) != 0 || (slideSquare & promotionRank) != 0)) {
				moves.push_back(moves::Move(from, to, moves::MoveFlags::QUEEN_PROMOTION));
				moves.push_back(moves::Move(from, to, moves::MoveFlags::ROOK_PROMOTION));
				moves.push_back(moves::Move(from, to, moves::MoveFlags::KNIGHT_PROMOTION));
				moves.push_back(moves::Move(from, to, moves::MoveFlags::BISHOP_PROMOTION));
			} else {
				moves.push_back(moves::Move(from, to, moves::MoveFlags::QUIET));
			}
		}
	}

	static void GetDoublePush(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		int shift;
		Bitset startRank, pawns;
		const PieceColor color = position.ColorToMove;

		if (color == PieceColor::WHITE) {
			shift = 16;
			startRank = BoardConstants::RANK_2;
			pawns = position.Pieces[PieceColor::WHITE.get_raw_value()][PieceType::PAWN.get_raw_value()];
			pawns = ((pawns & startRank) << 8) & ~position.OccupancySummary;
			pawns = (pawns << 8) & ~position.OccupancySummary;
		} else {
			shift = -16;
			startRank = BoardConstants::RANK_7;
			pawns = position.Pieces[PieceColor::BLACK.get_raw_value()][PieceType::PAWN.get_raw_value()];
			pawns = ((pawns & startRank) >> 8) & ~position.OccupancySummary;
			pawns = (pawns >> 8) & ~position.OccupancySummary;
		}

		pawns &= evasionMask;
		while (pawns != 0) {
			const Bitset piece = pawns.getLsb();
			pawns = pawns.popLsb();

			const Square from(piece.bitScan() - shift);
			const Square to(piece.bitScan());

			moves.push_back(moves::Move(from, to, moves::MoveFlags::DOUBLE_PUSH));
		}
	}

	static void GetDiagonalAttacks(const Position &position, int dir, common::Bitset prohibitedFile, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		int shift;
		Bitset promotionRank, enemyOccupancy, pawns;
		const PieceColor color = position.ColorToMove;

		if (color == PieceColor::WHITE) {
			shift = dir;
			promotionRank = BoardConstants::RANK_8;
			enemyOccupancy = position.Occupancy[PieceColor::BLACK.get_raw_value()] | position.EnPassant;
			pawns = position.Pieces[PieceColor::WHITE.get_raw_value()][PieceType::PAWN.get_raw_value()];
			pawns = ((pawns & ~prohibitedFile) << dir) & enemyOccupancy;
		} else {
			shift = -dir;
			promotionRank = BoardConstants::RANK_1;
			enemyOccupancy = position.Occupancy[PieceColor::WHITE.get_raw_value()] | position.EnPassant;
			pawns = position.Pieces[PieceColor::BLACK.get_raw_value()][PieceType::PAWN.get_raw_value()];
			pawns = ((pawns & ~prohibitedFile) >> dir) & enemyOccupancy;
		}

		pawns &= evasionMask;
		while (pawns != 0) {
			const Bitset piece = pawns.getLsb();
			pawns = pawns.popLsb();

			const Square from(piece.bitScan() - shift);
			const Square to(piece.bitScan());

			// Note: Special case to handle en passant after sliding
			Bitset slideRank = WallOperations::SLIDE_SQUARE[position.Walls.bitScan()][to].asBitboard();

			if ((piece & promotionRank) != 0 || (slideRank & promotionRank) != 0) {
				moves.push_back(moves::Move(from, to, moves::MoveFlags::QUEEN_PROMOTION_CAPTURE));
				moves.push_back(moves::Move(from, to, moves::MoveFlags::ROOK_PROMOTION_CAPTURE));
				moves.push_back(moves::Move(from, to, moves::MoveFlags::KNIGHT_PROMOTION_CAPTURE));
				moves.push_back(moves::Move(from, to, moves::MoveFlags::BISHOP_PROMOTION_CAPTURE));
			} else {
				if ((piece & position.EnPassant) != 0) {
					moves.push_back(moves::Move(from, to, moves::MoveFlags::EN_PASSANT));
				} else {
					moves.push_back(moves::Move(from, to, moves::MoveFlags::CAPTURE));
				}
			}
		}
	}
};

} //namespace phase4::engine::board::operators

#endif
