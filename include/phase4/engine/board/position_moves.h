#ifndef PHASE4_ENGINE_BOARD_POSITION_MOVES_H
#define PHASE4_ENGINE_BOARD_POSITION_MOVES_H

#include <phase4/engine/board/operators.h>
#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/result.h>

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/fast_vector.h>
#include <phase4/engine/common/game_phase.h>
#include <phase4/engine/common/math.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/wall_operations.h>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <optional>
#include <string_view>

namespace phase4::engine::board {

constexpr size_t NUM_PREFIX_SIZE = 5; // Moves have a number prefix, enough for 99999 moves
constexpr size_t NUM_SEPERATOR_SIZE = 2; // Side for the ". " after the number
constexpr size_t UTF_PIECE_SIZE = 3; // UTF for chess pieces take 3 bytes
constexpr size_t ASCII_COUNT = 6; // There should only be 6 characters max
constexpr size_t LONGEST_ALGEBRAIC_NOTATION = NUM_PREFIX_SIZE + NUM_SEPERATOR_SIZE + UTF_PIECE_SIZE + ASCII_COUNT + 1;
using AlgebraicNotation = std::array<char, LONGEST_ALGEBRAIC_NOTATION>;

inline std::string_view SV(const AlgebraicNotation &notation) {
	return notation.data();
}

class PositionMoves {
public:
	static inline void processRookCastling(Position &position, common::Square square) {
		using namespace common;

		switch (square.get_raw_value()) {
			case Square::H1.get_raw_value():
				position.hash() = position.hash().removeCastlingFlag(position.castling(), Castling::WHITE_SHORT);
				position.castling() &= ~Castling::WHITE_SHORT;
				break;
			case Square::A1.get_raw_value():
				position.hash() = position.hash().removeCastlingFlag(position.castling(), Castling::WHITE_LONG);
				position.castling() &= ~Castling::WHITE_LONG;
				break;
			case Square::H8.get_raw_value():
				position.hash() = position.hash().removeCastlingFlag(position.castling(), Castling::BLACK_SHORT);
				position.castling() &= ~Castling::BLACK_SHORT;
				break;
			case Square::A8.get_raw_value():
				position.hash() = position.hash().removeCastlingFlag(position.castling(), Castling::BLACK_LONG);
				position.castling() &= ~Castling::BLACK_LONG;
				break;
		}
	}

	static inline void processKingCastling(Position &position, common::PieceColor color) {
		using namespace common;

		if (color == PieceColor::WHITE) {
			position.hash() = position.hash().removeCastlingFlag(position.castling(), Castling::WHITE_SHORT);
			position.hash() = position.hash().removeCastlingFlag(position.castling(), Castling::WHITE_LONG);
			position.castling() &= ~Castling::WHITE_CASTLING;
		} else {
			position.hash() = position.hash().removeCastlingFlag(position.castling(), Castling::BLACK_SHORT);
			position.hash() = position.hash().removeCastlingFlag(position.castling(), Castling::BLACK_LONG);
			position.castling() &= ~Castling::BLACK_CASTLING;
		}
	}

