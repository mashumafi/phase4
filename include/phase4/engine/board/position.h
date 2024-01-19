#ifndef PHASE4_ENGINE_BOARD_POSITION_H
#define PHASE4_ENGINE_BOARD_POSITION_H

#include <phase4/engine/ai/score/evaluation_constants.h> // TODO: remove cycle
#include <phase4/engine/ai/score/piece_square_tables/piece_square_table_data.h>
#include <phase4/engine/board/zobrist_hashing.h>
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/game_phase.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/moves/moves_generator.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board {

class Position {
public:
	ZobristHashing Hash;
	uint64_t PawnHash;

	common::Bitset Pieces[2][6];
	common::Bitset Occupancy[2];
	common::Bitset OccupancySummary;
	common::Bitset EnPassant;
	common::Castling Castling = common::Castling::NONE;
	common::PieceColor ColorToMove = common::PieceColor::WHITE;
	int MovesCount;
	int IrreversibleMovesCount;
	int NullMoves;

	bool CastlingDone[2];
	int Material[2];
	int32_t Position[2][2];

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
		Castling = common::Castling::EVERYTHING;
		ColorToMove = common::PieceColor::WHITE;
		MovesCount = 0;
		IrreversibleMovesCount = 0;
		NullMoves = 0;

		CastlingDone[common::PieceColor::WHITE.get_raw_value()] = false;
		CastlingDone[common::PieceColor::BLACK.get_raw_value()] = false;

		CalculatePieceTable(PieceTable);

		//Hash = ZobristHashing.CalculateHash(this);
		//PawnHash = ZobristHashing.CalculatePawnHash(this);
	}

	void CalculatePieceTable(std::array<common::PieceType, 64> &pieceTable) {
		pieceTable.fill(common::PieceType::EMPTY);
		for (int fieldIndex = 0; fieldIndex < 64; fieldIndex++) {
			for (common::PieceType pieceIndex : common::PieceType::PIECES) {
				common::Bitset bitboard = Pieces[common::PieceColor::WHITE.get_raw_value()][pieceIndex.get_raw_value()] | Pieces[common::PieceColor::BLACK.get_raw_value()][pieceIndex.get_raw_value()];
				if ((bitboard & (1ul << fieldIndex)) != 0) {
					pieceTable[fieldIndex] = pieceIndex;
					break;
				}
			}
		}
	}

	void recalculateEvaluationDependentValues() {
		Material[common::PieceColor::WHITE.get_raw_value()] = calculateMaterial(common::PieceColor::WHITE);
		Material[common::PieceColor::BLACK.get_raw_value()] = calculateMaterial(common::PieceColor::BLACK);

		Position[common::PieceColor::WHITE.get_raw_value()][common::GamePhase::OPENING] = calculatePosition(common::PieceColor::WHITE, common::GamePhase::OPENING);
		Position[common::PieceColor::WHITE.get_raw_value()][common::GamePhase::ENDING] = calculatePosition(common::PieceColor::WHITE, common::GamePhase::ENDING);
		Position[common::PieceColor::BLACK.get_raw_value()][common::GamePhase::OPENING] = calculatePosition(common::PieceColor::BLACK, common::GamePhase::OPENING);
		Position[common::PieceColor::BLACK.get_raw_value()][common::GamePhase::ENDING] = calculatePosition(common::PieceColor::BLACK, common::GamePhase::ENDING);
	}

	int32_t calculateMaterial(common::PieceColor color) {
		int32_t material = 0;

		for (size_t i = 0; i < 6; i++) {
			material += common::Bitset(Pieces[color.get_raw_value()][i]).count() * ai::score::EvaluationConstants::Pieces[i];
		}

		return material;
	}

	int calculatePosition(common::PieceColor color, common::GamePhase phase) {
		uint32_t result = 0;

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
			//Hash = Hash.ToggleWalls(Walls); // TODO: IMPLEMENT
			Walls = 0;
		}
	}

	void MovePiece(common::PieceColor color, common::PieceType piece, common::Square from, common::Square to) {
		using namespace common;
		using namespace ai::score::piece_square_tables;

		common::Bitset move = from.asBitboard() | to.asBitboard();

		Pieces[color.get_raw_value()][piece.get_raw_value()] = Pieces[color.get_raw_value()][piece.get_raw_value()] ^ move;
		Occupancy[color.get_raw_value()] = Occupancy[color.get_raw_value()] ^ move;
		OccupancySummary = OccupancySummary ^ move;

		Position[color.get_raw_value()][GamePhase::OPENING] -= PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][from];
		Position[color.get_raw_value()][GamePhase::OPENING] += PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][to];

		Position[color.get_raw_value()][GamePhase::ENDING] -= PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][from];
		Position[color.get_raw_value()][GamePhase::ENDING] += PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][to];

		PieceTable[from] = PieceType::EMPTY;
		PieceTable[to] = piece;
	}

	void AddPiece(common::PieceColor color, common::PieceType piece, common::Square fieldIndex) {
		using namespace phase4::engine::common;
		using namespace phase4::engine::ai::score;

		common::Bitset field = fieldIndex.asBitboard();

		Pieces[color.get_raw_value()][piece.get_raw_value()] = Pieces[color.get_raw_value()][piece.get_raw_value()] ^ field;
		Occupancy[color.get_raw_value()] = Occupancy[color.get_raw_value()] ^ field;
		OccupancySummary = OccupancySummary ^ field;

		Material[color.get_raw_value()] += EvaluationConstants::Pieces[piece.get_raw_value()];

		Position[color.get_raw_value()][GamePhase::OPENING] += piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][fieldIndex];
		Position[color.get_raw_value()][GamePhase::ENDING] += piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][fieldIndex];

		PieceTable[fieldIndex] = piece;
	}

	void RemovePiece(common::PieceColor color, common::PieceType piece, common::Square fieldIndex) {
		using namespace phase4::engine::common;
		using namespace phase4::engine::ai::score;

		common::Bitset field = fieldIndex.asBitboard();

		Pieces[color.get_raw_value()][piece.get_raw_value()] = Pieces[color.get_raw_value()][piece.get_raw_value()] ^ field;
		Occupancy[color.get_raw_value()] = Occupancy[color.get_raw_value()] ^ field;
		OccupancySummary = OccupancySummary ^ field;

		Material[color.get_raw_value()] -= EvaluationConstants::Pieces[piece.get_raw_value()];

		Position[color.get_raw_value()][GamePhase::OPENING] -= piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][fieldIndex];
		Position[color.get_raw_value()][GamePhase::ENDING] -= piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][fieldIndex];

		PieceTable[fieldIndex] = PieceType::EMPTY;
	}

	bool isKingChecked(common::PieceColor color) {
		const common::Bitset king = Pieces[color.get_raw_value()][common::PieceType::KING.get_raw_value()];
		if (king == 0) {
			return false;
		}

		const uint8_t kingField = king.bitScan();
		return isFieldAttacked(color, common::Square(kingField));
	}

	bool isFieldAttacked(common::PieceColor color, common::Square fieldIndex) {
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
