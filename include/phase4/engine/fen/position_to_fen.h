#ifndef PHASE4_ENGINE_FEN_POSITION_TO_FEN_H
#define PHASE4_ENGINE_FEN_POSITION_TO_FEN_H

#include <phase4/engine/board/position.h>

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/math.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>

#include <cstdio>
#include <sstream>
#include <string>

namespace phase4::engine::fen {

class PositionToFen {
public:
	static std::string encode(const board::Position &position) noexcept {
		std::string fen;
		fen.reserve(100);
		encodePieces(fen, position);
		fen.push_back(' ');
		encodeColor(fen, position);
		fen.push_back(' ');
		encodeCastling(fen, position);
		fen.push_back(' ');
		encodeEnPassant(fen, position);
		fen.push_back(' ');
		encodeHalfmoveClock(fen, position);
		fen.push_back(' ');
		encodeFullmoveNumber(fen, position);

		return fen;
	}

private:
	static void encodePieces(std::string &fen, const board::Position &position) noexcept {
		using namespace common;

		for (int16_t rank = 7; rank >= 0; rank--) {
			uint8_t emptyFields = 0;

			for (int16_t file = 0; file < 8; file++) {
				const Square fieldIndex(FieldIndex(file, rank));

				const PieceType possibleWhitePiece = position.pieceTable(fieldIndex);
				const PieceType possibleBlackPiece = position.pieceTable(fieldIndex);

				const PieceColor color = std::invoke([&]() -> PieceColor {
					if ((position.occupancy(PieceColor::WHITE) & fieldIndex.asBitboard()) != 0) {
						return PieceColor::WHITE;
					} else if ((position.occupancy(PieceColor::BLACK) & fieldIndex.asBitboard()) != 0) {
						return PieceColor::BLACK;
					}
					return PieceColor::INVALID;
				});

				const PieceType piece = std::invoke([&]() -> PieceType {
					if (color == PieceColor::WHITE) {
						return possibleWhitePiece;
					} else if (color == PieceColor::BLACK) {
						return possibleBlackPiece;
					}
					return PieceType::INVALID;
				});

				const bool hasWall = (position.walls() & fieldIndex.asBitboard()) != 0;

				if (piece != PieceType::INVALID) {
					if (emptyFields != 0) {
						fen += std::to_string(emptyFields);
						emptyFields = 0;
					}

					fen.push_back(convertToPiece(piece, color));
				} else if (hasWall) {
					if (emptyFields != 0) {
						fen += std::to_string(emptyFields);
						emptyFields = 0;
					}

					fen.push_back('*');
				} else {
					emptyFields++;
				}
			}

			if (emptyFields != 0) {
				fen += std::to_string(emptyFields);
			}

			if (rank > 0) {
				fen.push_back('/');
			}
		}
	}

	static inline void encodeColor(std::string &fen, const board::Position &position) noexcept {
		fen.push_back(position.colorToMove() == common::PieceColor::WHITE ? 'w' : 'b');
	}

	static inline void encodeCastling(std::string &fen, const board::Position &position) noexcept {
		if (position.castling() == common::Castling::NONE) {
			fen.push_back('-');
			return;
		}

		if ((position.castling() & common::Castling::WHITE_SHORT) != common::Castling::NONE) {
			fen.push_back('K');
		}

		if ((position.castling() & common::Castling::WHITE_LONG) != common::Castling::NONE) {
			fen.push_back('Q');
		}

		if ((position.castling() & common::Castling::BLACK_SHORT) != common::Castling::NONE) {
			fen.push_back('k');
		}

		if ((position.castling() & common::Castling::BLACK_LONG) != common::Castling::NONE) {
			fen.push_back('q');
		}
	}

	static inline void encodeEnPassant(std::string &fen, const board::Position &position) noexcept {
		if (position.enPassant() == 0) {
			fen.push_back('-');
		}

		const uint8_t enPassantField = position.enPassant().bitScan(); // TODO: skip lsb
		const common::Square enPassantPosition(enPassantField);

		std::stringstream iss; // TODO: Avoid string stream
		iss << enPassantPosition;

		fen.append(iss.str());
	}

	static inline void encodeHalfmoveClock(std::string &fen, const board::Position &position) noexcept {
		std::array<char, 5> halfmoveClock;
		snprintf(halfmoveClock.data(), halfmoveClock.size(), "%zu", position.irreversibleMovesCount());
		fen.append(halfmoveClock.data());
	}

	static inline void encodeFullmoveNumber(std::string &fen, const board::Position &position) noexcept {
		std::array<char, 6> fullmoveClock;
		snprintf(fullmoveClock.data(), fullmoveClock.size(), "%d", common::Math::max_uint16(position.movesCount(), 1));
		fen.append(fullmoveClock.data());
	}

	static inline char convertToPiece(common::PieceType piece, common::PieceColor color) noexcept {
		using namespace common;

		const auto pieceAsChar = [](PieceType piece) -> char {
			switch (piece.get_raw_value()) {
				case PieceType::PAWN.get_raw_value():
					return 'P';
				case PieceType::ROOK.get_raw_value():
					return 'R';
				case PieceType::KNIGHT.get_raw_value():
					return 'N';
				case PieceType::BISHOP.get_raw_value():
					return 'B';
				case PieceType::QUEEN.get_raw_value():
					return 'Q';
				case PieceType::KING.get_raw_value():
					return 'K';
				default:
					return '?';
			}
		};

		return color == PieceColor::BLACK
				? static_cast<char>(std::tolower(static_cast<unsigned char>(pieceAsChar(piece))))
				: pieceAsChar(piece);
	}
};

} //namespace phase4::engine::fen

#endif