	static inline moves::Result makeMove(Position &position, moves::Move move) {
		using namespace common;

		moves::Result result;

		const PieceType pieceType = position.pieceTable(move.from());
		const PieceColor enemyColor = position.colorToMove().invert();

		if (position.colorToMove() == PieceColor::BLACK) {
			++position.movesCount();
		}

		if (pieceType == PieceType::PAWN || move.flags().isCapture() || move.flags().isCastling()) {
			position.irreversibleMovesCount() = 0;
		} else {
			position.irreversibleMovesCount()++;
		}

		if (position.enPassant() != 0) {
			const uint8_t enPassantRank = position.enPassant().fastBitScan() % 8;
			position.hash() = position.hash().toggleEnPassant(enPassantRank);
			position.enPassant() = 0;
		}

		if (move.flags().isSinglePush()) {
			position.movePiece(position.colorToMove(), pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			position.hash() = position.hash().movePiece(position.colorToMove(), pieceType, move.from(), move.to());

			if (pieceType == PieceType::PAWN) {
				position.pawnHash() = position.pawnHash().movePiece(position.colorToMove(), pieceType, move.from(), move.to());
			}
		} else if (move.flags().isDoublePush()) {
			position.movePiece(position.colorToMove(), pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			position.hash() = position.hash().movePiece(position.colorToMove(), pieceType, move.from(), move.to());
			position.pawnHash() = position.pawnHash().movePiece(position.colorToMove(), pieceType, move.from(), move.to());

			const Bitboard enPassantField = (position.colorToMove() == PieceColor::WHITE) ? move.to().south(1).asBitboard() : move.to().north(1).asBitboard();
			const uint8_t enPassantFieldIndex = enPassantField.fastBitScan();

			position.enPassant() = enPassantField;
			position.hash() = position.hash().toggleEnPassant(enPassantFieldIndex % 8);
		} else if (move.flags().isEnPassant()) {
			const Square enemyPieceField((position.colorToMove() == PieceColor::WHITE) ? move.to().south(1) : move.to().north(1));
			const PieceType killedPiece = position.pieceTable(enemyPieceField);

			position.removePiece(enemyColor, killedPiece, enemyPieceField);
			result.removed.push_back({ killedPiece, enemyPieceField });
			position.hash() = position.hash().addOrRemovePiece(enemyColor, killedPiece, enemyPieceField);
			position.pawnHash() = position.pawnHash().addOrRemovePiece(enemyColor, killedPiece, enemyPieceField);

			position.movePiece(position.colorToMove(), pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			position.hash() = position.hash().movePiece(position.colorToMove(), pieceType, move.from(), move.to());
			position.pawnHash() = position.pawnHash().movePiece(position.colorToMove(), pieceType, move.from(), move.to());

			result.killed = moves::Result::PieceAndSquare{ killedPiece, enemyPieceField };
		} else if (move.flags().isCapture()) {
			PieceType killedPiece = position.pieceTable(move.to());

			position.removePiece(enemyColor, killedPiece, move.to());
			result.removed.push_back({ killedPiece, move.to() });
			position.hash() = position.hash().addOrRemovePiece(enemyColor, killedPiece, move.to());

			if (killedPiece == PieceType::PAWN) {
				position.pawnHash() = position.pawnHash().addOrRemovePiece(enemyColor, killedPiece, move.to());
			} else if (killedPiece == PieceType::ROOK) {
				processRookCastling(position, move.to());
			}

			if (move.flags().isPromotion()) {
				PieceType promotionPiece = move.flags().getPromotionPiece();

				position.removePiece(position.colorToMove(), pieceType, move.from());
				result.removed.push_back({ pieceType, move.from() });
				position.hash() = position.hash().addOrRemovePiece(position.colorToMove(), pieceType, move.from());
				position.pawnHash() = position.pawnHash().addOrRemovePiece(position.colorToMove(), pieceType, move.from());

				position.addPiece(position.colorToMove(), promotionPiece, move.to());
				result.added = { promotionPiece, move.to() };
				position.hash() = position.hash().addOrRemovePiece(position.colorToMove(), promotionPiece, move.to());

				result.promotion = promotionPiece;
			} else {
				position.movePiece(position.colorToMove(), pieceType, move.from(), move.to());
				result.moved.push_back({ move.from(), move.to() });
				position.hash() = position.hash().movePiece(position.colorToMove(), pieceType, move.from(), move.to());

				if (pieceType == PieceType::PAWN) {
					position.pawnHash() = position.pawnHash().movePiece(position.colorToMove(), pieceType, move.from(), move.to());
				}
			}

			result.killed = moves::Result::PieceAndSquare{ killedPiece, move.to() };
		} else if (move.flags().isCastling()) {
			if (move.flags().isKingCastling()) { // Short/King castling
				if (position.colorToMove() == PieceColor::WHITE) {
					position.movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::G1);
					position.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::H1, Square::F1);

					result.moved.push_back({ Square::E1, Square::G1 });
					result.moved.push_back({ Square::H1, Square::F1 });

					position.hash() = position.hash().movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::G1);
					position.hash() = position.hash().movePiece(PieceColor::WHITE, PieceType::ROOK, Square::H1, Square::F1);
				} else {
					position.movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::G8);
					position.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::H8, Square::F8);

					result.moved.push_back({ Square::E8, Square::G8 });
					result.moved.push_back({ Square::H8, Square::F8 });

					position.hash() = position.hash().movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::G8);
					position.hash() = position.hash().movePiece(PieceColor::BLACK, PieceType::ROOK, Square::H8, Square::F8);
				}
			} else { // Long/Queen castling
				if (position.colorToMove() == PieceColor::WHITE) {
					position.movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::C1);
					position.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::A1, Square::D1);

