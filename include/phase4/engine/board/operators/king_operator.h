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

		const PieceColor color = position.m_colorToMove;
		const PieceColor enemyColor = color.invert();
		const Bitset piece = position.m_colorPieceMasks[color.get_raw_value()][PieceType::KING.get_raw_value()];

		if (unlikely(piece == 0)) {
			return; // NOTE: There should always be a king
		}

		const Square from(piece.bitScan());
		Bitset availableMoves = moves::MovesGenerator::getKingMoves(from) & position.m_occupancyByColor[enemyColor.get_raw_value()];

		while (availableMoves != 0) {
			const Bitset field = availableMoves.getLsb();
			const Square fieldIndex(field.bitScan());
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

		const PieceColor color = position.m_colorToMove;
		const Bitset piece = position.m_colorPieceMasks[color.get_raw_value()][PieceType::KING.get_raw_value()];

		if (unlikely(piece == 0)) {
			return; // NOTE: There should always be a king
		}

		const Square from(piece.bitScan());
		Bitset availableMoves = moves::MovesGenerator::getKingMoves(from) & ~position.m_occupancySummary;

		while (availableMoves != 0) {
			const Bitset field = availableMoves.getLsb();
			const Square fieldIndex(field.bitScan());
			availableMoves = availableMoves.popLsb();

			moves.emplace_back(from, fieldIndex, moves::MoveFlags::QUIET);
		}
	}

	static void getAvailableCaptureMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.m_colorToMove;
		const PieceColor enemyColor = color.invert();
		const Bitset piece = position.m_colorPieceMasks[color.get_raw_value()][PieceType::KING.get_raw_value()];

		if (unlikely(piece == 0)) {
			return; // NOTE: There should always be a king
		}

		const Square from(piece.bitScan());
		Bitset availableMoves = moves::MovesGenerator::getKingMoves(from) & position.m_occupancyByColor[enemyColor.get_raw_value()];

		while (availableMoves != 0) {
			const Bitset field = availableMoves.getLsb();
			const Square fieldIndex(field.bitScan());
			availableMoves = availableMoves.popLsb();

			moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
		}
	}

	static bool isMoveLegal(const Position &position, moves::Move move) {
		using namespace common;

		const PieceColor enemyColor = position.m_colorToMove.invert();
		const Bitset availableMoves = moves::MovesGenerator::getKingMoves(move.from());
		const Bitset toField = move.to().asBitboard();

		if (move.flags().isSinglePush() && (availableMoves & toField) != 0 && (position.m_occupancySummary & toField) == 0) {
			return true;
		}

		if (move.flags().isCapture() && (availableMoves & toField) != 0 && (position.m_occupancyByColor[enemyColor.get_raw_value()] & toField) != 0) {
			return true;
		}

		if (move.flags().isKingCastling()) {
			if (position.m_colorToMove == PieceColor::WHITE && isWhiteKingCastlingAvailable(position, position.m_colorToMove)) {
				return true;
			} else if (position.m_colorToMove == PieceColor::BLACK && isBlackKingCastlingAvailable(position, position.m_colorToMove)) {
				return true;
			}
		}

		if (move.flags().isQueenCastling()) {
			if (position.m_colorToMove == PieceColor::WHITE && isWhiteQueenCastlingAvailable(position, position.m_colorToMove)) {
				return true;
			} else if (position.m_colorToMove == PieceColor::BLACK && isBlackQueenCastlingAvailable(position, position.m_colorToMove)) {
				return true;
			}
		}

		return false;
	}

private:
	static bool isWhiteKingCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitset PASSING_FIELDS = Square::G1.asBitboard() | Square::F1.asBitboard();

		if (unlikely((position.m_castling & common::Castling::WHITE_SHORT) != common::Castling::NONE && (position.m_occupancySummary & PASSING_FIELDS) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::E1);
			return !position.isFieldAttacked(color, Square::G1) && !position.isFieldAttacked(color, Square::F1) && !isKingInCheck;
		}

		return false;
	}

	static bool isWhiteQueenCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitset PASSING_FIELDS = Square::D1.asBitboard() | Square::C1.asBitboard();

		if (unlikely((position.m_castling & Castling::WHITE_LONG) != Castling::NONE && (position.m_occupancySummary & PASSING_FIELDS) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::E1);
			return !isKingInCheck && !position.isFieldAttacked(color, Square::D1) && !position.isFieldAttacked(color, Square::C1);
		}

		return false;
	}

	static bool isBlackKingCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitset PASSING_FIELDS = Square::G8.asBitboard() | Square::F8.asBitboard();

		if (unlikely((position.m_castling & Castling::BLACK_SHORT) != Castling::NONE && (position.m_occupancySummary & PASSING_FIELDS) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::E8);
			return !position.isFieldAttacked(color, Square::G8) && !position.isFieldAttacked(color, Square::F8) && !isKingInCheck;
		}

		return false;
	}

	static bool isBlackQueenCastlingAvailable(const Position &position, common::PieceColor color) {
		using namespace common;

		constexpr Bitset PASSING_FIELDS = Square::D8.asBitboard() | Square::C8.asBitboard();

		if (unlikely((position.m_castling & Castling::BLACK_LONG) != Castling::NONE && (position.m_occupancySummary & PASSING_FIELDS) == 0)) {
			const bool isKingInCheck = position.isFieldAttacked(color, Square::E8);
			return !isKingInCheck && !position.isFieldAttacked(color, Square::D8) && !position.isFieldAttacked(color, Square::C8);
		}

		return false;
	}
};

} //namespace phase4::engine::board::operators

#endif
