#ifndef PHASE4_ENGINE_BOARD_OPERATORS_PAWN_OPERATOR_H
#define PHASE4_ENGINE_BOARD_OPERATORS_PAWN_OPERATOR_H

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/position_constants.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/common/wall_operations.h>

#include <cstdint>

namespace phase4::engine::board::operators {

class PawnOperator {
public:
	static void getLoudMoves(const Position &position, moves::Moves &moves, common::Bitboard evasionMask) {
		using namespace common;

		const PieceColor color = position.colorToMove();

		getSinglePush(position, moves, true, evasionMask);
		getDiagonalAttacks(position, color == PieceColor::WHITE ? 9 : 7, PositionConstants::FILE_A, moves, evasionMask);
		getDiagonalAttacks(position, color == PieceColor::WHITE ? 7 : 9, PositionConstants::FILE_H, moves, evasionMask);
	}

	static void getQuietMoves(const Position &position, moves::Moves &moves, common::Bitboard evasionMask) {
		getSinglePush(position, moves, false, evasionMask);
		getDoublePush(position, moves, evasionMask);
	}

	static void getAvailableCaptureMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.colorToMove();

		getDiagonalAttacks(position, color == PieceColor::WHITE ? 9 : 7, PositionConstants::FILE_A, moves, Bitboard::MAX);
		getDiagonalAttacks(position, color == PieceColor::WHITE ? 7 : 9, PositionConstants::FILE_H, moves, Bitboard::MAX);
	}

