#ifndef PHASE4_ENGINE_BOARD_POSITION_STATE_H
#define PHASE4_ENGINE_BOARD_POSITION_STATE_H

#include <phase4/engine/board/piece_square_tables/piece_square_table_data.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/zobrist_hashing.h>

#include <phase4/engine/common/util.h>

namespace phase4::engine::board {

class PositionState {
public:
	static const Position DEFAULT;

	static constexpr void setDefaultState(Position &position);

	static constexpr void recalculateEvaluationDependentValues(Position &position);

private:
	static constexpr ZobristHashing calculateHash(const Position &position);

	static constexpr ZobristHashing calculatePawnHash(const Position &position);

	/// @brief Calculates the piece table based on piece masks
	/// @param position the Position to update and compute from
	static constexpr void calculatePieceTable(Position &position);

	static constexpr int32_t calculateMaterial(const Position &position, common::PieceColor color);

	static constexpr int32_t calculatePosition(const Position &position, common::PieceColor color, common::GamePhase phase);

	static constexpr Position makeDefaultPosition();
};

inline constexpr void PositionState::calculatePieceTable(Position &position) {
	for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
		for (common::PieceType pieceIndex = common::PieceType::PAWN; pieceIndex != common::PieceType::INVALID; ++pieceIndex) {
			common::Bitset bitboard = position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][pieceIndex.get_raw_value()] | position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][pieceIndex.get_raw_value()];
			if ((bitboard & fieldIndex.asBitboard()) != 0) {
				position.m_pieceTable[fieldIndex] = pieceIndex;
				break;
			}
		}
	}
}

inline constexpr int32_t PositionState::calculateMaterial(const Position &position, common::PieceColor color) {
	int32_t material = 0;

	for (size_t i = 0; i < 6; i++) {
		material += common::Bitset(position.m_colorPieceMasks[color.get_raw_value()][i]).count() * board::EvaluationConstants::PIECE_VALUES[i];
	}

	return material;
}

inline constexpr int32_t PositionState::calculatePosition(const Position &position, common::PieceColor color, common::GamePhase phase) {
	int32_t result = 0;

	for (size_t pieceIndex = 0; pieceIndex < 6; pieceIndex++) {
		common::Bitset pieces(position.m_colorPieceMasks[color.get_raw_value()][pieceIndex]);
		while (pieces != 0) {
			common::Bitset lsb = pieces.getLsb();
			pieces = pieces.popLsb();
			common::Bitset fieldIndex = lsb.bitScan();

			result += piece_square_tables::PieceSquareTablesData::VALUES[pieceIndex][color.get_raw_value()][phase][fieldIndex.asSize()];
		}
	}

	return result;
}

inline constexpr void PositionState::recalculateEvaluationDependentValues(Position &position) {
	position.m_material[common::PieceColor::WHITE.get_raw_value()] = calculateMaterial(position, common::PieceColor::WHITE);
	position.m_material[common::PieceColor::BLACK.get_raw_value()] = calculateMaterial(position, common::PieceColor::BLACK);

	position.m_positionEval[common::PieceColor::WHITE.get_raw_value()][common::GamePhase::OPENING] = calculatePosition(position, common::PieceColor::WHITE, common::GamePhase::OPENING);
	position.m_positionEval[common::PieceColor::WHITE.get_raw_value()][common::GamePhase::ENDING] = calculatePosition(position, common::PieceColor::WHITE, common::GamePhase::ENDING);
	position.m_positionEval[common::PieceColor::BLACK.get_raw_value()][common::GamePhase::OPENING] = calculatePosition(position, common::PieceColor::BLACK, common::GamePhase::OPENING);
	position.m_positionEval[common::PieceColor::BLACK.get_raw_value()][common::GamePhase::ENDING] = calculatePosition(position, common::PieceColor::BLACK, common::GamePhase::ENDING);
}

