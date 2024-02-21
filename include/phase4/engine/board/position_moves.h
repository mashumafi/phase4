#ifndef PHASE4_ENGINE_BOARD_POSITION_MOVES_H
#define PHASE4_ENGINE_BOARD_POSITION_MOVES_H

#include <phase4/engine/board/operators.h>
#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/move.h>

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/fast_vector.h>
#include <phase4/engine/common/game_phase.h>
#include <phase4/engine/common/math.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/wall_operations.h>

#include <cstdint>
#include <cstring>
#include <optional>

namespace phase4::engine::board {

class PositionMoves {
public:
	struct MakeMoveResult {
		struct Movement {
			common::Square from;
			common::Square to;

			bool operator==(MakeMoveResult::Movement that) const {
				return from == that.from && to == that.to;
			}
		};

		struct PieceAndSquare {
			common::PieceType pieceType;
			common::Square at;

			bool operator==(MakeMoveResult::PieceAndSquare that) const {
				return pieceType == that.pieceType && at == that.at;
			}
		};

		common::FastVector<Movement, 5> moved;

		std::optional<PieceAndSquare> added;

		std::optional<common::PieceType> promotion;

		std::optional<PieceAndSquare> killed;
		common::FastVector<PieceAndSquare, 2> removed;

		std::optional<common::FieldIndex> slide;
	};