	static bool isMoveLegal(const Position &position, moves::Move move) {
		using namespace common;

		const PieceColor enemyColor = position.colorToMove().invert();
		const Bitboard toField = move.to().asBitboard();

		if (!move.flags().isCapture()) {
			if (move.flags().isSinglePush() || move.flags().isPromotion()) {
				if ((position.occupancySummary() & toField) == 0) {
					return true;
				}
			} else if (move.flags().isDoublePush()) {
				const Bitboard middleField((move.from().middle(move.to()).asBitboard()));
				if ((position.occupancySummary() & middleField) == 0 && (position.occupancySummary() & toField) == 0) {
					return true;
				}
			}
		} else {
			if (move.flags().isEnPassant()) {
				if ((position.enPassant() & toField) != 0) {
					return true;
				}
			} else {
				int8_t difference = move.to().get_raw_value() - move.from().get_raw_value();
				int8_t colorDifference = -(position.colorToMove().get_raw_value() * 2 - 1) * difference;

				if ((position.occupancy(enemyColor) & toField) != 0 && (colorDifference == 7 || colorDifference == 9)) {
					return true;
				}
			}
		}

		return false;
	}

private:
	static void getSinglePush(const Position &position, moves::Moves &moves, bool promotionsMode, common::Bitboard evasionMask) {
		using namespace common;

		int8_t shift;
		Bitboard promotionRank, pawns;
		const PieceColor color = position.colorToMove();

		if (color == PieceColor::WHITE) {
			shift = 8;
			promotionRank = PositionConstants::RANK_8;
			pawns = position.colorPieceMask(PieceColor::WHITE, PieceType::PAWN);

			if (promotionsMode) {
				pawns &= PositionConstants::NEAR_PROMOTION_AREA_WHITE;
			} else {
				pawns &= ~PositionConstants::NEAR_PROMOTION_AREA_WHITE;
			}

			pawns = (pawns << 8) & ~position.occupancySummary();
		} else {
			shift = -8;
			promotionRank = PositionConstants::RANK_1;
			pawns = position.colorPieceMask(PieceColor::BLACK, PieceType::PAWN);

			if (promotionsMode) {
				pawns &= PositionConstants::NEAR_PROMOTION_AREA_BLACK;
			} else {
				pawns &= ~PositionConstants::NEAR_PROMOTION_AREA_BLACK;
			}

			pawns = (pawns >> 8) & ~position.occupancySummary();
		}

		pawns &= evasionMask;
		while (pawns != 0) {
			const Bitboard piece = pawns.getLsb(); // TODO: skip lsb
			pawns = pawns.popLsb();

			const Square from(piece.fastBitScan() - shift);
			const Square to(piece.fastBitScan());

			// Note: Special case to handle promotion after sliding
			Bitboard slideSquare = position.walls() > 0 ? (WallOperations::SLIDE_SQUARE[position.walls().fastBitScan()][to].asBitboard()) : 0;

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

	static void getDoublePush(const Position &position, moves::Moves &moves, common::Bitboard evasionMask) {
		using namespace common;

		int8_t shift;
		Bitboard startRank, pawns;
		const PieceColor color = position.colorToMove();

		if (color == PieceColor::WHITE) {
			shift = 16;
			startRank = PositionConstants::RANK_2;
			pawns = position.colorPieceMask(PieceColor::WHITE, PieceType::PAWN);
			pawns = ((pawns & startRank) << 8) & ~position.occupancySummary();
			pawns = (pawns << 8) & ~position.occupancySummary();
		} else {
			shift = -16;
			startRank = PositionConstants::RANK_7;
			pawns = position.colorPieceMask(PieceColor::BLACK, PieceType::PAWN);
			pawns = ((pawns & startRank) >> 8) & ~position.occupancySummary();
			pawns = (pawns >> 8) & ~position.occupancySummary();
		}

		pawns &= evasionMask;
		while (pawns != 0) {
			const Bitboard piece = pawns.getLsb(); // TODO: skip lsb
			pawns = pawns.popLsb();

			const Square from(piece.fastBitScan() - shift);
			const Square to(piece.fastBitScan());

			moves.emplace_back(from, to, moves::MoveFlags::DOUBLE_PUSH);
		}
	}

	static void getDiagonalAttacks(const Position &position, int8_t dir, common::Bitboard prohibitedFile, moves::Moves &moves, common::Bitboard evasionMask) {
		using namespace common;

		int8_t shift;
		Bitboard promotionRank, enemyOccupancy, pawns;
		const PieceColor color = position.colorToMove();

		if (color == PieceColor::WHITE) {
			shift = dir;
			promotionRank = PositionConstants::RANK_8;
			enemyOccupancy = position.occupancy(PieceColor::BLACK) | position.enPassant();
			pawns = position.colorPieceMask(PieceColor::WHITE, PieceType::PAWN);
			pawns = ((pawns & ~prohibitedFile) << dir) & enemyOccupancy;
		} else {
			shift = -dir;
			promotionRank = PositionConstants::RANK_1;
			enemyOccupancy = position.occupancy(PieceColor::WHITE) | position.enPassant();
			pawns = position.colorPieceMask(PieceColor::BLACK, PieceType::PAWN);
			pawns = ((pawns & ~prohibitedFile) >> dir) & enemyOccupancy;
		}

		pawns &= evasionMask;
		while (pawns != 0) {
			const Bitboard piece = pawns.getLsb(); // TODO: skip lsb
			pawns = pawns.popLsb();

			const Square from(piece.fastBitScan() - shift);
			const Square to(piece.fastBitScan());

			// Note: Special case to handle en passant after sliding
			Bitboard slideRank = position.walls() == 0 ? 0 : WallOperations::SLIDE_SQUARE[position.walls().fastBitScan()][to].asBitboard();

			if ((piece & promotionRank) != 0 || (slideRank & promotionRank) != 0) {
				moves.emplace_back(from, to, moves::MoveFlags::QUEEN_PROMOTION_CAPTURE);
				moves.emplace_back(from, to, moves::MoveFlags::ROOK_PROMOTION_CAPTURE);
				moves.emplace_back(from, to, moves::MoveFlags::KNIGHT_PROMOTION_CAPTURE);
				moves.emplace_back(from, to, moves::MoveFlags::BISHOP_PROMOTION_CAPTURE);
			} else {
				if ((piece & position.enPassant()) != 0) {
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
