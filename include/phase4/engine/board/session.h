#ifndef PHASE4_ENGINE_BOARD_SESSION_H
#define PHASE4_ENGINE_BOARD_SESSION_H

#include <phase4/engine/board/ordering/history_heuristic.h>
#include <phase4/engine/board/ordering/killer_heuristic.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>
#include <phase4/engine/board/position_state.h>
#include <phase4/engine/board/transposition/hash_tables.h>
#include <phase4/engine/board/zobrist_hashing.h>

#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/fast_vector.h>
#include <phase4/engine/common/field_index.h>

#include <cmath>
#include <cstdint>

namespace phase4::engine::board {

class Session {
public:
	Session(const Position &position = PositionState::DEFAULT) {
		m_position = position;
	}

	void setPosition(const Position &position) {
		m_position = position;

		m_killedPieces.clear();
		m_enPassants.clear();
		m_castlings.clear();
		m_promotedPieces.clear();
		m_hashes.clear();
		m_pawnHashes.clear();
		m_irreversibleMovesCounts.clear();
		m_wallSlides.clear();

		m_killerHeuristic.clear();
		m_historyHeuristic.clear();

		m_hashTables.m_evaluationHashTable.clear();
		m_hashTables.m_pawnHashTable.clear();
		m_hashTables.m_transpositionTable.clear();
	}

	bool isKingChecked(common::PieceColor color) const {
		return m_position.isKingChecked(color);
	}

	bool isMoveLegal(moves::Move move) const {
		Position positionCopy = Position(m_position);
		PositionMoves::makeMove(positionCopy, move);
		return !positionCopy.isKingChecked(m_position.m_colorToMove);
	}

	PositionMoves::MakeMoveResult makeMove(moves::Move move) {
		m_castlings.push_back(m_position.m_castling);
		m_hashes.push_back(m_position.m_hash);
		m_pawnHashes.push_back(m_position.m_pawnHash);
		m_enPassants.push_back(m_position.m_enPassant);
		m_irreversibleMovesCounts.push_back(m_position.m_irreversibleMovesCount);

		const PositionMoves::MakeMoveResult &details = PositionMoves::makeMove(m_position, move);
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

		const FieldIndex wallMove = m_wallSlides.pop_back();
		if (wallMove != FieldIndex::ZERO) {
			PositionMoves::slideWall(m_position, -wallMove);
		}

		const PieceType pieceType = m_position.m_pieceTable[move.to()];
		m_position.m_colorToMove = m_position.m_colorToMove.invert();

		if (move.flags().isSinglePush() || move.flags().isDoublePush()) {
			m_position.movePiece(m_position.m_colorToMove, pieceType, move.to(), move.from());
		} else if (move.flags().isEnPassant()) {
			const PieceColor enemyColor = m_position.m_colorToMove.invert();
			const Square enemyPieceField(m_position.m_colorToMove == PieceColor::WHITE ? static_cast<uint8_t>(move.to() - 8) : static_cast<uint8_t>(move.to() + 8));
			const PieceType killedPiece = m_killedPieces.pop_back();

			m_position.movePiece(m_position.m_colorToMove, PieceType::PAWN, move.to(), move.from());
			m_position.addPiece(enemyColor, killedPiece, enemyPieceField);
		} else if (move.flags().isCapture()) {
			const PieceColor enemyColor = m_position.m_colorToMove.invert();
			const PieceType killedPiece = m_killedPieces.pop_back();

			// Promotion
			if (move.flags().isPromotion()) {
				const PieceType promotionPiece = m_promotedPieces.pop_back();
				m_position.removePiece(m_position.m_colorToMove, promotionPiece, move.to());
				m_position.addPiece(m_position.m_colorToMove, PieceType::PAWN, move.from());
			} else {
				m_position.movePiece(m_position.m_colorToMove, pieceType, move.to(), move.from());
			}

			m_position.addPiece(enemyColor, killedPiece, move.to());
		} else if (move.flags().isCastling()) {
			if (move.flags().isKingCastling()) { // Short/King castling
				if (m_position.m_colorToMove == PieceColor::WHITE) {
					m_position.movePiece(PieceColor::WHITE, PieceType::KING, Square::G1, Square::E1);
					m_position.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::F1, Square::H1);
				} else {
					m_position.movePiece(PieceColor::BLACK, PieceType::KING, Square::G8, Square::E8);
					m_position.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::F8, Square::H8);
				}
			} else { // Long/Queen castling
				if (m_position.m_colorToMove == PieceColor::WHITE) {
					m_position.movePiece(PieceColor::WHITE, PieceType::KING, Square::C1, Square::E1);
					m_position.movePiece(PieceColor::WHITE, PieceType::ROOK, Square::D1, Square::A1);
				} else {
					m_position.movePiece(PieceColor::BLACK, PieceType::KING, Square::C8, Square::E8);
					m_position.movePiece(PieceColor::BLACK, PieceType::ROOK, Square::D8, Square::A8);
				}
			}

