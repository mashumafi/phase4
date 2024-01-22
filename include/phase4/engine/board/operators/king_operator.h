#ifndef PHASE4_ENGINE_BOARD_OPERATORS_KING_OPERATOR_H
#define PHASE4_ENGINE_BOARD_OPERATORS_KING_OPERATOR_H

#include <phase4/engine/ai/score/evaluation_constants.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/common/util.h>
#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>

#include <cstdint>
#include <tuple>

namespace phase4::engine::board::operators {

class KingOperator {
public:
	static void getLoudMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.ColorToMove;
		const PieceColor enemyColor = color.invert();
		const Bitset piece = position.Pieces[color.get_raw_value()][PieceType::KING.get_raw_value()];

		if (unlikely(piece == 0)) {
			return; // NOTE: There should always be a king
		}

		const Square from(piece.bitScan());
		Bitset availableMoves = moves::MovesGenerator::getKingMoves(from) & position.Occupancy[enemyColor.get_raw_value()];

		while (availableMoves != 0) {
			const Bitset field = availableMoves.getLsb();
			const Square fieldIndex(field.bitScan());
			availableMoves = availableMoves.popLsb();

			moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
		}

		if (color == PieceColor::WHITE) {
			if (IsWhiteKingCastlingAvailable(position, color)) {
				moves.emplace_back(Square::A4, Square::A2, moves::MoveFlags::KING_CASTLE);
			}

			if (IsWhiteQueenCastlingAvailable(position, color)) {
				moves.emplace_back(Square::A4, Square::A6, moves::MoveFlags::QUEEN_CASTLE);
			}
		} else {
			if (IsBlackKingCastlingAvailable(position, color)) {
				moves.emplace_back(Square::H4, Square::H2, moves::MoveFlags::KING_CASTLE);
			}

			if (IsBlackQueenCastlingAvailable(position, color)) {
				moves.emplace_back(Square::H4, Square::H6, moves::MoveFlags::QUEEN_CASTLE);
			}
		}
	}

	static void getQuietMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.ColorToMove;
		const Bitset piece = position.Pieces[color.get_raw_value()][PieceType::KING.get_raw_value()];

		if (unlikely(piece == 0)) {
			return; // NOTE: There should always be a king
		}

		const Square from(piece.bitScan());
		Bitset availableMoves = moves::MovesGenerator::getKingMoves(from) & ~position.OccupancySummary;

		while (availableMoves != 0) {
			const Bitset field = availableMoves.getLsb();
			const Square fieldIndex(field.bitScan());
			availableMoves = availableMoves.popLsb();

			moves.emplace_back(from, fieldIndex, moves::MoveFlags::QUIET);
		}
	}

	static void getAvailableCaptureMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.ColorToMove;
		const PieceColor enemyColor = color.invert();
		const Bitset piece = position.Pieces[color.get_raw_value()][PieceType::KING.get_raw_value()];

		if (unlikely(piece == 0)) {
			return; // NOTE: There should always be a king
		}

		const Square from(piece.bitScan());
		Bitset availableMoves = moves::MovesGenerator::getKingMoves(from) & position.Occupancy[enemyColor.get_raw_value()];

		while (availableMoves != 0) {
			const Bitset field = availableMoves.getLsb();
			const Square fieldIndex(field.bitScan());
			availableMoves = availableMoves.popLsb();

			moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
		}
	}

	static bool isMoveLegal(const Position &position, moves::Move move) {
		using namespace common;

		const PieceColor enemyColor = position.ColorToMove.invert();
		const Bitset availableMoves = moves::MovesGenerator::getKingMoves(move.from());
		const Bitset toField = move.to().asBitboard();

		if (move.flags().isSinglePush() && (availableMoves & toField) != 0 && (position.OccupancySummary & toField) == 0) {
			return true;
		}

		if (move.flags().isCapture() && (availableMoves & toField) != 0 && (position.Occupancy[enemyColor.get_raw_value()] & toField) != 0) {
			return true;
		}

		if (move.flags().isKingCastling()) {
			if (position.ColorToMove == PieceColor::WHITE && IsWhiteKingCastlingAvailable(position, position.ColorToMove)) {
				return true;
			} else if (position.ColorToMove == PieceColor::BLACK && IsBlackKingCastlingAvailable(position, position.ColorToMove)) {
				return true;
			}
		}

		if (move.flags().isQueenCastling()) {
			if (position.ColorToMove == PieceColor::WHITE && IsWhiteQueenCastlingAvailable(position, position.ColorToMove)) {
				return true;
			} else if (position.ColorToMove == PieceColor::BLACK && IsBlackQueenCastlingAvailable(position, position.ColorToMove)) {
				return true;
			}
		}

		return false;
	}

private:
	static bool IsWhiteKingCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitset PASSING = Square::A2.asBitboard() | Square::A3.asBitboard();

		if (unlikely((position.m_castling & common::Castling::WHITE_SHORT) != common::Castling::NONE && (position.OccupancySummary & PASSING) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::A4);
			return !position.isFieldAttacked(color, Square::A2) && !position.isFieldAttacked(color, Square::A3) && !isKingInCheck;
		}

		return false;
	}

	static bool IsWhiteQueenCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitset PASSING = Square::A5.asBitboard() | Square::A6.asBitboard();

		if (unlikely((position.m_castling & Castling::WHITE_LONG) != Castling::NONE && (position.OccupancySummary & PASSING) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::A4);
			return !isKingInCheck && !position.isFieldAttacked(color, Square::A5) && !position.isFieldAttacked(color, Square::A6);
		}

		return false;
	}

	static bool IsBlackKingCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitset PASSING = Square::H2.asBitboard() | Square::H3.asBitboard();

		if (unlikely((position.m_castling & Castling::BLACK_SHORT) != Castling::NONE && (position.OccupancySummary & PASSING) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::H4);
			return !position.isFieldAttacked(color, Square::H2) && !position.isFieldAttacked(color, Square::H3) && !isKingInCheck;
		}

		return false;
	}

	static bool IsBlackQueenCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitset PASSING = Square::H5.asBitboard() | Square::H6.asBitboard();

		if (unlikely((position.m_castling & Castling::BLACK_LONG) != Castling::NONE && (position.OccupancySummary & PASSING) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::H4);
			return !isKingInCheck && !position.isFieldAttacked(color, Square::H5) && !position.isFieldAttacked(color, Square::H6);
		}

		return false;
	}
};

} //namespace phase4::engine::board::operators

#endif
