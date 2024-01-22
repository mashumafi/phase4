#ifndef PHASE4_ENGINE_BOARD_POSITION_H
#define PHASE4_ENGINE_BOARD_POSITION_H

#include <phase4/engine/ai/score/evaluation_constants.h> // TODO: remove cycle
#include <phase4/engine/ai/score/piece_square_tables/piece_square_table_data.h>
#include <phase4/engine/board/zobrist_hashing.h>
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/fast_vector.h>
#include <phase4/engine/common/game_phase.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/wall_operations.h>
#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <optional>

namespace phase4::engine::board {

class Position {
public:
	ZobristHashing m_hash;
	ZobristHashing PawnHash;

	common::Bitset Pieces[2][6];
	common::Bitset Occupancy[2];
	common::Bitset OccupancySummary;
	common::Bitset EnPassant;
	common::Castling m_castling = common::Castling::NONE;
	common::PieceColor ColorToMove = common::PieceColor::WHITE;
	uint16_t MovesCount;
	uint16_t IrreversibleMovesCount;
	uint16_t NullMoves;

	bool CastlingDone[2];
	int32_t Material[2];
	int32_t m_positionEval[2][2];

	std::array<common::PieceType, 64> PieceTable;

	common::Bitset Walls = 0;

	void SetDefaultState() {
		Pieces[common::PieceColor::WHITE.get_raw_value()][common::PieceType::PAWN.get_raw_value()] = 65280;
		Pieces[common::PieceColor::WHITE.get_raw_value()][common::PieceType::ROOK.get_raw_value()] = 129;
		Pieces[common::PieceColor::WHITE.get_raw_value()][common::PieceType::KNIGHT.get_raw_value()] = 66;
		Pieces[common::PieceColor::WHITE.get_raw_value()][common::PieceType::BISHOP.get_raw_value()] = 36;
		Pieces[common::PieceColor::WHITE.get_raw_value()][common::PieceType::QUEEN.get_raw_value()] = 16;
		Pieces[common::PieceColor::WHITE.get_raw_value()][common::PieceType::KING.get_raw_value()] = 8;

		Pieces[common::PieceColor::BLACK.get_raw_value()][common::PieceType::PAWN.get_raw_value()] = 71776119061217280;
		Pieces[common::PieceColor::BLACK.get_raw_value()][common::PieceType::ROOK.get_raw_value()] = 9295429630892703744ULL;
		Pieces[common::PieceColor::BLACK.get_raw_value()][common::PieceType::KNIGHT.get_raw_value()] = 4755801206503243776;
		Pieces[common::PieceColor::BLACK.get_raw_value()][common::PieceType::BISHOP.get_raw_value()] = 2594073385365405696;
		Pieces[common::PieceColor::BLACK.get_raw_value()][common::PieceType::QUEEN.get_raw_value()] = 1152921504606846976;
		Pieces[common::PieceColor::BLACK.get_raw_value()][common::PieceType::KING.get_raw_value()] = 576460752303423488;

		Occupancy[common::PieceColor::WHITE.get_raw_value()] = 65535;
		Occupancy[common::PieceColor::BLACK.get_raw_value()] = 18446462598732840960ULL;
		OccupancySummary = Occupancy[common::PieceColor::WHITE.get_raw_value()] | Occupancy[common::PieceColor::BLACK.get_raw_value()] | Walls;

		EnPassant = 0;
		m_castling = common::Castling::EVERYTHING;
		ColorToMove = common::PieceColor::WHITE;
		MovesCount = 0;
		IrreversibleMovesCount = 0;
		NullMoves = 0;

		CastlingDone[common::PieceColor::WHITE.get_raw_value()] = false;
		CastlingDone[common::PieceColor::BLACK.get_raw_value()] = false;

		CalculatePieceTable(PieceTable);

		//m_hash = ZobristHashing.CalculateHash(this);
		//PawnHash = ZobristHashing.CalculatePawnHash(this);
	}

