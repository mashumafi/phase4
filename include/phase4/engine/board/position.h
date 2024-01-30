#ifndef PHASE4_ENGINE_BOARD_POSITION_H
#define PHASE4_ENGINE_BOARD_POSITION_H

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/piece_square_tables/piece_square_table_data.h>
#include <phase4/engine/board/zobrist_hashing.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>
#include <phase4/engine/moves/patterns/passing_pattern_generator.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/fast_vector.h>
#include <phase4/engine/common/game_phase.h>
#include <phase4/engine/common/math.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/wall_operations.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <optional>

namespace phase4::engine::board {

class Position {
public:
	ZobristHashing m_hash;
	ZobristHashing m_pawnHash;

	std::array<std::array<common::Bitset, 6>, 2> m_colorPieceMasks;
	std::array<common::Bitset, 2> m_occupancyByColor = {};
	common::Bitset m_occupancySummary;
	common::Bitset m_enPassant;
	common::Castling m_castling = common::Castling::EVERYTHING;
	common::PieceColor m_colorToMove = common::PieceColor::WHITE;
	uint16_t m_movesCount = 0;
	size_t m_irreversibleMovesCount = 0;
	uint16_t m_nullMoves = 0;

	std::array<bool, 2> m_castlingDone = {};
	std::array<int32_t, 2> m_material = {};
	std::array<std::array<int32_t, 2>, 2> m_positionEval = {};

	std::array<common::PieceType, 64> m_pieceTable = {};

	common::Bitset m_walls = 0;

	inline constexpr Position() noexcept = default;

	inline constexpr Position(const Position &that) noexcept = default;
	inline constexpr Position &operator=(const Position &that) = default;

	inline constexpr Position(Position &&that) noexcept = default;
	inline constexpr Position &operator=(Position &&that) noexcept = default;

	void clearWalls() {
		if (likely(m_walls > 0)) {
			m_occupancySummary = m_occupancySummary & ~m_walls;
			m_hash = m_hash.toggleWalls(m_walls);
			m_walls = 0;
		}
	}

	inline void movePiece(common::PieceColor color, common::PieceType piece, common::Square from, common::Square to) {
		using namespace common;
		using namespace piece_square_tables;

		common::Bitset move = from.asBitboard() | to.asBitboard();

		m_colorPieceMasks[color.get_raw_value()][piece.get_raw_value()] ^= move;
		m_occupancyByColor[color.get_raw_value()] ^= move;
		m_occupancySummary = m_occupancySummary ^ move;

		m_positionEval[color.get_raw_value()][GamePhase::OPENING] -= PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][from];
		m_positionEval[color.get_raw_value()][GamePhase::OPENING] += PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][to];

