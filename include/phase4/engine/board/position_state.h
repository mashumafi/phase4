#ifndef PHASE4_ENGINE_BOARD_POSITION_STATE_H
#define PHASE4_ENGINE_BOARD_POSITION_STATE_H

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/zobrist_hashing.h>

namespace phase4::engine::board {

class PositionState {
public:
	static const Position DEFAULT;

	static constexpr void setDefaultState(Position &position);

	static constexpr void recalculateEvaluationDependentValues(Position &position);

private:
	static constexpr ZobristHashing calculateHash(Position &position);

	static constexpr ZobristHashing calculatePawnHash(Position &position);

	static constexpr void calculatePieceTable(Position &position);

	static constexpr int32_t calculateMaterial(const Position &position, common::PieceColor color);

	static constexpr int32_t calculatePosition(const Position &position, common::PieceColor color, common::GamePhase phase);

	static constexpr Position makeDefaultPosition();
};

constexpr void PositionState::calculatePieceTable(Position &position) {
	//position.m_pieceTable.fill(common::PieceType::INVALID);
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

constexpr int32_t PositionState::calculateMaterial(const Position &position, common::PieceColor color) {
	int32_t material = 0;

	for (size_t i = 0; i < 6; i++) {
		material += common::Bitset(position.m_colorPieceMasks[color.get_raw_value()][i]).count() * ai::score::EvaluationConstants::Pieces[i];
	}

	return material;
}

constexpr int32_t PositionState::calculatePosition(const Position &position, common::PieceColor color, common::GamePhase phase) {
	int32_t result = 0;

	for (size_t pieceIndex = 0; pieceIndex < 6; pieceIndex++) {
		common::Bitset pieces(position.m_colorPieceMasks[color.get_raw_value()][pieceIndex]);
		while (pieces != 0) {
			common::Bitset lsb = pieces.getLsb();
			pieces = pieces.popLsb();
			common::Bitset fieldIndex = lsb.bitScan();

			result += ai::score::piece_square_tables::PieceSquareTablesData::VALUES[pieceIndex][color.get_raw_value()][phase][fieldIndex.asSize()];
		}
	}

	return result;
}

constexpr void PositionState::recalculateEvaluationDependentValues(Position &position) {
	position.m_material[common::PieceColor::WHITE.get_raw_value()] = calculateMaterial(position, common::PieceColor::WHITE);
	position.m_material[common::PieceColor::BLACK.get_raw_value()] = calculateMaterial(position, common::PieceColor::BLACK);

	position.m_positionEval[common::PieceColor::WHITE.get_raw_value()][common::GamePhase::OPENING] = calculatePosition(position, common::PieceColor::WHITE, common::GamePhase::OPENING);
	position.m_positionEval[common::PieceColor::WHITE.get_raw_value()][common::GamePhase::ENDING] = calculatePosition(position, common::PieceColor::WHITE, common::GamePhase::ENDING);
	position.m_positionEval[common::PieceColor::BLACK.get_raw_value()][common::GamePhase::OPENING] = calculatePosition(position, common::PieceColor::BLACK, common::GamePhase::OPENING);
	position.m_positionEval[common::PieceColor::BLACK.get_raw_value()][common::GamePhase::ENDING] = calculatePosition(position, common::PieceColor::BLACK, common::GamePhase::ENDING);
}

constexpr ZobristHashing PositionState::calculateHash(Position &position) {
	(void)position;
	ZobristHashing hash;
	return hash;
}

constexpr ZobristHashing PositionState::calculatePawnHash(Position &position) {
	(void)position;
	ZobristHashing pawnHash;
	return pawnHash;
}

constexpr void PositionState::setDefaultState(Position &position) {
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

constexpr Position PositionState::makeDefaultPosition() {
	Position position;
	setDefaultState(position);
	return position;
}

inline const Position PositionState::DEFAULT = makeDefaultPosition();

} //namespace phase4::engine::board

#endif