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

	static constexpr void setDefaultState(Position &position) noexcept;

	static constexpr void recalculateEvaluationDependentValues(Position &position) noexcept;

	/// @brief Calculates the piece table based on piece masks
	/// @param position the Position to update and compute from
	static constexpr void calculatePieceTable(Position &position) noexcept;

	static constexpr ZobristHashing calculateHash(const Position &position) noexcept;

	static constexpr ZobristHashing calculatePawnHash(const Position &position) noexcept;

private:
	static constexpr int32_t calculateMaterial(const Position &position, common::PieceColor color) noexcept;

	static constexpr int32_t calculatePosition(const Position &position, common::PieceColor color, common::GamePhase phase) noexcept;

	static constexpr Position makeDefaultPosition() noexcept;
};

inline constexpr void PositionState::calculatePieceTable(Position &position) noexcept {
	for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
		for (common::PieceType pieceIndex = common::PieceType::PAWN; pieceIndex != common::PieceType::INVALID; ++pieceIndex) {
			common::Bitboard bitboard = position.colorPieceMask(common::PieceColor::WHITE, pieceIndex) | position.colorPieceMask(common::PieceColor::BLACK, pieceIndex);
			if ((bitboard & fieldIndex.asBitboard()) != 0) {
				position.pieceTable(fieldIndex) = pieceIndex;
				break;
			}
		}
	}
}

inline constexpr int32_t PositionState::calculateMaterial(const Position &position, common::PieceColor color) noexcept {
	int32_t material = 0;

	for (common::PieceType piece = common::PieceType::PAWN; piece != common::PieceType::INVALID; ++piece) {
		material += common::Bitboard(position.colorPieceMask(color, piece)).count() * board::EvaluationConstants::pieceValue(piece);
	}

	return material;
}

inline constexpr int32_t PositionState::calculatePosition(const Position &position, common::PieceColor color, common::GamePhase phase) noexcept {
	int32_t result = 0;

	for (common::PieceType pieceIndex = common::PieceType::PAWN; pieceIndex != common::PieceType::INVALID; ++pieceIndex) {
		common::Bitboard pieces(position.colorPieceMask(color, pieceIndex));
		while (pieces != 0) {
			common::Bitboard lsb = pieces.getLsb(); // TODO: Skip lsb
			pieces = pieces.popLsb();
			common::Square fieldIndex(lsb.bitScan());

			result += piece_square_tables::PieceSquareTablesData::value(pieceIndex, color, phase, fieldIndex);
		}
	}

	return result;
}

inline constexpr void PositionState::recalculateEvaluationDependentValues(Position &position) noexcept {
	position.material(common::PieceColor::WHITE) = calculateMaterial(position, common::PieceColor::WHITE);
	position.material(common::PieceColor::BLACK) = calculateMaterial(position, common::PieceColor::BLACK);

	position.positionEval(common::PieceColor::WHITE, common::GamePhase::OPENING) = calculatePosition(position, common::PieceColor::WHITE, common::GamePhase::OPENING);
	position.positionEval(common::PieceColor::WHITE, common::GamePhase::ENDING) = calculatePosition(position, common::PieceColor::WHITE, common::GamePhase::ENDING);
	position.positionEval(common::PieceColor::BLACK, common::GamePhase::OPENING) = calculatePosition(position, common::PieceColor::BLACK, common::GamePhase::OPENING);
	position.positionEval(common::PieceColor::BLACK, common::GamePhase::ENDING) = calculatePosition(position, common::PieceColor::BLACK, common::GamePhase::ENDING);
}

