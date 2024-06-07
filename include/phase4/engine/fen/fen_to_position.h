#ifndef PHASE4_ENGINE_FEN_FEN_TO_POSITION_H
#define PHASE4_ENGINE_FEN_FEN_TO_POSITION_H

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_state.h>
#include <phase4/engine/board/zobrist_hashing.h>

#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/common/wall_operations.h>

#include <cctype>
#include <charconv>
#include <optional>
#include <string_view>

namespace phase4::engine::fen {

class FenToPosition {
public:
	static std::string_view skipUntilAlphaNumeric(std::string_view s) noexcept {
		for (size_t i = 0; i < s.size(); ++i) {
			if (!std::isspace(s[i])) {
				return std::string_view(s.data() + i, s.size() - i);
			}
		}

		return "";
	}

	static std::string_view collectUntilSpace(std::string_view &s) noexcept {
		for (size_t i = 0; i < s.size(); ++i) {
			if (std::isspace(s[i])) {
				const std::string_view result(s.data(), i);
				s = std::string_view(s.data() + i, s.size() - i);
				return result;
			}
		}

		const std::string_view result = s;
		s = "";
		return result;
	}

	static std::optional<board::Position> parse(std::string_view fen) noexcept {
		using namespace common;

		fen = skipUntilAlphaNumeric(fen);
		const std::string_view pieces = collectUntilSpace(fen);

		fen = skipUntilAlphaNumeric(fen);
		const std::string_view sideToMove = collectUntilSpace(fen);

		fen = skipUntilAlphaNumeric(fen);
		const std::string_view castlingRights = collectUntilSpace(fen);

		fen = skipUntilAlphaNumeric(fen);
		const std::string_view enPassantSquare = collectUntilSpace(fen);

		fen = skipUntilAlphaNumeric(fen);
		const std::string_view halfmoveClock = collectUntilSpace(fen);

		uint16_t halfmoveClockNumber = 0;
		{
			const auto [_, ec] = std::from_chars(halfmoveClock.data(), halfmoveClock.data() + halfmoveClock.size(), halfmoveClockNumber);
			if (ec != std::errc())
				return {};
		}

		fen = skipUntilAlphaNumeric(fen);
		const std::string_view fullmove = collectUntilSpace(fen);

		fen = skipUntilAlphaNumeric(fen);
		if (!fen.empty()) {
			return {};
		}

		uint16_t fullmoveNumber = 0;
		{
			const auto [_, ec] = std::from_chars(fullmove.data(), fullmove.data() + fullmove.size(), fullmoveNumber);
			if (ec != std::errc())
				return {};
		}

		board::Position result;
		if (!parseSideToMove(sideToMove, result)) {
			return {};
		}

		if (!parsePieces(pieces, result)) {
			return {};
		}

		if (!parseCastlingRights(castlingRights, result)) {
			return {};
		}

		if (!parseEnPassantState(enPassantSquare, result)) {
			return {};
		}

		if (!validated(result)) {
			return {};
		}

		board::PositionState::recalculateEvaluationDependentValues(result);
		board::PositionState::calculatePieceTable(result);

		result.movesCount() = fullmoveNumber;
		result.irreversibleMovesCount() = halfmoveClockNumber;
		result.hash() = board::PositionState::calculateHash(result);
		result.pawnHash() = board::PositionState::calculatePawnHash(result);

		result.castlingDone(PieceColor::WHITE) = (result.castling() & Castling::WHITE_CASTLING) == Castling::NONE;
		result.castlingDone(PieceColor::BLACK) = (result.castling() & Castling::BLACK_CASTLING) == Castling::NONE;

		return result;
	}

private:
	static bool parsePieces(std::string_view pieces, board::Position &position) noexcept {
		common::FieldIndex field(0, 7);

		for (size_t i = 0; i < pieces.size(); ++i) {
			const char c = pieces[i];
			if (std::isalpha(c)) {
				const std::optional<common::PieceType> &piece = convertToPiece(c);
				if (!piece) {
					return false;
				}
				const common::PieceColor color = convertToColor(c);
				position.addPiece(color, *piece, common::Square(field));
				field += common::FieldIndex(1, 0);
			} else if (std::isdigit(c)) {
				// Numbers should be 0 to 8
				if (c < '0' || '8' < c) {
					return false;
				}
				field += common::FieldIndex(c - '0', 0);
			} else if (c == '*') {
				const common::Square square(field);
				if (position.walls() != 0) {
					// Walls should be adjacent
					if ((square.asBitboard() & position.walls()) == 0) {
						return false;
					}
				} else {
					position.walls() = common::WallOperations::SLIDE_FROM[square];
				}
				position.occupancySummary() |= square.asBitboard();
				field += common::FieldIndex(1, 0);
			} else if (c == '/') {
				// Ensure the end is reached
				if (field.x != 8) {
					return false;
				}
				field = common::FieldIndex(0, field.y - 1);
			}

			// Handle out of bounds
			if (field.x > 8 || field.y < 0) {
				return false;
			}
		}

		// Ensure the end is reached
		if (field.y != 0) {
			return false;
		}

		return true;
	}

	static bool parseSideToMove(std::string_view sideToMove, board::Position &position) noexcept {
		if (sideToMove.size() != 1) {
			return false;
		}
		switch (sideToMove[0]) {
			case 'w':
				position.colorToMove() = common::PieceColor::WHITE;
				return true;
			case 'b':
				position.colorToMove() = common::PieceColor::BLACK;
				return true;
			default:
				return false;
		}
	}

	static bool parseCastlingRights(std::string_view castlingRights, board::Position &position) {
		if (castlingRights.size() == 1 && castlingRights[0] == '-') {
			return true;
		}

		for (size_t i = 0; i < castlingRights.size(); ++i) {
			switch (castlingRights[i]) {
				case 'K':
					position.castling() |= common::Castling::WHITE_SHORT;
					break;
				case 'Q':
					position.castling() |= common::Castling::WHITE_LONG;
					break;
				case 'k':
					position.castling() |= common::Castling::BLACK_SHORT;
					break;
				case 'q':
					position.castling() |= common::Castling::BLACK_LONG;
					break;
				default:
					return false;
			}
		}

		return true;
	}

	static bool parseEnPassantState(std::string_view enPassantSquare, board::Position &position) noexcept {
		if (enPassantSquare != "-") {
			const std::optional<common::Square> &square = common::Square::parse(enPassantSquare);
			if (!square) {
				return false;
			}
			position.enPassant() = square->asBitboard();
		}

		return true;
	}

	static std::optional<common::PieceType> convertToPiece(char c) noexcept {
		switch (std::tolower(c)) {
			case 'p':
				return common::PieceType::PAWN;
			case 'r':
				return common::PieceType::ROOK;
			case 'n':
				return common::PieceType::KNIGHT;
			case 'b':
				return common::PieceType::BISHOP;
			case 'q':
				return common::PieceType::QUEEN;
			case 'k':
				return common::PieceType::KING;
		}

		return {};
	}

	static common::PieceColor convertToColor(char c) noexcept {
		return std::isupper(c) ? common::PieceColor::WHITE : common::PieceColor::BLACK;
	}

	static bool validated(board::Position &) {
		// TODO: Fix invalid castling flags
		// TODO: Fail if king is missing
		// TODO: Fail if king is in check
		// TODO: Check enpassant makes sense
		return true;
	}
};

} //namespace phase4::engine::fen

#endif
