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
	static std::optional<board::Position> parse(std::string_view fen) {
		using namespace common;

		const size_t spacePos = fen.find(' ');
		if (spacePos == std::string_view::npos)
			return {};

		const std::string_view pieces = fen.substr(0, spacePos);

		fen.remove_prefix(spacePos + 1);

		size_t nextSpacePos = fen.find(' ');
		if (nextSpacePos == std::string_view::npos)
			return {};
		const std::string_view sideToMove = fen.substr(0, nextSpacePos);

		fen.remove_prefix(nextSpacePos + 1);

		nextSpacePos = fen.find(' ');
		if (nextSpacePos == std::string_view::npos)
			return {};
		const std::string_view castlingRights = fen.substr(0, nextSpacePos);

		// Move the position to the next character after the space
		fen.remove_prefix(nextSpacePos + 1);

		nextSpacePos = fen.find(' ');
		if (nextSpacePos == std::string_view::npos)
			return {};
		const std::string_view enPassantSquare = fen.substr(0, nextSpacePos);

		fen.remove_prefix(nextSpacePos + 1);

		nextSpacePos = fen.find(' ');
		if (nextSpacePos == std::string_view::npos)
			return {};
		const std::string_view halfmoveClock = fen.substr(0, nextSpacePos);

		uint16_t halfmoveClockNumber = 0;
		{
			const auto [_, ec] = std::from_chars(halfmoveClock.data(), halfmoveClock.data() + halfmoveClock.size(), halfmoveClockNumber);
			if (ec != std::errc())
				return {};
		}

		fen.remove_prefix(nextSpacePos + 1);

		const std::string_view fullmove = fen;

		uint16_t fullmoveNumber = 0;
		{
			const auto [_, ec] = std::from_chars(fullmove.data(), fullmove.data() + fullmove.size(), fullmoveNumber);
			if (ec != std::errc())
				return {};
		}

		board::Position result;
		const PieceColor currentColor = parseSideToMove(sideToMove);

		if (!parsePieces(pieces, result))
			return {};

		parseCastlingRights(castlingRights, result);
		parseEnPassantState(enPassantSquare, result);

		board::PositionState::recalculateEvaluationDependentValues(result);
		board::PositionState::calculatePieceTable(result);

		result.m_movesCount = fullmoveNumber;
		result.m_irreversibleMovesCount = halfmoveClockNumber;
		result.m_colorToMove = currentColor;
		result.m_hash = board::PositionState::calculateHash(result);
		result.m_pawnHash = board::PositionState::calculatePawnHash(result);

		result.m_castlingDone[PieceColor::WHITE.get_raw_value()] = (result.m_castling & Castling::WHITE_CASTLING) == Castling::NONE;
		result.m_castlingDone[PieceColor::BLACK.get_raw_value()] = (result.m_castling & Castling::BLACK_CASTLING) == Castling::NONE;

		return result;
	}

private:
	static bool parsePieces(std::string_view pieces, board::Position &position) {
		common::FieldIndex field(0, 7);

		for (size_t i = 0; i < pieces.size(); ++i) {
			const char c = pieces[i];
			if (std::isalpha(c)) {
				const std::optional<common::PieceType> &piece = convertToPiece(c);
				if (!piece)
					return false;
				const common::PieceColor color = convertToColor(c);
				position.addPiece(color, *piece, common::Square(field));
				field += common::FieldIndex(1, 0);
			} else if (std::isdigit(c)) {
				field += common::FieldIndex(c - '0', 0);
			} else if (c == '*') {
				const common::Square square(field);
				if (position.m_walls != 0) {
					assert((square.asBitboard() & position.m_walls) != 0);
				} else {
					position.m_walls = common::WallOperations::SLIDE_FROM[square];
				}
				position.m_occupancySummary |= square.asBitboard();
				field += common::FieldIndex(1, 0);
			} else if (c == '/') {
				field = common::FieldIndex(0, field.y - 1);
			}
		}

		return true;
	}

	static common::PieceColor parseSideToMove(std::string_view sideToMove) {
		return sideToMove == "w" ? common::PieceColor::WHITE : common::PieceColor::BLACK;
	}

	static void parseCastlingRights(std::string_view castlingRights, board::Position &position) {
		if (castlingRights.find('K') != std::string_view::npos) {
			position.m_castling |= common::Castling::WHITE_SHORT;
		}

		if (castlingRights.find('Q') != std::string_view::npos) {
			position.m_castling |= common::Castling::WHITE_LONG;
		}

		if (castlingRights.find('k') != std::string_view::npos) {
			position.m_castling |= common::Castling::BLACK_SHORT;
		}

		if (castlingRights.find('q') != std::string_view::npos) {
			position.m_castling |= common::Castling::BLACK_LONG;
		}
	}

	static void parseEnPassantState(std::string_view enPassantSquare, board::Position &position) {
		if (enPassantSquare != "-") {
			common::Square square(enPassantSquare);
			position.m_enPassant = square.asBitboard();
		}
	}

	static std::optional<common::PieceType> convertToPiece(char c) {
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

	static common::PieceColor convertToColor(char c) {
		return std::isupper(c) ? common::PieceColor::WHITE : common::PieceColor::BLACK;
	}
};

} //namespace phase4::engine::fen

#endif
