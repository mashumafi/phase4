#ifndef PHASE4_ENGINE_BOARD_SESSION_H
#define PHASE4_ENGINE_BOARD_SESSION_H

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_state.h>
#include <phase4/engine/board/zobrist_hashing.h>

#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/fast_vector.h>
#include <phase4/engine/common/field_index.h>

#include <cstdint>

namespace phase4::engine::board {

class Session {
public:
	Session() {
	}

	void setDefaultState() {
		m_position = PositionState::DEFAULT;

		m_killedPieces.clear();
		m_enPassants.clear();
		m_castlings.clear();
		m_promotedPieces.clear();
		m_hashes.clear();
		m_pawnHashes.clear();
		m_irreversibleMovesCounts.clear();
		m_wallSlides.clear();
	}

	bool isKingChecked(common::PieceColor color) const {
		return m_position.isKingChecked(color);
	}

	bool isMoveLegal(moves::Move move) const {
		Position positionCopy = Position(m_position);
		positionCopy.makeMove(move);
		return !positionCopy.isKingChecked(m_position.m_colorToMove);
	}

	Position::MakeMoveResult makeMove(moves::Move move) {
		m_castlings.push_back(m_position.m_castling);
		m_hashes.push_back(m_position.m_hash);
		m_pawnHashes.push_back(m_position.m_pawnHash);
		m_enPassants.push_back(m_position.m_enPassant);
		m_irreversibleMovesCounts.push_back(m_position.m_irreversibleMovesCount);

		Position::MakeMoveResult details = m_position.makeMove(move);
		if (unlikely(details.promotion))
			m_promotedPieces.push_back(*details.promotion);

		if (unlikely(details.killed))
			m_killedPieces.push_back(details.killed->pieceType);

		if (unlikely(details.slide))
			m_wallSlides.push_back(*details.slide);

		return details;
	}

	void undoMove(moves::Move move) {
		using namespace common;

		FieldIndex wallMove = m_wallSlides.pop_back();
		if (wallMove != FieldIndex::ZERO) {
			m_position.slideWall(wallMove);
		}

		PieceType pieceType = m_position.m_pieceTable[move.to()];
		m_position.m_colorToMove = m_position.m_colorToMove;

		if (move.flags().isSinglePush() || move.flags().isDoublePush()) {
			m_position.MovePiece(m_position.m_colorToMove, pieceType, move.to(), move.from());
		} else if (move.flags().isEnPassant()) {
			const PieceColor enemyColor = m_position.m_colorToMove.invert();
			const Square enemyPieceField(m_position.m_colorToMove == PieceColor::WHITE ? static_cast<uint8_t>(move.to() - 8) : static_cast<uint8_t>(move.to() + 8));
			const PieceType killedPiece = m_killedPieces.pop_back();

			m_position.MovePiece(m_position.m_colorToMove, PieceType::PAWN, move.to(), move.from());
			m_position.addPiece(enemyColor, killedPiece, enemyPieceField);
		} else if (move.flags().isCapture()) {
			PieceColor enemyColor = m_position.m_colorToMove.invert();
			const PieceType killedPiece = m_killedPieces.pop_back();

			// Promotion
			if (move.flags().isPromotion()) {
				const PieceType promotionPiece = m_promotedPieces.pop_back();
				m_position.removePiece(m_position.m_colorToMove, promotionPiece, move.to());
				m_position.addPiece(m_position.m_colorToMove, PieceType::PAWN, move.from());
			} else {
				m_position.MovePiece(m_position.m_colorToMove, pieceType, move.to(), move.from());
			}

			m_position.addPiece(enemyColor, killedPiece, move.to());
		} else if (move.flags().isCastling()) {
			// Short castling
			if (move.flags().isKingCastling()) {
				if (m_position.m_colorToMove == PieceColor::WHITE) {
					m_position.MovePiece(PieceColor::WHITE, PieceType::KING, Square::G1, Square::E1);
					m_position.MovePiece(PieceColor::WHITE, PieceType::ROOK, Square::F1, Square::H1);
				} else {
					m_position.MovePiece(PieceColor::BLACK, PieceType::KING, Square::G8, Square::E8);
					m_position.MovePiece(PieceColor::BLACK, PieceType::ROOK, Square::F8, Square::H8);
				}
			}
			// Long castling
			else {
				if (m_position.m_colorToMove == PieceColor::WHITE) {
					m_position.MovePiece(PieceColor::WHITE, PieceType::KING, Square::C1, Square::E1);
					m_position.MovePiece(PieceColor::WHITE, PieceType::ROOK, Square::D1, Square::A1);
				} else {
					m_position.MovePiece(PieceColor::BLACK, PieceType::KING, Square::C8, Square::E8);
					m_position.MovePiece(PieceColor::BLACK, PieceType::ROOK, Square::D8, Square::A8);
				}
			}

			m_position.m_castlingDone[m_position.m_colorToMove.get_raw_value()] = false;
		} else if (move.flags().isPromotion()) {
			PieceType promotionPiece = m_promotedPieces.pop_back();
			m_position.removePiece(m_position.m_colorToMove, promotionPiece, move.to());
			m_position.addPiece(m_position.m_colorToMove, PieceType::PAWN, move.from());
		}

		m_position.m_irreversibleMovesCount = m_irreversibleMovesCounts.pop_back();
		m_position.m_pawnHash = m_pawnHashes.pop_back();
		m_position.m_hash = m_hashes.pop_back();
		m_position.m_castling = m_castlings.pop_back();
		m_position.m_enPassant = m_enPassants.pop_back();

		if (m_position.m_colorToMove == PieceColor::WHITE) {
			m_position.m_movesCount--;
		}
	}

	static constexpr uint32_t calculateMaterialAtOpening() {
		return common::EvaluationConstants::PIECE_VALUES[common::PieceType::KING.get_raw_value()] +
				common::EvaluationConstants::PIECE_VALUES[common::PieceType::QUEEN.get_raw_value()] +
				common::EvaluationConstants::PIECE_VALUES[common::PieceType::ROOK.get_raw_value()] * 2 +
				common::EvaluationConstants::PIECE_VALUES[common::PieceType::BISHOP.get_raw_value()] * 2 +
				common::EvaluationConstants::PIECE_VALUES[common::PieceType::KNIGHT.get_raw_value()] * 2 +
				common::EvaluationConstants::PIECE_VALUES[common::PieceType::PAWN.get_raw_value()] * 8;
	}

	void clearWalls() {
		m_position.clearWalls();
	}

private:
	Position m_position;
	static const int32_t MATERIAL_AT_OPENING;

	common::FastVector<common::PieceType> m_killedPieces;
	common::FastVector<common::Bitset> m_enPassants;
	common::FastVector<common::Castling> m_castlings;
	common::FastVector<common::PieceType> m_promotedPieces;
	common::FastVector<ZobristHashing> m_hashes;
	common::FastVector<ZobristHashing> m_pawnHashes;
	common::FastVector<uint8_t> m_irreversibleMovesCounts;
	common::FastVector<common::FieldIndex> m_wallSlides;
};

inline constexpr int32_t Session::MATERIAL_AT_OPENING = Session::calculateMaterialAtOpening();

} //namespace phase4::engine::board

#endif