					result.moved.push_back({ Square::E1, Square::C1 });
					result.moved.push_back({ Square::A1, Square::D1 });

					position.hash() = position.hash().movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::C1);
					position.hash() = position.hash().movePiece(PieceColor::WHITE, PieceType::ROOK, Square::A1, Square::D1);
				} else {
					position.movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::C8);
					position.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::A8, Square::D8);

					result.moved.push_back({ Square::E8, Square::C8 });
					result.moved.push_back({ Square::A8, Square::D8 });

					position.hash() = position.hash().movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::C8);
					position.hash() = position.hash().movePiece(PieceColor::BLACK, PieceType::ROOK, Square::A8, Square::D8);
				}
			}

			processKingCastling(position, position.colorToMove());

			position.castlingDone(position.colorToMove()) = true;
		} else if (move.flags().isPromotion()) {
			PieceType promotionPiece = move.flags().getPromotionPiece();

			position.removePiece(position.colorToMove(), pieceType, move.from());
			result.removed.push_back({ pieceType, move.from() });
			position.hash() = position.hash().addOrRemovePiece(position.colorToMove(), pieceType, move.from());
			position.pawnHash() = position.pawnHash().addOrRemovePiece(position.colorToMove(), pieceType, move.from());

			position.addPiece(position.colorToMove(), promotionPiece, move.to());
			result.added = { promotionPiece, move.to() };
			position.hash() = position.hash().addOrRemovePiece(position.colorToMove(), promotionPiece, move.to());

			result.promotion = promotionPiece;
		}

		if (pieceType == PieceType::KING && !move.flags().isCastling()) {
			processKingCastling(position, position.colorToMove());
		} else if (pieceType == PieceType::ROOK && position.castling() != Castling::NONE) {
			processRookCastling(position, move.from());
		}

		if (likely(position.walls() > 0)) {
			const uint8_t wallIndex = position.walls().fastBitScan();
			const FieldIndex wallMove = WallOperations::SLIDE_DIR[wallIndex][move.to()];
			if (wallMove != FieldIndex::ZERO) {
				position.occupancySummary() = position.occupancySummary() & ~position.walls();

				Bitboard original = WallOperations::SLIDE_TO[wallIndex][move.to()];
				while (original > 0) {
					const Square from(original.fastBitScan());
					const Square to = WallOperations::SLIDE_SQUARE[wallIndex][from];
					if (auto pieceResult = position.getPiece(from)) {
						auto [resultColor, resultType] = *pieceResult;
						position.movePiece(resultColor, resultType, from, to);
						position.hash() = position.hash().movePiece(resultColor, resultType, from, to);
						if (resultType == PieceType::PAWN) {
							position.pawnHash() = position.pawnHash().movePiece(resultColor, resultType, from, to);
						}

						if (resultType == PieceType::PAWN && position.enPassant() != 0) {
							const int8_t offset = (resultColor == PieceColor::WHITE) ? -8 : 8;
							const uint8_t enPassantField = position.enPassant().fastBitScan();
							if (enPassantField == from.get_raw_value() + offset) {
								const uint8_t enPassantRank = enPassantField % 8;
								position.hash() = position.hash().toggleEnPassant(enPassantRank);

								if (wallMove.offset() >= 0)
									position.enPassant() = position.enPassant() >> wallMove.offset();
								else
									position.enPassant() = position.enPassant() << -wallMove.offset();
								position.hash() = position.hash().toggleEnPassant(position.enPassant().fastBitScan() % 8);
							}
						}

						if (resultType == PieceType::KING) {
							processKingCastling(position, resultColor);
						} else if (resultType == PieceType::ROOK) {
							processRookCastling(position, from);
						}
					}
					original = original.popLsb();
				}

				position.hash() = position.hash().toggleWalls(position.walls()); // Turn off previous wall
				position.walls() = WallOperations::SLIDE_TO[wallIndex][move.to()];
				position.hash() = position.hash().toggleWalls(position.walls()); // Turn on new wall location
				position.occupancySummary() = position.occupancySummary() | position.walls();
			}
			result.slide = wallMove;
		} else {
			result.slide = common::FieldIndex::ZERO;
		}

		position.colorToMove() = enemyColor;
		position.hash() = position.hash().changeSide();

		return result;
	}

	struct SlideResult {
		struct Movement {
			common::Square from;
			common::Square to;
		};
		common::FastVector<Movement, 4> moves;
	};

	/// @brief Utility for sliding the wall. This is for 2 use-cases:
	///        * Undo a Move, which handles updating the hash
	///        * Initial selection of square to remove (no hash computation needed)
	///        This function should never move a piece unless the caller recomputes hashes.
	/// @param wallMove Direction to move the wall should be magitude of 2
	/// @return
	static inline SlideResult slideWall(Position &position, common::FieldIndex wallMove) {
		using namespace common;

		if (unlikely(position.walls().fastCount() != 4))
			return {};

		SlideResult result;

		position.occupancySummary() &= ~position.walls();

		Bitboard walls = position.walls();
		if (wallMove.offset() >= 0)
			walls <<= wallMove.offset();
		else
			walls >>= -wallMove.offset();
		assert(walls > 0);

		Bitboard original = walls;
		while (original != 0) {
			const Square from(original.fastBitScan());
			const Square to(from - wallMove.offset());
			if (auto pieceResult = position.getPiece(from)) {
				const auto [resultColor, resultType] = *pieceResult;
				result.moves.push_back({ from, to });
				position.movePiece(resultColor, resultType, from, to);

				// Update castling
				if (resultType == PieceType::KING) {
					processKingCastling(position, resultColor);
				} else {
					processRookCastling(position, from);
				}

				// Update en passant
				if (position.enPassant() != 0) {
					const uint8_t enPassantRank = position.enPassant().fastBitScan() % 8;
					position.hash() = position.hash().toggleEnPassant(enPassantRank);
					position.enPassant() = 0;
				}
			}

			original = original.popLsb();
		}

		position.hash() = position.hash().toggleWalls(position.walls()); // Turn off previous wall
		position.walls() = walls;
		position.hash() = position.hash().toggleWalls(position.walls()); // Turn on new wall location
		position.occupancySummary() |= position.walls();

		return result;
	}

	static inline std::optional<moves::Move> findRealMove(moves::Moves moves, moves::Move move) {
		for (size_t i = 0; i < moves.size(); ++i) {
			const bool correctFlags = moves[i].flags().getPromotionPiece() == move.flags().getPromotionPiece();
			if (moves[i].from() == move.from() && moves[i].to() == move.to() && correctFlags) {
				return moves[i];
			}
		}

		return std::nullopt;
	};

	/// @brief Based on the list of moves, determines if adding a promotion flag would make it valid
	/// @param moves List of moves to check
	/// @param move The desired move
	/// @return True if promotion flag exists on the move or if the move exists in the list of moves and promotion is there instead
	static inline bool isPromotionFlagMissing(const moves::Moves &moves, moves::Move move) {
		if (move.flags().isPromotion()) {
			return true;
		}

		for (size_t i = 0; i < moves.size(); ++i) {
			if (moves[i].from() == move.from() && moves[i].to() == move.to() && moves[i].flags().isPromotion()) {
				return true;
			}
		}
		return false;
	}

	static inline std::optional<moves::Move> findRealMove(const board::Position &position, moves::Move move) {
		moves::Moves moves;
		board::Operators::getAllMoves(position, moves);
		return findRealMove(moves, move);
	};

	/// @brief makes a series of moves from a position, move squares are validated but flags can be quiet or promotion
	/// @param position starting position to make moves
	/// @param moves the moves to make, only the squares and promotion type need to be correct
	/// @return the resulting position after making the moves
	static std::optional<board::Position> makeMoves(const board::Position &position, const moves::Moves &moves) {
		board::Position mutablePosition = position;
		for (size_t i = 0; i < moves.size(); ++i) {
			const std::optional<moves::Move> move = findRealMove(mutablePosition, moves[i]);
			if (!move) {
				return std::nullopt;
			}
			board::PositionMoves::makeMove(mutablePosition, *move);
		}

		return mutablePosition;
	}

	static void getValidMoves(const board::Position &position, moves::Moves &moves) {
		moves::Moves allMoves;
		Operators::getAllMoves(position, allMoves);

		for (size_t i = 0; i < allMoves.size(); ++i) {
			board::Position positionCopy = position;
			board::PositionMoves::makeMove(positionCopy, allMoves[i]);
			if (!positionCopy.isKingChecked(position.colorToMove())) {
				moves.push_back(allMoves[i]);
			}
		}
	}

	static AlgebraicNotation algebraicNotation(const Position &position, moves::Move move) noexcept {
		using namespace common;
		using namespace moves;

		AlgebraicNotation result = { 0 };

		auto realMove = findRealMove(position, move);
		if (!realMove) {
			return result;
		}

		size_t offset = 0;

		if (position.colorToMove() == PieceColor::WHITE) {
			const uint16_t moveNumber = position.movesCount();
			offset += snprintf(result.data() + offset, result.size() + offset, "%d. ", moveNumber);
			assert(offset < result.size());
		}

		if (realMove->flags().isKingCastling()) {
			offset += snprintf(result.data() + offset, result.size() + offset, "O-O");
			assert(offset < result.size());
		} else if (realMove->flags().isQueenCastling()) {
			offset += snprintf(result.data() + offset, result.size() + offset, "O-O-O");
			assert(offset < result.size());
		} else {
			const PieceType pieceType = position.pieceTable(realMove->from());
			if (pieceType == PieceType::PAWN) {
				if (realMove->flags() == MoveFlags::EN_PASSANT || realMove->flags().isCapture()) {
					const std::array<char, 3> fromBuffer = realMove->from().asBuffer();
					offset += snprintf(result.data() + offset, result.size() - offset, "%c", fromBuffer[0]);
					assert(offset < result.size());
				}
			} else {
				offset += snprintf(result.data() + offset, result.size() - offset, "%s", pieceType.toSymbol(position.colorToMove()));
				assert(offset < result.size());

				// Disambiguate move
				Bitboard colorPieceMask = position.colorPieceMask(position.colorToMove(), pieceType);
				bool disambiguateRank = false;
				bool disambiguateFile = false;

				//
				while (colorPieceMask > 0) {
					Square square(colorPieceMask);
					colorPieceMask = colorPieceMask.popLsb();
					if (square == move.from()) {
						continue;
					}

					Moves moves;
					PositionMoves::getValidMoves(position, moves);
					for (size_t i = 0; i < moves.size(); ++i) {
						const Move ambiguousMove = moves[i];
						if (ambiguousMove.from() == square && ambiguousMove.to() == move.to()) {
							Bitboard ambiguousSquare(ambiguousMove.from().asBitboard());
							if ((moves::patterns::RankPatternGenerator::getPatternForField(move.from()) & ambiguousSquare) != 0) {
								disambiguateRank = true;
							}
							if ((moves::patterns::FilePatternGenerator::getPatternForField(move.from()) & ambiguousSquare) != 0) {
								disambiguateFile = true;
							}
						}
					}
				}

				auto buffer = move.from().asBuffer();
				if (disambiguateRank) {
					result[offset++] = buffer[0];
					assert(offset < result.size());
				}
				if (disambiguateFile) {
					result[offset++] = buffer[1];
					assert(offset < result.size());
				}
			}

			if (realMove->flags() == MoveFlags::EN_PASSANT || realMove->flags().isCapture()) {
				result[offset++] = L'x';
				assert(offset < result.size());
			}

			const std::array<char, 3> toBuffer = realMove->to().asBuffer();
			offset += snprintf(result.data() + offset, result.size() - offset, "%s", toBuffer.data());
			assert(offset < result.size());

			if (realMove->flags().isPromotion()) {
				result[offset++] = L'=';
				assert(offset < result.size());
				offset += snprintf(result.data() + offset, result.size() - offset, "%s", realMove->flags().getPromotionPiece().toSymbol(position.colorToMove()));
				assert(offset < result.size());
			}
		}

		Position positionCopy = position;
		PositionMoves::makeMove(positionCopy, *realMove);
		if (positionCopy.isKingChecked(positionCopy.colorToMove())) {
			Moves validMoves;
			getValidMoves(positionCopy, validMoves);
			if (validMoves.is_empty()) {
				result[offset++] = L'#';
			} else {
				result[offset++] = L'+';
			}
			assert(offset < result.size());
		}

		assert(result[offset] == '\0');
		return result;
	}
};

} //namespace phase4::engine::board

#endif