		m_positionEval[color.get_raw_value()][GamePhase::ENDING] -= PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][from];
		m_positionEval[color.get_raw_value()][GamePhase::ENDING] += PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][to];

		m_pieceTable[from] = PieceType::INVALID;
		m_pieceTable[to] = piece;
	}

	inline void addPiece(common::PieceColor color, common::PieceType piece, common::Square fieldIndex) {
		using namespace phase4::engine::common;

		common::Bitset field = fieldIndex.asBitboard();

		m_colorPieceMasks[color.get_raw_value()][piece.get_raw_value()] ^= field;
		m_occupancyByColor[color.get_raw_value()] ^= field;
		m_occupancySummary = m_occupancySummary ^ field;

		m_material[color.get_raw_value()] += board::EvaluationConstants::PIECE_VALUES[piece.get_raw_value()];

		m_positionEval[color.get_raw_value()][GamePhase::OPENING] += piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][fieldIndex];
		m_positionEval[color.get_raw_value()][GamePhase::ENDING] += piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][fieldIndex];

		m_pieceTable[fieldIndex] = piece;
	}

	inline void removePiece(common::PieceColor color, common::PieceType piece, common::Square fieldIndex) {
		using namespace phase4::engine::common;

		common::Bitset field = fieldIndex.asBitboard();

		m_colorPieceMasks[color.get_raw_value()][piece.get_raw_value()] ^= field;
		m_occupancyByColor[color.get_raw_value()] ^= field;
		m_occupancySummary ^= field;

		m_material[color.get_raw_value()] -= EvaluationConstants::PIECE_VALUES[piece.get_raw_value()];

		m_positionEval[color.get_raw_value()][GamePhase::OPENING] -= piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][fieldIndex];
		m_positionEval[color.get_raw_value()][GamePhase::ENDING] -= piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][fieldIndex];

		m_pieceTable[fieldIndex] = PieceType::INVALID;
	}

	bool isKingChecked(common::PieceColor color) const {
		const common::Bitset king = m_colorPieceMasks[color.get_raw_value()][common::PieceType::KING.get_raw_value()];
		if (unlikely(king == 0)) {
			return false;
		}

		const uint8_t kingField = king.fastBitScan();
		return isFieldAttacked(color, common::Square(kingField));
	}

	std::optional<std::tuple<common::PieceColor, common::PieceType>> GetPiece(uint16_t fieldIndex) {
		using namespace common;

		PieceType pieceType = m_pieceTable[fieldIndex];
		if (pieceType == PieceType::INVALID) {
			return {};
		}

		uint64_t fieldBB = WallOperations::SquareBB(fieldIndex);
		if ((m_colorPieceMasks[PieceColor::WHITE.get_raw_value()][pieceType.get_raw_value()] & fieldBB) > 0) {
			return std::make_tuple(PieceColor::WHITE, pieceType);
		}

		if ((m_colorPieceMasks[PieceColor::BLACK.get_raw_value()][pieceType.get_raw_value()] & fieldBB) > 0) {
			return std::make_tuple(PieceColor::BLACK, pieceType);
		}

		return {};
	}

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

	MakeMoveResult makeMove(moves::Move move) {
		using namespace common;

		MakeMoveResult result;

		PieceType pieceType = m_pieceTable[move.from()];
		PieceColor enemyColor = m_colorToMove.invert();

		if (m_colorToMove == PieceColor::WHITE) {
			m_movesCount++;
		}

		if (pieceType == PieceType::PAWN || move.flags().isCapture() || move.flags().isCastling()) {
			m_irreversibleMovesCount = 0;
		} else {
			m_irreversibleMovesCount++;
		}

		if (m_enPassant != 0) {
			uint8_t enPassantRank = m_enPassant.fastBitScan() % 8;
			m_hash = m_hash.toggleEnPassant(enPassantRank);
			m_enPassant = 0;
		}

		if (move.flags().isSinglePush()) {
			movePiece(m_colorToMove, pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			m_hash = m_hash.movePiece(m_colorToMove, pieceType, move.from(), move.to());

			if (pieceType == PieceType::PAWN) {
				m_pawnHash = m_pawnHash.movePiece(m_colorToMove, pieceType, move.from(), move.to());
			}
		} else if (move.flags().isDoublePush()) {
			movePiece(m_colorToMove, pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			m_hash = m_hash.movePiece(m_colorToMove, pieceType, move.from(), move.to());
			m_pawnHash = m_pawnHash.movePiece(m_colorToMove, pieceType, move.from(), move.to());

			Bitset enPassantField = (m_colorToMove == PieceColor::WHITE) ? Square(move.to() - 8).asBitboard() : Square(move.to() + 8).asBitboard();
			uint8_t enPassantFieldIndex = enPassantField.fastBitScan();

			m_enPassant = enPassantField;
			m_hash = m_hash.toggleEnPassant(enPassantFieldIndex % 8);
		} else if (move.flags().isEnPassant()) {
			Square enemyPieceField((m_colorToMove == PieceColor::WHITE) ? move.to() - 8 : move.to() + 8);
			PieceType killedPiece = m_pieceTable[enemyPieceField];

			removePiece(enemyColor, killedPiece, enemyPieceField);
			result.removed.push_back({ killedPiece, enemyPieceField });
			m_hash = m_hash.addOrRemovePiece(enemyColor, killedPiece, enemyPieceField);
			m_pawnHash = m_pawnHash.addOrRemovePiece(enemyColor, killedPiece, enemyPieceField);

			movePiece(m_colorToMove, pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			m_hash = m_hash.movePiece(m_colorToMove, pieceType, move.from(), move.to());
			m_pawnHash = m_pawnHash.movePiece(m_colorToMove, pieceType, move.from(), move.to());

			result.killed = MakeMoveResult::PieceAndSquare{ killedPiece, enemyPieceField };
		} else if (move.flags().isCapture()) {
			PieceType killedPiece = m_pieceTable[move.to()];

			removePiece(enemyColor, killedPiece, move.to());
			result.removed.push_back({ killedPiece, move.to() });
			m_hash = m_hash.addOrRemovePiece(enemyColor, killedPiece, move.to());

			if (killedPiece == PieceType::PAWN) {
				m_pawnHash = m_pawnHash.addOrRemovePiece(enemyColor, killedPiece, move.to());
			} else if (killedPiece == PieceType::ROOK) {
				switch (move.to()) {
					case Square::H1.get_raw_value():
						m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
						m_castling &= ~Castling::WHITE_SHORT;
						break;
					case Square::A1.get_raw_value():
						m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
						m_castling &= ~Castling::WHITE_LONG;
						break;
					case Square::H8.get_raw_value():
						m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
						m_castling &= ~Castling::BLACK_SHORT;
						break;
					case Square::A8.get_raw_value():
						m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
						m_castling &= ~Castling::BLACK_LONG;
						break;
				}
			}

			if (move.flags().isPromotion()) {
				PieceType promotionPiece = move.flags().getPromotionPiece();

				removePiece(m_colorToMove, pieceType, move.from());
				result.removed.push_back({ pieceType, move.from() });
				m_hash = m_hash.addOrRemovePiece(m_colorToMove, pieceType, move.from());
				m_pawnHash = m_pawnHash.addOrRemovePiece(m_colorToMove, pieceType, move.from());

				addPiece(m_colorToMove, promotionPiece, move.to());
				result.added = { promotionPiece, move.to() };
				m_hash = m_hash.addOrRemovePiece(m_colorToMove, promotionPiece, move.to());

				result.promotion = promotionPiece;
			} else {
				movePiece(m_colorToMove, pieceType, move.from(), move.to());
				result.moved.push_back({ move.from(), move.to() });
				m_hash = m_hash.movePiece(m_colorToMove, pieceType, move.from(), move.to());

				if (pieceType == PieceType::PAWN) {
					m_pawnHash = m_pawnHash.movePiece(m_colorToMove, pieceType, move.from(), move.to());
				}
			}

			result.killed = MakeMoveResult::PieceAndSquare{ killedPiece, move.to() };
		} else if (move.flags().isCastling()) {
			// Short castling
			if (move.flags().isKingCastling()) {
				if (m_colorToMove == PieceColor::WHITE) {
					movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::G1);
					movePiece(PieceColor::WHITE, PieceType::ROOK, Square::H1, Square::F1);

					result.moved.push_back({ Square::E1, Square::G1 });
					result.moved.push_back({ Square::H1, Square::F1 });

					m_hash = m_hash.movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::G1);
					m_hash = m_hash.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::H1, Square::F1);
				} else {
					movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::G8);
					movePiece(PieceColor::BLACK, PieceType::ROOK, Square::H8, Square::F8);

					result.moved.push_back({ Square::E8, Square::G8 });
					result.moved.push_back({ Square::H8, Square::F8 });

					m_hash = m_hash.movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::G8);
					m_hash = m_hash.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::H8, Square::F8);
				}
			} else { // Long castling
				if (m_colorToMove == PieceColor::WHITE) {
					movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::C1);
					movePiece(PieceColor::WHITE, PieceType::ROOK, Square::A1, Square::D1);

					result.moved.push_back({ Square::E1, Square::C1 });
					result.moved.push_back({ Square::A1, Square::D1 });

					m_hash = m_hash.movePiece(PieceColor::WHITE, PieceType::KING, Square::E1, Square::C1);
					m_hash = m_hash.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::A1, Square::D1);
				} else {
					movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::C8);
					movePiece(PieceColor::BLACK, PieceType::ROOK, Square::A8, Square::D8);

					result.moved.push_back({ Square::E8, Square::C8 });
					result.moved.push_back({ Square::A8, Square::D8 });

					m_hash = m_hash.movePiece(PieceColor::BLACK, PieceType::KING, Square::E8, Square::C8);
					m_hash = m_hash.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::A8, Square::D8);
				}
			}

			if (m_colorToMove == PieceColor::WHITE) {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
				m_castling &= ~Castling::WHITE_CASTLING;
			} else {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
				m_castling &= ~Castling::BLACK_CASTLING;
			}

			m_castlingDone[m_colorToMove.get_raw_value()] = true;
		} else if (move.flags().isPromotion()) {
			PieceType promotionPiece = move.flags().getPromotionPiece();

			removePiece(m_colorToMove, pieceType, move.from());
			result.removed.push_back({ pieceType, move.from() });
			m_hash = m_hash.addOrRemovePiece(m_colorToMove, pieceType, move.from());
			m_pawnHash = m_pawnHash.addOrRemovePiece(m_colorToMove, pieceType, move.from());

			addPiece(m_colorToMove, promotionPiece, move.to());
			result.added = { promotionPiece, move.to() };
			m_hash = m_hash.addOrRemovePiece(m_colorToMove, promotionPiece, move.to());

			result.promotion = promotionPiece;
		}

		if (pieceType == PieceType::KING && !move.flags().isCastling()) {
			if (m_colorToMove == PieceColor::WHITE) {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
				m_castling &= ~Castling::WHITE_CASTLING;
			} else {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
				m_castling &= ~Castling::BLACK_CASTLING;
			}
		} else if (pieceType == PieceType::ROOK && m_castling != Castling::NONE) {
			switch (move.from().get_raw_value()) {
				case Square::H1.get_raw_value():
					m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
					m_castling &= ~Castling::WHITE_SHORT;
					break;
				case Square::A1.get_raw_value():
					m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
					m_castling &= ~Castling::WHITE_LONG;
					break;
				case Square::H8.get_raw_value():
					m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
					m_castling &= ~Castling::BLACK_SHORT;
					break;
				case Square::A8.get_raw_value():
					m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
					m_castling &= ~Castling::BLACK_LONG;
					break;
			}
		}

		if (likely(m_walls > 0)) {
			uint8_t wallIndex = m_walls.fastBitScan();
			const FieldIndex wallMove = WallOperations::SLIDE_DIR[wallIndex][move.to()];
			if (wallMove != FieldIndex::ZERO) {
				m_occupancySummary = m_occupancySummary & ~m_walls;

				Bitset original = WallOperations::SLIDE_TO[wallIndex][move.to()];
				while (original > 0) {
					Square from(original.fastBitScan());
					Square to = WallOperations::SLIDE_SQUARE[wallIndex][from];
					result.moved.push_back(MakeMoveResult::Movement{ from, to });
					if (auto pieceResult = GetPiece(from)) {
						auto [resultColor, resultType] = *pieceResult;
						movePiece(resultColor, resultType, from, to);
						result.moved.push_back({ from, to });
						m_hash = m_hash.movePiece(resultColor, resultType, from, to);
						if (resultType == PieceType::PAWN) {
							m_pawnHash = m_pawnHash.movePiece(resultColor, resultType, from, to);
						}

						if (resultType == PieceType::PAWN && m_enPassant != 0) {
							int offset = (resultColor == PieceColor::WHITE) ? -8 : 8;
							uint8_t enPassantField = m_enPassant.fastBitScan();
							if (enPassantField == from.get_raw_value() + offset) {
								uint8_t enPassantRank = enPassantField % 8;
								m_hash = m_hash.toggleEnPassant(enPassantRank);

								if (wallMove.offset() >= 0)
									m_enPassant = m_enPassant >> wallMove.offset();
								else
									m_enPassant = m_enPassant << -wallMove.offset();
								m_hash = m_hash.toggleEnPassant(m_enPassant.fastBitScan() % 8);
							}
						}

						if (resultType == PieceType::KING) {
							if (resultColor == PieceColor::WHITE) {
								m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
								m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
								m_castling &= ~Castling::WHITE_CASTLING;
							} else {
								m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
								m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
								m_castling &= ~Castling::BLACK_CASTLING;
							}
						} else if (resultType == PieceType::ROOK) {
							switch (from.get_raw_value()) {
								case Square::H1.get_raw_value():
									m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
									m_castling &= ~Castling::WHITE_SHORT;
									break;
								case Square::A1.get_raw_value():
									m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
									m_castling &= ~Castling::WHITE_LONG;
									break;
								case Square::H8.get_raw_value():
									m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
									m_castling &= ~Castling::BLACK_SHORT;
									break;
								case Square::A8.get_raw_value():
									m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
									m_castling &= ~Castling::BLACK_LONG;
									break;
							}
						}
					}
					original = original.popLsb();
				}

				m_hash = m_hash.toggleWalls(m_walls); // Turn off previous wall
				m_walls = WallOperations::SLIDE_TO[wallIndex][move.to()];
				m_hash = m_hash.toggleWalls(m_walls); // Turn on new wall location
				m_occupancySummary = m_occupancySummary | m_walls;
			}
			result.slide = wallMove;
		} else {
			result.slide = common::FieldIndex::ZERO;
		}

		m_colorToMove = enemyColor;
		m_hash = m_hash.changeSide();

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
	SlideResult slideWall(common::FieldIndex wallMove) {
		using namespace common;

		if (unlikely(m_walls.fastCount() != 4))
			return {};

		SlideResult result;

		m_occupancySummary &= ~m_walls;

		Bitset walls = m_walls;
		if (wallMove.offset() >= 0)
			walls >>= wallMove.offset();
		else
			walls <<= -wallMove.offset();

		Bitset original = walls;
		while (original != 0) {
			const Square from(original.fastBitScan());
			const Square to(from + wallMove.offset());
			result.moves.push_back({ from, to });
			if (auto pieceResult = GetPiece(from)) {
				const auto [resultColor, resultType] = *pieceResult;
				movePiece(resultColor, resultType, from, to);
			}

			original = original.popLsb();
		}

		m_hash = m_hash.toggleWalls(m_walls); // Turn off previous wall
		m_walls = walls;
		m_hash = m_hash.toggleWalls(m_walls);
		; // Turn on new wall location
		m_occupancySummary |= m_walls;

		return result;
	}

	bool isFieldAttacked(common::PieceColor color, common::Square fieldIndex) const {
		using namespace common;

		PieceColor enemyColor = color.invert();

		const Bitset fileRankAttacks = moves::MovesGenerator::getRookMoves(m_occupancySummary, fieldIndex) & m_occupancyByColor[enemyColor.get_raw_value()];
		const Bitset attackingRooks = fileRankAttacks & (m_colorPieceMasks[enemyColor.get_raw_value()][PieceType::ROOK.get_raw_value()] | m_colorPieceMasks[enemyColor.get_raw_value()][PieceType::QUEEN.get_raw_value()]);
		if (attackingRooks != 0) {
			return true;
		}

		const Bitset diagonalAttacks = moves::MovesGenerator::getBishopMoves(m_occupancySummary, fieldIndex) & m_occupancyByColor[enemyColor.get_raw_value()];
		const Bitset attackingBishops = diagonalAttacks & (m_colorPieceMasks[enemyColor.get_raw_value()][PieceType::BISHOP.get_raw_value()] | m_colorPieceMasks[enemyColor.get_raw_value()][PieceType::QUEEN.get_raw_value()]);
		if (attackingBishops != 0) {
			return true;
		}

		const Bitset jumpAttacks = moves::MovesGenerator::getKnightMoves(fieldIndex);
		const Bitset attackingKnights = jumpAttacks & m_colorPieceMasks[enemyColor.get_raw_value()][PieceType::KNIGHT.get_raw_value()];
		if (attackingKnights != 0) {
			return true;
		}

		const Bitset boxAttacks = moves::MovesGenerator::getKingMoves(fieldIndex);
		const Bitset attackingKings = boxAttacks & m_colorPieceMasks[enemyColor.get_raw_value()][PieceType::KING.get_raw_value()];
		if (attackingKings != 0) {
			return true;
		}

		const Bitset field = fieldIndex.asBitboard();
		const Bitset potentialPawns = boxAttacks & m_colorPieceMasks[enemyColor.get_raw_value()][PieceType::PAWN.get_raw_value()];
		const Bitset attackingPawns = (color.get_raw_value() == PieceColor::WHITE.get_raw_value()) ? field & ((potentialPawns >> 7) | (potentialPawns >> 9)) : field & ((potentialPawns << 7) | (potentialPawns << 9));

		return (attackingPawns != 0);
	}

	constexpr int32_t getPhaseRatio() const {
		using namespace common;

		const int32_t materialOfWeakerSide = Math::min_int32(m_material[PieceColor::WHITE.get_raw_value()], m_material[PieceColor::BLACK.get_raw_value()]);

		constexpr int32_t openingDelta = board::EvaluationConstants::MATERIAL_AT_OPENING - board::EvaluationConstants::OPENING_ENDGAME_EDGE;
		const int32_t boardDelta = materialOfWeakerSide - board::EvaluationConstants::OPENING_ENDGAME_EDGE;

		return Math::max_int32(boardDelta, 0) * PositionConstants::PHASE_RESOLUTION / openingDelta;
	}

	bool isFieldPassing(common::PieceColor color, common::Square field) const {
		const common::PieceColor enemyColor = color.invert();
		const common::Bitset passingArea = moves::patterns::PassingPatternGenerator::getPattern(color, field);

		return (passingArea & m_colorPieceMasks[enemyColor.get_raw_value()][common::PieceType::PAWN.get_raw_value()]) == 0;
	}

	bool isFiftyMoveRuleDraw() const {
		if (m_nullMoves == 0 && m_irreversibleMovesCount >= 100) {
			return true;
		}

		return false;
	}

	bool isInsufficientMaterial() const {
		using namespace common;

		int32_t drawEdge = EvaluationConstants::PIECE_VALUES[PieceType::KING.get_raw_value()] + 4 * EvaluationConstants::PIECE_VALUES[PieceType::PAWN.get_raw_value()];
		if (m_material[PieceColor::WHITE.get_raw_value()] < drawEdge && m_material[PieceColor::BLACK.get_raw_value()] < drawEdge) {
			const Bitset whiteKnightOrBishopPresent = (m_colorPieceMasks[PieceColor::WHITE.get_raw_value()][PieceType::KNIGHT.get_raw_value()] | m_colorPieceMasks[PieceColor::WHITE.get_raw_value()][PieceType::BISHOP.get_raw_value()]) != 0;
			const Bitset blackKnightOrBishopPresent = (m_colorPieceMasks[PieceColor::BLACK.get_raw_value()][PieceType::KNIGHT.get_raw_value()] | m_colorPieceMasks[PieceColor::BLACK.get_raw_value()][PieceType::BISHOP.get_raw_value()]) != 0;

			if (whiteKnightOrBishopPresent != 0 && blackKnightOrBishopPresent != 0) {
				return false;
			}

			if (m_colorPieceMasks[PieceColor::WHITE.get_raw_value()][PieceType::PAWN.get_raw_value()] == 0 && m_colorPieceMasks[PieceColor::BLACK.get_raw_value()][PieceType::PAWN.get_raw_value()] == 0) {
				return true;
			}
		}

		return false;
	}
};

} //namespace phase4::engine::board

#endif