	static inline MakeMoveResult makeMove(Position &position, moves::Move move) {
		using namespace common;

		MakeMoveResult result;

		const PieceType pieceType = position.m_pieceTable[move.from()];
		const PieceColor enemyColor = position.m_colorToMove.invert();

		if (position.m_colorToMove == PieceColor::BLACK) {
			++position.m_movesCount;
		}

		if (pieceType == PieceType::PAWN || move.flags().isCapture() || move.flags().isCastling()) {
			position.m_irreversibleMovesCount = 0;
		} else {
			position.m_irreversibleMovesCount++;
		}

		if (position.m_enPassant != 0) {
			const uint8_t enPassantRank = position.m_enPassant.fastBitScan() % 8;
			position.m_hash = position.m_hash.toggleEnPassant(enPassantRank);
			position.m_enPassant = 0;
		}

		if (move.flags().isSinglePush()) {
			position.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			position.m_hash = position.m_hash.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());

			if (pieceType == PieceType::PAWN) {
				position.m_pawnHash = position.m_pawnHash.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());
			}
		} else if (move.flags().isDoublePush()) {
			position.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			position.m_hash = position.m_hash.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());
			position.m_pawnHash = position.m_pawnHash.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());

			const Bitboard enPassantField = (position.m_colorToMove == PieceColor::WHITE) ? move.to().south(1).asBitboard() : move.to().north(1).asBitboard();
			const uint8_t enPassantFieldIndex = enPassantField.fastBitScan();

			position.m_enPassant = enPassantField;
			position.m_hash = position.m_hash.toggleEnPassant(enPassantFieldIndex % 8);
		} else if (move.flags().isEnPassant()) {
			const Square enemyPieceField((position.m_colorToMove == PieceColor::WHITE) ? move.to().south(1) : move.to().north(1));
			const PieceType killedPiece = position.m_pieceTable[enemyPieceField];

			position.removePiece(enemyColor, killedPiece, enemyPieceField);
			result.removed.push_back({ killedPiece, enemyPieceField });
			position.m_hash = position.m_hash.addOrRemovePiece(enemyColor, killedPiece, enemyPieceField);
			position.m_pawnHash = position.m_pawnHash.addOrRemovePiece(enemyColor, killedPiece, enemyPieceField);

			position.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			position.m_hash = position.m_hash.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());
			position.m_pawnHash = position.m_pawnHash.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());

			result.killed = MakeMoveResult::PieceAndSquare{ killedPiece, enemyPieceField };
		} else if (move.flags().isCapture()) {
			PieceType killedPiece = position.m_pieceTable[move.to()];

			position.removePiece(enemyColor, killedPiece, move.to());
			result.removed.push_back({ killedPiece, move.to() });
			position.m_hash = position.m_hash.addOrRemovePiece(enemyColor, killedPiece, move.to());

			if (killedPiece == PieceType::PAWN) {
				position.m_pawnHash = position.m_pawnHash.addOrRemovePiece(enemyColor, killedPiece, move.to());
			} else if (killedPiece == PieceType::ROOK) {
				switch (move.to()) {
					case Square::H1.get_raw_value():
						position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_SHORT);
						position.m_castling &= ~Castling::WHITE_SHORT;
						break;
					case Square::A1.get_raw_value():
						position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_LONG);
						position.m_castling &= ~Castling::WHITE_LONG;
						break;
					case Square::H8.get_raw_value():
						position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_SHORT);
						position.m_castling &= ~Castling::BLACK_SHORT;
						break;
					case Square::A8.get_raw_value():
						position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_LONG);
						position.m_castling &= ~Castling::BLACK_LONG;
						break;
				}
			}

			if (move.flags().isPromotion()) {
				PieceType promotionPiece = move.flags().getPromotionPiece();

				position.removePiece(position.m_colorToMove, pieceType, move.from());
				result.removed.push_back({ pieceType, move.from() });
				position.m_hash = position.m_hash.addOrRemovePiece(position.m_colorToMove, pieceType, move.from());
				position.m_pawnHash = position.m_pawnHash.addOrRemovePiece(position.m_colorToMove, pieceType, move.from());

				position.addPiece(position.m_colorToMove, promotionPiece, move.to());
				result.added = { promotionPiece, move.to() };
				position.m_hash = position.m_hash.addOrRemovePiece(position.m_colorToMove, promotionPiece, move.to());

				result.promotion = promotionPiece;
			} else {
				position.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());
				result.moved.push_back({ move.from(), move.to() });
				position.m_hash = position.m_hash.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());

				if (pieceType == PieceType::PAWN) {
					position.m_pawnHash = position.m_pawnHash.movePiece(position.m_colorToMove, pieceType, move.from(), move.to());
				}
			}

			result.killed = MakeMoveResult::PieceAndSquare{ killedPiece, move.to() };
		} else if (move.flags().isCastling()) {
			if (move.flags().isKingCastling()) { // Short/King castling
				if (position.m_colorToMove == PieceColor::WHITE) {
					position.movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::G1);
					position.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::H1, Square::F1);

					result.moved.push_back({ Square::E1, Square::G1 });
					result.moved.push_back({ Square::H1, Square::F1 });

					position.m_hash = position.m_hash.movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::G1);
					position.m_hash = position.m_hash.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::H1, Square::F1);
				} else {
					position.movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::G8);
					position.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::H8, Square::F8);

					result.moved.push_back({ Square::E8, Square::G8 });
					result.moved.push_back({ Square::H8, Square::F8 });

					position.m_hash = position.m_hash.movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::G8);
					position.m_hash = position.m_hash.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::H8, Square::F8);
				}
			} else { // Long/Queen castling
				if (position.m_colorToMove == PieceColor::WHITE) {
					position.movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::C1);
					position.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::A1, Square::D1);

					result.moved.push_back({ Square::E1, Square::C1 });
					result.moved.push_back({ Square::A1, Square::D1 });

					position.m_hash = position.m_hash.movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::C1);
					position.m_hash = position.m_hash.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::A1, Square::D1);
				} else {
					position.movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::C8);
					position.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::A8, Square::D8);

					result.moved.push_back({ Square::E8, Square::C8 });
					result.moved.push_back({ Square::A8, Square::D8 });

					position.m_hash = position.m_hash.movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::C8);
					position.m_hash = position.m_hash.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::A8, Square::D8);
				}
			}

			if (position.m_colorToMove == PieceColor::WHITE) {
				position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_SHORT);
				position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_LONG);
				position.m_castling &= ~Castling::WHITE_CASTLING;
			} else {
				position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_SHORT);
				position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_LONG);
				position.m_castling &= ~Castling::BLACK_CASTLING;
			}

			position.m_castlingDone[position.m_colorToMove.get_raw_value()] = true;
		} else if (move.flags().isPromotion()) {
			PieceType promotionPiece = move.flags().getPromotionPiece();

			position.removePiece(position.m_colorToMove, pieceType, move.from());
			result.removed.push_back({ pieceType, move.from() });
			position.m_hash = position.m_hash.addOrRemovePiece(position.m_colorToMove, pieceType, move.from());
			position.m_pawnHash = position.m_pawnHash.addOrRemovePiece(position.m_colorToMove, pieceType, move.from());

			position.addPiece(position.m_colorToMove, promotionPiece, move.to());
			result.added = { promotionPiece, move.to() };
			position.m_hash = position.m_hash.addOrRemovePiece(position.m_colorToMove, promotionPiece, move.to());

			result.promotion = promotionPiece;
		}

		if (pieceType == PieceType::KING && !move.flags().isCastling()) {
			if (position.m_colorToMove == PieceColor::WHITE) {
				position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_SHORT);
				position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_LONG);
				position.m_castling &= ~Castling::WHITE_CASTLING;
			} else {
				position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_SHORT);
				position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_LONG);
				position.m_castling &= ~Castling::BLACK_CASTLING;
			}
		} else if (pieceType == PieceType::ROOK && position.m_castling != Castling::NONE) {
			switch (move.from().get_raw_value()) {
				case Square::H1.get_raw_value():
					position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_SHORT);
					position.m_castling &= ~Castling::WHITE_SHORT;
					break;
				case Square::A1.get_raw_value():
					position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_LONG);
					position.m_castling &= ~Castling::WHITE_LONG;
					break;
				case Square::H8.get_raw_value():
					position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_SHORT);
					position.m_castling &= ~Castling::BLACK_SHORT;
					break;
				case Square::A8.get_raw_value():
					position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_LONG);
					position.m_castling &= ~Castling::BLACK_LONG;
					break;
			}
		}

		if (likely(position.m_walls > 0)) {
			uint8_t wallIndex = position.m_walls.fastBitScan();
			const FieldIndex wallMove = WallOperations::SLIDE_DIR[wallIndex][move.to()];
			if (wallMove != FieldIndex::ZERO) {
				position.m_occupancySummary = position.m_occupancySummary & ~position.m_walls;

				Bitboard original = WallOperations::SLIDE_TO[wallIndex][move.to()];
				while (original > 0) {
					Square from(original.fastBitScan());
					Square to = WallOperations::SLIDE_SQUARE[wallIndex][from];
					result.moved.push_back(MakeMoveResult::Movement{ from, to });
					if (auto pieceResult = position.getPiece(from)) {
						auto [resultColor, resultType] = *pieceResult;
						position.movePiece(resultColor, resultType, from, to);
						result.moved.push_back({ from, to });
						position.m_hash = position.m_hash.movePiece(resultColor, resultType, from, to);
						if (resultType == PieceType::PAWN) {
							position.m_pawnHash = position.m_pawnHash.movePiece(resultColor, resultType, from, to);
						}

						if (resultType == PieceType::PAWN && position.m_enPassant != 0) {
							int8_t offset = (resultColor == PieceColor::WHITE) ? -8 : 8;
							uint8_t enPassantField = position.m_enPassant.fastBitScan();
							if (enPassantField == from.get_raw_value() + offset) {
								uint8_t enPassantRank = enPassantField % 8;
								position.m_hash = position.m_hash.toggleEnPassant(enPassantRank);

								if (wallMove.offset() >= 0)
									position.m_enPassant = position.m_enPassant >> wallMove.offset();
								else
									position.m_enPassant = position.m_enPassant << -wallMove.offset();
								position.m_hash = position.m_hash.toggleEnPassant(position.m_enPassant.fastBitScan() % 8);
							}
						}

						if (resultType == PieceType::KING) {
							if (resultColor == PieceColor::WHITE) {
								position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_SHORT);
								position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_LONG);
								position.m_castling &= ~Castling::WHITE_CASTLING;
							} else {
								position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_SHORT);
								position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_LONG);
								position.m_castling &= ~Castling::BLACK_CASTLING;
							}
						} else if (resultType == PieceType::ROOK) {
							switch (from.get_raw_value()) {
								case Square::H1.get_raw_value():
									position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_SHORT);
									position.m_castling &= ~Castling::WHITE_SHORT;
									break;
								case Square::A1.get_raw_value():
									position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::WHITE_LONG);
									position.m_castling &= ~Castling::WHITE_LONG;
									break;
								case Square::H8.get_raw_value():
									position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_SHORT);
									position.m_castling &= ~Castling::BLACK_SHORT;
									break;
								case Square::A8.get_raw_value():
									position.m_hash = position.m_hash.removeCastlingFlag(position.m_castling, Castling::BLACK_LONG);
									position.m_castling &= ~Castling::BLACK_LONG;
									break;
							}
						}
					}
					original = original.popLsb();
				}

				position.m_hash = position.m_hash.toggleWalls(position.m_walls); // Turn off previous wall
				position.m_walls = WallOperations::SLIDE_TO[wallIndex][move.to()];
				position.m_hash = position.m_hash.toggleWalls(position.m_walls); // Turn on new wall location
				position.m_occupancySummary = position.m_occupancySummary | position.m_walls;
			}
			result.slide = wallMove;
		} else {
			result.slide = common::FieldIndex::ZERO;
		}

		position.m_colorToMove = enemyColor;
		position.m_hash = position.m_hash.changeSide();

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

		if (unlikely(position.m_walls.fastCount() != 4))
			return {};

		SlideResult result;

		position.m_occupancySummary &= ~position.m_walls;

		Bitboard walls = position.m_walls;
		if (wallMove.offset() >= 0)
			walls >>= wallMove.offset();
		else
			walls <<= -wallMove.offset();

		Bitboard original = walls;
		while (original != 0) {
			const Square from(original.fastBitScan());
			const Square to(from + wallMove.offset());
			result.moves.push_back({ from, to });
			if (auto pieceResult = position.getPiece(from)) {
				const auto [resultColor, resultType] = *pieceResult;
				position.movePiece(resultColor, resultType, from, to);
			}

			original = original.popLsb();
		}

		position.m_hash = position.m_hash.toggleWalls(position.m_walls); // Turn off previous wall
		position.m_walls = walls;
		position.m_hash = position.m_hash.toggleWalls(position.m_walls); // Turn on new wall location
		position.m_occupancySummary |= position.m_walls;

		return result;
	}

	static inline std::optional<moves::Move> findRealMove(moves::Moves moves, moves::Move move) {
		const bool isQuiet = move.flags() == moves::MoveFlags::QUIET;
		for (size_t i = 0; i < moves.size(); ++i) {
			const bool correctFlags = isQuiet || (moves[i].flags().isPromotion() && moves[i].flags().getPromotionPiece() == move.flags().getPromotionPiece());
			if (moves[i].from() == move.from() && moves[i].to() == move.to() && correctFlags) {
				return moves[i];
			}
		}

		return std::nullopt;
	};

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
};

} //namespace phase4::engine::board

#endif