	void CalculatePieceTable(std::array<common::PieceType, 64> &pieceTable) {
		pieceTable.fill(common::PieceType::INVALID);
		for (int fieldIndex = 0; fieldIndex < 64; fieldIndex++) {
			for (common::PieceType pieceIndex = common::PieceType::PAWN; pieceIndex != common::PieceType::INVALID; ++pieceIndex) {
				common::Bitset bitboard = Pieces[common::PieceColor::WHITE.get_raw_value()][pieceIndex.get_raw_value()] | Pieces[common::PieceColor::BLACK.get_raw_value()][pieceIndex.get_raw_value()];
				if ((bitboard & (1ull << fieldIndex)) != 0) {
					pieceTable[fieldIndex] = pieceIndex;
					break;
				}
			}
		}
	}

	void recalculateEvaluationDependentValues() {
		Material[common::PieceColor::WHITE.get_raw_value()] = calculateMaterial(common::PieceColor::WHITE);
		Material[common::PieceColor::BLACK.get_raw_value()] = calculateMaterial(common::PieceColor::BLACK);

		m_positionEval[common::PieceColor::WHITE.get_raw_value()][common::GamePhase::OPENING] = calculatePosition(common::PieceColor::WHITE, common::GamePhase::OPENING);
		m_positionEval[common::PieceColor::WHITE.get_raw_value()][common::GamePhase::ENDING] = calculatePosition(common::PieceColor::WHITE, common::GamePhase::ENDING);
		m_positionEval[common::PieceColor::BLACK.get_raw_value()][common::GamePhase::OPENING] = calculatePosition(common::PieceColor::BLACK, common::GamePhase::OPENING);
		m_positionEval[common::PieceColor::BLACK.get_raw_value()][common::GamePhase::ENDING] = calculatePosition(common::PieceColor::BLACK, common::GamePhase::ENDING);
	}

	int32_t calculateMaterial(common::PieceColor color) {
		int32_t material = 0;

		for (size_t i = 0; i < 6; i++) {
			material += common::Bitset(Pieces[color.get_raw_value()][i]).count() * ai::score::EvaluationConstants::Pieces[i];
		}

		return material;
	}

	int32_t calculatePosition(common::PieceColor color, common::GamePhase phase) {
		int32_t result = 0;

		for (size_t pieceIndex = 0; pieceIndex < 6; pieceIndex++) {
			common::Bitset pieces(Pieces[color.get_raw_value()][pieceIndex]);
			while (pieces != 0) {
				common::Bitset lsb = pieces.getLsb();
				pieces = pieces.popLsb();
				common::Bitset fieldIndex = lsb.bitScan();

				result += ai::score::piece_square_tables::PieceSquareTablesData::VALUES[pieceIndex][color.get_raw_value()][phase][fieldIndex.asSize()];
			}
		}

		return result;
	}

	void clearWalls() {
		if (Walls > 0) {
			OccupancySummary = OccupancySummary & ~Walls;
			//m_hash = m_hash.ToggleWalls(Walls); // TODO: IMPLEMENT
			Walls = 0;
		}
	}

	void MovePiece(common::PieceColor color, common::PieceType piece, common::Square from, common::Square to) {
		using namespace common;
		using namespace ai::score::piece_square_tables;

		common::Bitset move = from.asBitboard() | to.asBitboard();

		Pieces[color.get_raw_value()][piece.get_raw_value()] ^= move;
		Occupancy[color.get_raw_value()] ^= move;
		OccupancySummary = OccupancySummary ^ move;

		m_positionEval[color.get_raw_value()][GamePhase::OPENING] -= PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][from];
		m_positionEval[color.get_raw_value()][GamePhase::OPENING] += PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][to];

