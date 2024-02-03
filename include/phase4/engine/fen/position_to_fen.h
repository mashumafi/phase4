#ifndef PHASE4_ENGINE_FEN_POSITION_TO_FEN_H
#define PHASE4_ENGINE_FEN_POSITION_TO_FEN_H

#include <phase4/engine/board/position.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/math.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>

#include <sstream>
#include <string>

namespace phase4::engine::fen {

class PositionToFen {
public:
	static std::string encode(const board::Position &position) {
		return encodePieces(position) + " " +
				encodeColor(position) + " " +
				encodeCastling(position) + " " +
				encodeEnPassant(position) + " " +
				encodeHalfmoveClock(position) + " " +
				encodeFullmoveNumber(position);
	}

private:
	static std::string encodePieces(const board::Position &position) {
		using namespace common;

		std::string resultBuilder;
		resultBuilder.reserve(9 * 8);
		std::string rankBuilder;
		rankBuilder.reserve(9);

		for (int32_t rank = 7; rank >= 0; rank--) {
			uint8_t emptyFields = 0;

			for (int32_t file = 0; file < 8; file++) {
				const Square fieldIndex(FieldIndex(file, rank));

				const PieceType possibleWhitePiece = position.m_pieceTable[fieldIndex];
				const PieceType possibleBlackPiece = position.m_pieceTable[fieldIndex];

				const PieceColor color = std::invoke([&]() -> PieceColor {
					if ((position.m_occupancyByColor[PieceColor::WHITE.get_raw_value()] & fieldIndex.asBitboard()) != 0) {
						return PieceColor::WHITE;
					} else if ((position.m_occupancyByColor[PieceColor::BLACK.get_raw_value()] & fieldIndex.asBitboard()) != 0) {
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

				const bool hasWall = (position.m_walls & fieldIndex.asBitboard()) != 0;

				if (piece != PieceType::INVALID) {
					if (emptyFields != 0) {
						rankBuilder += std::to_string(emptyFields);
						emptyFields = 0;
					}

					rankBuilder.push_back(convertToPiece(piece, color));
				} else if (hasWall) {
					if (emptyFields != 0) {
						rankBuilder += std::to_string(emptyFields);
						emptyFields = 0;
					}

					rankBuilder.push_back('*');
				} else {
					emptyFields++;
				}
			}

			if (emptyFields != 0) {
				rankBuilder += std::to_string(emptyFields);
			}

			resultBuilder += rankBuilder;
			rankBuilder.clear();

			if (rank > 0) {
				resultBuilder.push_back('/');
			}
		}

		return resultBuilder;
	}

	static std::string encodeColor(const board::Position &position) {
		return position.m_colorToMove == common::PieceColor::WHITE ? "w" : "b";
	}

	static std::string encodeCastling(const board::Position &position) {
		std::string resultBuilder;
		resultBuilder.reserve(4);

		if ((position.m_castling & common::Castling::WHITE_SHORT) != common::Castling::NONE) {
			resultBuilder += "K";
		}

		if ((position.m_castling & common::Castling::WHITE_LONG) != common::Castling::NONE) {
			resultBuilder += "Q";
		}

		if ((position.m_castling & common::Castling::BLACK_SHORT) != common::Castling::NONE) {
			resultBuilder += "k";
		}

		if ((position.m_castling & common::Castling::BLACK_LONG) != common::Castling::NONE) {
			resultBuilder += "q";
		}

		if (resultBuilder.size() == 0) {
			resultBuilder += "-";
		}

		return resultBuilder;
	}

	static std::string encodeEnPassant(const board::Position &position) {
		if (position.m_enPassant == 0) {
			return "-";
		}

		const uint8_t enPassantField = position.m_enPassant.bitScan(); // TODO: skip lsb
		const common::Square enPassantPosition(enPassantField);

		std::stringstream iss;
		iss << enPassantPosition;

		return iss.str();
	}

	static std::string encodeHalfmoveClock(const board::Position &position) {
		return std::to_string(position.m_irreversibleMovesCount);
	}

	static std::string encodeFullmoveNumber(const board::Position &position) {
		return std::to_string(common::Math::max_uint16(position.m_movesCount, 1));
	}

	static char convertToPiece(common::PieceType piece, common::PieceColor color) {
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
				? std::tolower(pieceAsChar(piece))
				: pieceAsChar(piece);
	}
};

} //namespace phase4::engine::fen

#endif
