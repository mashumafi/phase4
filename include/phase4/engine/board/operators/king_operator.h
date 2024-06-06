#ifndef PHASE4_ENGINE_BOARD_OPERATORS_KING_OPERATOR_H
#define PHASE4_ENGINE_BOARD_OPERATORS_KING_OPERATOR_H

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/common/util.h>

#include <cstdint>
#include <tuple>

namespace phase4::engine::board::operators {

class KingOperator {
public:
	static void getLoudMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.colorToMove();
		const PieceColor enemyColor = color.invert();
		const Bitboard piece = position.colorPieceMask(color, PieceType::KING);

		if (unlikely(piece == 0)) {
			return; // NOTE: There should always be a king
		}

		const Square from(piece.fastBitScan());
		Bitboard availableMoves = moves::MovesGenerator::getKingMoves(from) & position.occupancy(enemyColor);

		while (availableMoves != 0) {
			const Bitboard field = availableMoves.getLsb(); // TODO: skip lsb
			const Square fieldIndex(field.fastBitScan());
			availableMoves = availableMoves.popLsb();

			moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
		}

		if (color == PieceColor::WHITE) {
			if (isWhiteKingCastlingAvailable(position, color)) {
				moves.emplace_back(Square::E1, Square::G1, moves::MoveFlags::KING_CASTLE);
			}

			if (isWhiteQueenCastlingAvailable(position, color)) {
				moves.emplace_back(Square::E1, Square::C1, moves::MoveFlags::QUEEN_CASTLE);
			}
		} else {
			if (isBlackKingCastlingAvailable(position, color)) {
				moves.emplace_back(Square::E8, Square::G8, moves::MoveFlags::KING_CASTLE);
			}

			if (isBlackQueenCastlingAvailable(position, color)) {
				moves.emplace_back(Square::E8, Square::C8, moves::MoveFlags::QUEEN_CASTLE);
			}
		}
	}

	static void getQuietMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.colorToMove();
		const Bitboard piece = position.colorPieceMask(color, PieceType::KING);

		if (unlikely(piece == 0)) {
			return; // NOTE: There should always be a king
		}

		const Square from(piece.fastBitScan());
		Bitboard availableMoves = moves::MovesGenerator::getKingMoves(from) & ~position.occupancySummary();

		while (availableMoves != 0) {
			const Bitboard field = availableMoves.getLsb(); // TODO: skip lsb
			const Square fieldIndex(field.fastBitScan());
			availableMoves = availableMoves.popLsb();

			moves.emplace_back(from, fieldIndex, moves::MoveFlags::QUIET);
		}
	}

	static void getAvailableCaptureMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.colorToMove();
		const PieceColor enemyColor = color.invert();
		const Bitboard piece = position.colorPieceMask(color, PieceType::KING);

		if (unlikely(piece == 0)) {
			return; // NOTE: There should always be a king
		}

		const Square from(piece.fastBitScan());
		Bitboard availableMoves = moves::MovesGenerator::getKingMoves(from) & position.occupancy(enemyColor);

		while (availableMoves != 0) {
			const Bitboard field = availableMoves.getLsb(); // TODO: skip lsb
			const Square fieldIndex(field.fastBitScan());
			availableMoves = availableMoves.popLsb();

			moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
		}
	}

	static bool isMoveLegal(const Position &position, moves::Move move) {
		using namespace common;

		const PieceColor enemyColor = position.colorToMove().invert();
		const Bitboard availableMoves = moves::MovesGenerator::getKingMoves(move.from());
		const Bitboard toField = move.to().asBitboard();

		if (move.flags().isSinglePush() && (availableMoves & toField) != 0 && (position.occupancySummary() & toField) == 0) {
			return true;
		}

		if (move.flags().isCapture() && (availableMoves & toField) != 0 && (position.occupancy(enemyColor) & toField) != 0) {
			return true;
		}

		if (move.flags().isKingCastling()) {
			if (position.colorToMove() == PieceColor::WHITE && isWhiteKingCastlingAvailable(position, position.colorToMove())) {
				return true;
			} else if (position.colorToMove() == PieceColor::BLACK && isBlackKingCastlingAvailable(position, position.colorToMove())) {
				return true;
			}
		}

		if (move.flags().isQueenCastling()) {
			if (position.colorToMove() == PieceColor::WHITE && isWhiteQueenCastlingAvailable(position, position.colorToMove())) {
				return true;
			} else if (position.colorToMove() == PieceColor::BLACK && isBlackQueenCastlingAvailable(position, position.colorToMove())) {
				return true;
			}
		}

		return false;
	}

private:
	static bool isWhiteKingCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitboard PASSING_FIELDS = Square::G1.asBitboard() | Square::F1.asBitboard();

		if (unlikely((position.castling() & common::Castling::WHITE_SHORT) != common::Castling::NONE && (position.occupancySummary() & PASSING_FIELDS) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::E1);
			return !position.isFieldAttacked(color, Square::G1) && !position.isFieldAttacked(color, Square::F1) && !isKingInCheck;
		}

		return false;
	}

	static bool isWhiteQueenCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitboard PASSING_FIELDS = Square::D1.asBitboard() | Square::C1.asBitboard();

		if (unlikely((position.castling() & Castling::WHITE_LONG) != Castling::NONE && (position.occupancySummary() & PASSING_FIELDS) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::E1);
			return !isKingInCheck && !position.isFieldAttacked(color, Square::D1) && !position.isFieldAttacked(color, Square::C1);
		}

		return false;
	}

	static bool isBlackKingCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitboard PASSING_FIELDS = Square::G8.asBitboard() | Square::F8.asBitboard();

		if (unlikely((position.castling() & Castling::BLACK_SHORT) != Castling::NONE && (position.occupancySummary() & PASSING_FIELDS) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::E8);
			return !position.isFieldAttacked(color, Square::G8) && !position.isFieldAttacked(color, Square::F8) && !isKingInCheck;
		}

		return false;
	}

	static bool isBlackQueenCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitboard PASSING_FIELDS = Square::D8.asBitboard() | Square::C8.asBitboard();

		if (unlikely((position.castling() & Castling::BLACK_LONG) != Castling::NONE && (position.occupancySummary() & PASSING_FIELDS) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::E8);
			return !isKingInCheck && !position.isFieldAttacked(color, Square::D8) && !position.isFieldAttacked(color, Square::C8);
		}

		return false;
	}
};

} //namespace phase4::engine::board::operators

#endif