		m_positionEval[color.get_raw_value()][GamePhase::ENDING] -= PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][from];
		m_positionEval[color.get_raw_value()][GamePhase::ENDING] += PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][to];

		PieceTable[from] = PieceType::INVALID;
		PieceTable[to] = piece;
	}

	void addPiece(common::PieceColor color, common::PieceType piece, common::Square fieldIndex) {
		using namespace phase4::engine::common;
		using namespace phase4::engine::ai::score;

		common::Bitset field = fieldIndex.asBitboard();

		Pieces[color.get_raw_value()][piece.get_raw_value()] ^= field;
		Occupancy[color.get_raw_value()] ^= field;
		OccupancySummary = OccupancySummary ^ field;

		Material[color.get_raw_value()] += EvaluationConstants::Pieces[piece.get_raw_value()];

		m_positionEval[color.get_raw_value()][GamePhase::OPENING] += piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][fieldIndex];
		m_positionEval[color.get_raw_value()][GamePhase::ENDING] += piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][fieldIndex];

		PieceTable[fieldIndex] = piece;
	}

	void removePiece(common::PieceColor color, common::PieceType piece, common::Square fieldIndex) {
		using namespace phase4::engine::common;
		using namespace phase4::engine::ai::score;

		common::Bitset field = fieldIndex.asBitboard();

		Pieces[color.get_raw_value()][piece.get_raw_value()] ^= field;
		Occupancy[color.get_raw_value()] ^= field;
		OccupancySummary ^= field;

		Material[color.get_raw_value()] -= EvaluationConstants::Pieces[piece.get_raw_value()];

		m_positionEval[color.get_raw_value()][GamePhase::OPENING] -= piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][fieldIndex];
		m_positionEval[color.get_raw_value()][GamePhase::ENDING] -= piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][fieldIndex];

		PieceTable[fieldIndex] = PieceType::INVALID;
	}

	bool isKingChecked(common::PieceColor color) const {
		const common::Bitset king = Pieces[color.get_raw_value()][common::PieceType::KING.get_raw_value()];
		if (king == 0) {
			return false;
		}

		const uint8_t kingField = king.bitScan();
		return isFieldAttacked(color, common::Square(kingField));
	}

	std::optional<std::tuple<common::PieceColor, common::PieceType>> GetPiece(uint16_t fieldIndex) {
		using namespace common;

		PieceType pieceType = PieceTable[fieldIndex];
		if (pieceType == PieceType::INVALID) {
			return {};
		}

		uint64_t fieldBB = WallOperations::SquareBB(fieldIndex);
		if ((Pieces[PieceColor::WHITE.get_raw_value()][pieceType.get_raw_value()] & fieldBB) > 0) {
			return std::make_tuple(PieceColor::WHITE, pieceType);
		}

		if ((Pieces[PieceColor::BLACK.get_raw_value()][pieceType.get_raw_value()] & fieldBB) > 0) {
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

		PieceType pieceType = PieceTable[move.from()];
		PieceColor enemyColor = ColorToMove.invert();

		if (ColorToMove == PieceColor::WHITE) {
			MovesCount++;
		}

		if (pieceType == PieceType::PAWN || move.flags().isCapture() || move.flags().isCastling()) {
			IrreversibleMovesCount = 0;
		} else {
			IrreversibleMovesCount++;
		}

		if (EnPassant != 0) {
			uint8_t enPassantRank = EnPassant.bitScan() % 8;
			m_hash = m_hash.toggleEnPassant(enPassantRank);
			EnPassant = 0;
		}

		if (move.flags().isSinglePush()) {
			MovePiece(ColorToMove, pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			m_hash = m_hash.movePiece(ColorToMove, pieceType, move.from(), move.to());

			if (pieceType == PieceType::PAWN) {
				PawnHash = PawnHash.movePiece(ColorToMove, pieceType, move.from(), move.to());
			}
		} else if (move.flags().isDoublePush()) {
			MovePiece(ColorToMove, pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			m_hash = m_hash.movePiece(ColorToMove, pieceType, move.from(), move.to());
			PawnHash = PawnHash.movePiece(ColorToMove, pieceType, move.from(), move.to());

			Bitset enPassantField = (ColorToMove == PieceColor::WHITE) ? Square(move.to() - 8).asBitboard() : Square(move.to() + 8).asBitboard();
			uint8_t enPassantFieldIndex = enPassantField.bitScan();

			EnPassant = enPassantField;
			m_hash = m_hash.toggleEnPassant(enPassantFieldIndex % 8);
		} else if (move.flags().isEnPassant()) {
			Square enemyPieceField((ColorToMove == PieceColor::WHITE) ? move.to() - 8 : move.to() + 8);
			PieceType killedPiece = PieceTable[enemyPieceField];

			removePiece(enemyColor, killedPiece, enemyPieceField);
			result.removed.push_back({ killedPiece, enemyPieceField });
			m_hash = m_hash.addOrRemovePiece(enemyColor, killedPiece, enemyPieceField);
			PawnHash = PawnHash.addOrRemovePiece(enemyColor, killedPiece, enemyPieceField);

			MovePiece(ColorToMove, pieceType, move.from(), move.to());
			result.moved.push_back({ move.from(), move.to() });
			m_hash = m_hash.movePiece(ColorToMove, pieceType, move.from(), move.to());
			PawnHash = PawnHash.movePiece(ColorToMove, pieceType, move.from(), move.to());

			result.killed = MakeMoveResult::PieceAndSquare{ killedPiece, enemyPieceField };
		} else if (move.flags().isCapture()) {
			PieceType killedPiece = PieceTable[move.to()];

			removePiece(enemyColor, killedPiece, move.to());
			result.removed.push_back({ killedPiece, move.to() });
			m_hash = m_hash.addOrRemovePiece(enemyColor, killedPiece, move.to());

			if (killedPiece == PieceType::PAWN) {
				PawnHash = PawnHash.addOrRemovePiece(enemyColor, killedPiece, move.to());
			} else if (killedPiece == PieceType::ROOK) {
				switch (move.to()) {
					case 0:
						m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
						m_castling &= ~Castling::WHITE_SHORT;
						break;
					case 7:
						m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
						m_castling &= ~Castling::WHITE_LONG;
						break;
					case 56:
						m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
						m_castling &= ~Castling::BLACK_SHORT;
						break;
					case 63:
						m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
						m_castling &= ~Castling::BLACK_LONG;
						break;
				}
			}

			if (move.flags().isPromotion()) {
				PieceType promotionPiece = move.flags().getPromotionPiece();

				removePiece(ColorToMove, pieceType, move.from());
				result.removed.push_back({ pieceType, move.from() });
				m_hash = m_hash.addOrRemovePiece(ColorToMove, pieceType, move.from());
				PawnHash = PawnHash.addOrRemovePiece(ColorToMove, pieceType, move.from());

				addPiece(ColorToMove, promotionPiece, move.to());
				result.added = { promotionPiece, move.to() };
				m_hash = m_hash.addOrRemovePiece(ColorToMove, promotionPiece, move.to());

				result.promotion = promotionPiece;
			} else {
				MovePiece(ColorToMove, pieceType, move.from(), move.to());
				result.moved.push_back({ move.from(), move.to() });
				m_hash = m_hash.movePiece(ColorToMove, pieceType, move.from(), move.to());

				if (pieceType == PieceType::PAWN) {
					PawnHash = PawnHash.movePiece(ColorToMove, pieceType, move.from(), move.to());
				}
			}

			result.killed = MakeMoveResult::PieceAndSquare{ killedPiece, move.to() };
		} else if (move.flags().isCastling()) {
			// Short castling
			if (move.flags().isKingCastling()) {
				if (ColorToMove == PieceColor::WHITE) {
					MovePiece(PieceColor::WHITE, PieceType::KING, Square::A4, Square::A2);
					MovePiece(PieceColor::WHITE, PieceType::ROOK, Square::A1, Square::A3);

					result.moved.push_back({ Square::A4, Square::A2 });
					result.moved.push_back({ Square::A1, Square::A3 });

					m_hash = m_hash.movePiece(PieceColor::WHITE, PieceType::KING, Square::A4, Square::A2);
					m_hash = m_hash.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::A1, Square::A3);
				} else {
					MovePiece(PieceColor::BLACK, PieceType::KING, Square::H4, Square::H2);
					MovePiece(PieceColor::BLACK, PieceType::ROOK, Square::H1, Square::H3);

					result.moved.push_back({ Square::H4, Square::H2 });
					result.moved.push_back({ Square::H1, Square::H3 });

					m_hash = m_hash.movePiece(PieceColor::BLACK, PieceType::KING, Square::H4, Square::H2);
					m_hash = m_hash.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::H1, Square::H3);
				}
			} else { // Long castling
				if (ColorToMove == PieceColor::WHITE) {
					MovePiece(PieceColor::WHITE, PieceType::KING, Square::A4, Square::A6);
					MovePiece(PieceColor::WHITE, PieceType::ROOK, Square::A8, Square::A5);

					result.moved.push_back({ Square::A4, Square::A6 });
					result.moved.push_back({ Square::A8, Square::A5 });

					m_hash = m_hash.movePiece(PieceColor::WHITE, PieceType::KING, Square::A4, Square::A6);
					m_hash = m_hash.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::A8, Square::A5);
				} else {
					MovePiece(PieceColor::BLACK, PieceType::KING, Square::H4, Square::H6);
					MovePiece(PieceColor::BLACK, PieceType::ROOK, Square::H8, Square::H5);

					result.moved.push_back({ Square::H4, Square::H6 });
					result.moved.push_back({ Square::H8, Square::H5 });

					m_hash = m_hash.movePiece(PieceColor::BLACK, PieceType::KING, Square::H4, Square::H6);
					m_hash = m_hash.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::H8, Square::H5);
				}
			}

			if (ColorToMove == PieceColor::WHITE) {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
				m_castling &= ~Castling::WHITE_CASTLING;
			} else {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
				m_castling &= ~Castling::BLACK_CASTLING;
			}

			CastlingDone[ColorToMove.get_raw_value()] = true;
		} else if (move.flags().isPromotion()) {
			PieceType promotionPiece = move.flags().getPromotionPiece();

			removePiece(ColorToMove, pieceType, move.from());
			result.removed.push_back({ pieceType, move.from() });
			m_hash = m_hash.addOrRemovePiece(ColorToMove, pieceType, move.from());
			PawnHash = PawnHash.addOrRemovePiece(ColorToMove, pieceType, move.from());

			addPiece(ColorToMove, promotionPiece, move.to());
			result.added = { promotionPiece, move.to() };
			m_hash = m_hash.addOrRemovePiece(ColorToMove, promotionPiece, move.to());

			result.promotion = promotionPiece;
		}

		if (pieceType == PieceType::KING && !move.flags().isCastling()) {
			if (ColorToMove == PieceColor::WHITE) {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
				m_castling &= ~Castling::WHITE_CASTLING;
			} else {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
				m_castling &= ~Castling::BLACK_CASTLING;
			}
		} else if (pieceType == PieceType::ROOK && m_castling != Castling::NONE) {
			if (move.from() == Square::A1) {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
				m_castling &= ~Castling::WHITE_SHORT;
			} else if (move.from() == Square::A8) {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
				m_castling &= ~Castling::WHITE_LONG;
			} else if (move.from() == Square::H1) {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
				m_castling &= ~Castling::BLACK_SHORT;
			} else if (move.from() == Square::H8) {
				m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
				m_castling &= ~Castling::BLACK_LONG;
			}
		}

		if (likely(Walls > 0)) {
			uint8_t wallIndex = Walls.bitScan();
			const FieldIndex wallMove = WallOperations::SLIDE_DIR[wallIndex][move.to()];
			if (wallMove != FieldIndex::ZERO) {
				OccupancySummary = OccupancySummary & ~Walls;

				Bitset original = WallOperations::SLIDE_TO[wallIndex][move.to()];
				while (original > 0) {
					Square from(original.bitScan());
					Square to = WallOperations::SLIDE_SQUARE[wallIndex][from];
					result.moved.push_back(MakeMoveResult::Movement{ from, to });
					if (auto pieceResult = GetPiece(from)) {
						auto [resultColor, resultType] = *pieceResult;
						MovePiece(resultColor, resultType, from, to);
						result.moved.push_back({ from, to });
						m_hash = m_hash.movePiece(resultColor, resultType, from, to);
						if (resultType == PieceType::PAWN) {
							PawnHash = PawnHash.movePiece(resultColor, resultType, from, to);
						}

						if (resultType == PieceType::PAWN && EnPassant != 0) {
							int offset = (resultColor == PieceColor::WHITE) ? -8 : 8;
							uint8_t enPassantField = EnPassant.bitScan();
							if (enPassantField == from.get_raw_value() + offset) {
								uint8_t enPassantRank = enPassantField % 8;
								m_hash = m_hash.toggleEnPassant(enPassantRank);

								if (wallMove.offset() >= 0)
									EnPassant = EnPassant >> wallMove.offset();
								else
									EnPassant = EnPassant << -wallMove.offset();
								m_hash = m_hash.toggleEnPassant(EnPassant.bitScan() % 8);
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
							if (from == 0) {
								m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_SHORT);
								m_castling &= ~Castling::WHITE_SHORT;
							} else if (from == 7) {
								m_hash = m_hash.removeCastlingFlag(m_castling, Castling::WHITE_LONG);
								m_castling &= ~Castling::WHITE_LONG;
							} else if (from == 56) {
								m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_SHORT);
								m_castling &= ~Castling::BLACK_SHORT;
							} else if (from == 63) {
								m_hash = m_hash.removeCastlingFlag(m_castling, Castling::BLACK_LONG);
								m_castling &= ~Castling::BLACK_LONG;
							}
						}
					}
					original = original.popLsb();
				}

				m_hash = m_hash.toggleWalls(Walls); // Turn off previous wall
				Walls = WallOperations::SLIDE_TO[wallIndex][move.to()];
				m_hash = m_hash.toggleWalls(Walls); // Turn on new wall location
				OccupancySummary = OccupancySummary | Walls;
			}
			result.slide = wallMove;
		} else {
			result.slide = common::FieldIndex::ZERO;
		}

		ColorToMove = enemyColor;
		m_hash = m_hash.changeSide();

		return result;
	}

	bool SlideWall(common::FieldIndex wallMove) {
		(void)wallMove;
		return false;
		/*if (Walls.count() != 4)
			return false;

		int slideCount = 0;
		std::array<Move, 4> moves;

		Walls.reset(wallMove.Offset());

		std::bitset<64> walls = Walls;
		if (wallMove.Offset() >= 0)
			walls >>= wallMove.Offset();
		else
			walls <<= -wallMove.Offset();

		std::bitset<64> original = walls;
		while (original.any()) {
			int from = original._Find_first();
			int to = from + wallMove.Offset();
			moves[slideCount++] = Move(from, to, 0);
			// GetPiece function not provided, implement it accordingly
			// MovePiece function not provided, implement it accordingly
			original.reset(from);
		}

		m_hash ^= Walls.to_ullong(); // Turn off previous wall
		Walls = walls;
		m_hash ^= Walls.to_ullong(); // Turn on new wall location

		if (emitSignal) {
			// Slide function not provided, implement it accordingly
			// Slide(moves);
		}
		return true;*/
	}

	bool isFieldAttacked(common::PieceColor color, common::Square fieldIndex) const {
		using namespace common;

		PieceColor enemyColor = color.invert();

		const Bitset fileRankAttacks = moves::MovesGenerator::getRookMoves(OccupancySummary, fieldIndex) & Occupancy[enemyColor.get_raw_value()];
		const Bitset attackingRooks = fileRankAttacks & (Pieces[enemyColor.get_raw_value()][PieceType::ROOK.get_raw_value()] | Pieces[enemyColor.get_raw_value()][PieceType::QUEEN.get_raw_value()]);
		if (attackingRooks != 0) {
			return true;
		}

		const Bitset diagonalAttacks = moves::MovesGenerator::getBishopMoves(OccupancySummary, fieldIndex) & Occupancy[enemyColor.get_raw_value()];
		const Bitset attackingBishops = diagonalAttacks & (Pieces[enemyColor.get_raw_value()][PieceType::BISHOP.get_raw_value()] | Pieces[enemyColor.get_raw_value()][PieceType::QUEEN.get_raw_value()]);
		if (attackingBishops != 0) {
			return true;
		}

		const Bitset jumpAttacks = moves::MovesGenerator::getKnightMoves(fieldIndex);
		const Bitset attackingKnights = jumpAttacks & Pieces[enemyColor.get_raw_value()][PieceType::KNIGHT.get_raw_value()];
		if (attackingKnights != 0) {
			return true;
		}

		const Bitset boxAttacks = moves::MovesGenerator::getKingMoves(fieldIndex);
		const Bitset attackingKings = boxAttacks & Pieces[enemyColor.get_raw_value()][PieceType::KING.get_raw_value()];
		if (attackingKings != 0) {
			return true;
		}

		const Bitset field = fieldIndex.asBitboard();
		const Bitset potentialPawns = boxAttacks & Pieces[enemyColor.get_raw_value()][PieceType::PAWN.get_raw_value()];
		const Bitset attackingPawns = (color.get_raw_value() == PieceColor::WHITE.get_raw_value()) ? field & ((potentialPawns >> 7) | (potentialPawns >> 9)) : field & ((potentialPawns << 7) | (potentialPawns << 9));

		return (attackingPawns != 0);
	}
};

} //namespace phase4::engine::board

#endif