			m_position.m_castlingDone[m_position.m_colorToMove.get_raw_value()] = false;
		} else if (move.flags().isPromotion()) {
			const PieceType promotionPiece = m_promotedPieces.pop_back();
			m_position.removePiece(m_position.m_colorToMove, promotionPiece, move.to());
			m_position.addPiece(m_position.m_colorToMove, PieceType::PAWN, move.from());
		}

		m_position.m_irreversibleMovesCount = m_irreversibleMovesCounts.pop_back();
		m_position.m_pawnHash = m_pawnHashes.pop_back();
		m_position.m_hash = m_hashes.pop_back();
		m_position.m_castling = m_castlings.pop_back();
		m_position.m_enPassant = m_enPassants.pop_back();

		if (m_position.m_colorToMove == PieceColor::BLACK) {
			--m_position.m_movesCount;
		}
	}

	void makeNullMove() {
		++m_position.m_nullMoves;
		if (m_position.m_colorToMove == common::PieceColor::WHITE) {
			++m_position.m_movesCount;
		}

		m_enPassants.push_back(m_position.m_enPassant);
		m_hashes.push_back(m_position.m_hash);

		if (m_position.m_enPassant != 0) {
			const uint8_t enPassantRank = m_position.m_enPassant.bitScan() % 8;
			m_position.m_hash = m_position.m_hash.toggleEnPassant(enPassantRank);
			m_position.m_enPassant = 0;
		}

		m_position.m_colorToMove = m_position.m_colorToMove.invert();
		m_position.m_hash = m_position.m_hash.changeSide();
	}

	void undoNullMove() {
		--m_position.m_nullMoves;
		m_position.m_colorToMove = m_position.m_colorToMove.invert();

		m_position.m_hash = m_hashes.pop_back();
		m_position.m_enPassant = m_enPassants.pop_back();

		if (m_position.m_colorToMove == common::PieceColor::WHITE) {
			--m_position.m_movesCount;
		}
	}

	bool isThreefoldRepetition() const {
		size_t positionsToCheck = std::min(m_hashes.size(), m_position.m_irreversibleMovesCount + 1);
		if (m_position.m_nullMoves == 0 && positionsToCheck >= 8) {
			size_t repetitionsCount = 1;
			for (size_t positionIndex = 1; positionIndex < positionsToCheck; positionIndex += 2) {
				if (m_hashes[positionIndex] == m_position.m_hash) {
					repetitionsCount++;
					if (repetitionsCount >= 3) {
						return true;
					}
				}
			}
		}

		return false;
	}

	void clearWalls() {
		m_position.clearWalls();
	}

private:
	Position m_position;

public:
	inline const Position &position() const {
		return m_position;
	}
	transposition::HashTables<> m_hashTables;
	ordering::HistoryHeuristic m_historyHeuristic;
	ordering::KillerHeuristic m_killerHeuristic;

	const common::FastVector<common::FieldIndex> &wallSlides() const {
		return m_wallSlides;
	}

private:
	common::FastVector<common::PieceType> m_killedPieces;
	common::FastVector<common::Bitboard> m_enPassants;
	common::FastVector<common::Castling> m_castlings;
	common::FastVector<common::PieceType> m_promotedPieces;
	common::FastVector<ZobristHashing> m_hashes;
	common::FastVector<ZobristHashing> m_pawnHashes;
	common::FastVector<size_t> m_irreversibleMovesCounts;
	common::FastVector<common::FieldIndex> m_wallSlides;
};

} //namespace phase4::engine::board

#endif