inline constexpr ZobristHashing PositionState::calculateHash(const Position &position) {
	using namespace common;

	(void)position;
	ZobristHashing hash(0);

	for (PieceColor color = PieceColor::WHITE; color != PieceColor::INVALID; ++color) {
		for (PieceType piece = PieceType::PAWN; piece != PieceType::INVALID; ++piece) {
			Bitset piecesToParse = position.m_colorPieceMasks[color.get_raw_value()][piece.get_raw_value()];
			while (piecesToParse != 0) {
				const Bitset lsb = piecesToParse.getLsb(); // TODO: skip lsb
				piecesToParse = piecesToParse.popLsb();

				Square fieldIndex(lsb.bitScan());
				hash = hash.addOrRemovePiece(color, piece, fieldIndex);
			}
		}
	}
	/*
		if ((position.m_castling & Castling::WHITE_SHORT) != 0) {
			result ^= _castlingHashes[0];
		}
		if ((position.m_castling & Castling::WHITE_LONG) != 0) {
			result ^= _castlingHashes[1];
		}
		if ((position.m_castling & Castling::BLACK_SHORT) != 0) {
			result ^= _castlingHashes[2];
		}
		if ((position.m_castling & Castling::BLACK_LONG) != 0) {
			result ^= _castlingHashes[3];
		}
*/
		if (position.m_enPassant != 0) {
			uint8_t fieldIndex = position.m_enPassant.bitScan();
			hash = hash.toggleEnPassant(fieldIndex % 8);
		}

		if (position.m_walls > 0) {
			hash = hash.toggleWalls(position.m_walls);
		}

		if (position.m_colorToMove == PieceColor::BLACK) {
			hash = hash.changeSide();
		}
	
	return hash;
}

inline constexpr ZobristHashing PositionState::calculatePawnHash(const Position &position) {
	using namespace common;

	ZobristHashing pawnHash(0);

	for (PieceColor color = PieceColor::WHITE; color != PieceColor::INVALID; ++color) {
		Bitset piecesToParse = position.m_colorPieceMasks[color.get_raw_value()][PieceType::PAWN.get_raw_value()];
		while (piecesToParse != 0) {
			const Bitset lsb = piecesToParse.getLsb(); // TODO: skip lsb
			piecesToParse = piecesToParse.popLsb();

			const Square fieldIndex(lsb.bitScan());
			pawnHash = pawnHash.addOrRemovePiece(color, PieceType::PAWN, fieldIndex);
		}
	}

	return pawnHash;
}

inline constexpr void PositionState::setDefaultState(Position &position) {
	position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::PAWN.get_raw_value()] = 65280;
	position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::ROOK.get_raw_value()] = 129;
	position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::KNIGHT.get_raw_value()] = 66;
	position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::BISHOP.get_raw_value()] = 36;
	position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::QUEEN.get_raw_value()] = 16;
	position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::KING.get_raw_value()] = 8;

	position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::PAWN.get_raw_value()] = 71776119061217280;
	position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::ROOK.get_raw_value()] = 9295429630892703744ULL;
	position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::KNIGHT.get_raw_value()] = 4755801206503243776;
	position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::BISHOP.get_raw_value()] = 2594073385365405696;
	position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::QUEEN.get_raw_value()] = 1152921504606846976;
	position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::KING.get_raw_value()] = 576460752303423488;

	position.m_occupancyByColor[common::PieceColor::WHITE.get_raw_value()] = 65535;
	position.m_occupancyByColor[common::PieceColor::BLACK.get_raw_value()] = 18446462598732840960ULL;
	position.m_occupancySummary = position.m_occupancyByColor[common::PieceColor::WHITE.get_raw_value()] | position.m_occupancyByColor[common::PieceColor::BLACK.get_raw_value()] | position.m_walls;

	position.m_enPassant = 0;
	position.m_castling = common::Castling::EVERYTHING;
	position.m_colorToMove = common::PieceColor::WHITE;
	position.m_movesCount = 0;
	position.m_irreversibleMovesCount = 0;
	position.m_nullMoves = 0;

	position.m_castlingDone[common::PieceColor::WHITE.get_raw_value()] = false;
	position.m_castlingDone[common::PieceColor::BLACK.get_raw_value()] = false;

	calculatePieceTable(position);

	position.m_hash = calculateHash(position);
	position.m_pawnHash = calculatePawnHash(position);

	recalculateEvaluationDependentValues(position);
}

inline constexpr Position PositionState::makeDefaultPosition() {
	Position position;
	setDefaultState(position);
	return position;
}

inline constexpr Position PositionState::DEFAULT = makeDefaultPosition();

} //namespace phase4::engine::board

#endif