inline constexpr ZobristHashing PositionState::calculateHash(const Position &position) noexcept {
	using namespace common;

	ZobristHashing hash(0);

	for (PieceColor color = PieceColor::WHITE; color != PieceColor::INVALID; ++color) {
		for (PieceType piece = PieceType::PAWN; piece != PieceType::INVALID; ++piece) {
			Bitboard piecesToParse = position.colorPieceMask(color, piece);
			while (piecesToParse != 0) {
				const Bitboard lsb = piecesToParse.getLsb(); // TODO: skip lsb
				piecesToParse = piecesToParse.popLsb();

				Square fieldIndex(lsb.bitScan());
				hash = hash.addOrRemovePiece(color, piece, fieldIndex);
			}
		}
	}

	if ((position.castling() & Castling::WHITE_SHORT) != Castling::NONE) {
		hash = hash.toggleCastlingFlag(Castling::WHITE_SHORT);
	}
	if ((position.castling() & Castling::WHITE_LONG) != Castling::NONE) {
		hash = hash.toggleCastlingFlag(Castling::WHITE_LONG);
	}
	if ((position.castling() & Castling::BLACK_SHORT) != Castling::NONE) {
		hash = hash.toggleCastlingFlag(Castling::BLACK_SHORT);
	}
	if ((position.castling() & Castling::BLACK_LONG) != Castling::NONE) {
		hash = hash.toggleCastlingFlag(Castling::BLACK_LONG);
	}

	if (position.enPassant() != 0) {
		uint8_t fieldIndex = position.enPassant().bitScan();
		hash = hash.toggleEnPassant(fieldIndex % 8);
	}

	if (position.walls() > 0) {
		hash = hash.slowToggleWalls(position.walls());
	}

	if (position.colorToMove() == PieceColor::BLACK) {
		hash = hash.changeSide();
	}

	return hash;
}

inline constexpr ZobristHashing PositionState::calculatePawnHash(const Position &position) noexcept {
	using namespace common;

	ZobristHashing pawnHash(0);

	for (PieceColor color = PieceColor::WHITE; color != PieceColor::INVALID; ++color) {
		Bitboard piecesToParse = position.colorPieceMask(color, PieceType::PAWN);
		while (piecesToParse != 0) {
			const Bitboard lsb = piecesToParse.getLsb(); // TODO: skip lsb
			piecesToParse = piecesToParse.popLsb();

			const Square fieldIndex(lsb.bitScan());
			pawnHash = pawnHash.addOrRemovePiece(color, PieceType::PAWN, fieldIndex);
		}
	}

	return pawnHash;
}

inline constexpr void PositionState::setDefaultState(Position &position) noexcept {
	position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::PAWN) = 65280;
	position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::ROOK) = 129;
	position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::KNIGHT) = 66;
	position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::BISHOP) = 36;
	position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::QUEEN) = 16;
	position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::KING) = 8;

	position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::PAWN) = 71776119061217280;
	position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::ROOK) = 9295429630892703744ULL;
	position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::KNIGHT) = 4755801206503243776;
	position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::BISHOP) = 2594073385365405696;
	position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::QUEEN) = 1152921504606846976;
	position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::KING) = 576460752303423488;

	position.occupancy(common::PieceColor::WHITE) = 65535;
	position.occupancy(common::PieceColor::BLACK) = 18446462598732840960ULL;
	position.occupancySummary() = position.occupancy(common::PieceColor::WHITE) | position.occupancy(common::PieceColor::BLACK);

	position.enPassant() = 0;
	position.castling() = common::Castling::EVERYTHING;
	position.colorToMove() = common::PieceColor::WHITE;
	position.movesCount() = 1;
	position.irreversibleMovesCount() = 0;
	position.nullMoves() = 0;

	position.castlingDone(common::PieceColor::WHITE) = false;
	position.castlingDone(common::PieceColor::BLACK) = false;

	calculatePieceTable(position);

	position.hash() = calculateHash(position);
	position.pawnHash() = calculatePawnHash(position);

	recalculateEvaluationDependentValues(position);
}

inline constexpr Position PositionState::makeDefaultPosition() noexcept {
	Position position;
	setDefaultState(position);
	return position;
}

inline constexpr Position PositionState::DEFAULT = makeDefaultPosition();

} //namespace phase4::engine::board

#endif
