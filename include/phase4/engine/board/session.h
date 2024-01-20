#ifndef PHASE4_ENGINE_BOARD_SESSION_H
#define PHASE4_ENGINE_BOARD_SESSION_H

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/zobrist_hashing.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/vector.h>

#include <cstdint>

namespace phase4::engine::board {

class Session {
public:
	Session();

	void SetDefaultState() {
		m_position.SetDefaultState();

		recalculateEvaluationDependentValues();

		m_killedPieces.clear();
		m_enPassants.clear();
		m_castlings.clear();
		m_promotedPieces.clear();
		m_hashes.clear();
		m_pawnHashes.clear();
		m_irreversibleMovesCounts.clear();
		m_wallSlides.clear();
	}

	void recalculateEvaluationDependentValues() {
		m_position.recalculateEvaluationDependentValues();
	}

	int32_t calculateMaterial(common::PieceColor color) {
		return m_position.calculateMaterial(color);
	}

	int32_t calculatePosition(common::PieceColor color, common::GamePhase phase) {
		return m_position.calculatePosition(color, phase);
	}

	void makeMove(moves::Move move) {
		m_castlings.push_back(m_position.m_castling);
		m_hashes.push_back(m_position.Hash);
		m_pawnHashes.push_back(m_position.PawnHash);
		m_enPassants.push_back(m_position.EnPassant);
		m_irreversibleMovesCounts.push_back(m_position.IrreversibleMovesCount);

		const Position::MoveDetails &details = m_position.makeMove(move);
		if (details.promotion)
			m_promotedPieces.push_back(*details.promotion);

		if (details.removed)
			m_killedPieces.push_back(details.removed->pieceType);
	}

	static constexpr uint32_t calculateMaterialAtOpening() {
		return ai::score::EvaluationConstants::Pieces[common::PieceType::KING.get_raw_value()] +
				ai::score::EvaluationConstants::Pieces[common::PieceType::QUEEN.get_raw_value()] +
				ai::score::EvaluationConstants::Pieces[common::PieceType::ROOK.get_raw_value()] * 2 +
				ai::score::EvaluationConstants::Pieces[common::PieceType::BISHOP.get_raw_value()] * 2 +
				ai::score::EvaluationConstants::Pieces[common::PieceType::KNIGHT.get_raw_value()] * 2 +
				ai::score::EvaluationConstants::Pieces[common::PieceType::PAWN.get_raw_value()] * 8;
	}

	void clearWalls() {
		m_position.clearWalls();
	}

private:
	Position m_position;
	static const int32_t MATERIAL_AT_OPENING;

	common::Vector<common::PieceType> m_killedPieces;
	common::Vector<common::Bitset> m_enPassants;
	common::Vector<common::Castling> m_castlings;
	common::Vector<common::PieceType> m_promotedPieces;
	common::Vector<ZobristHashing> m_hashes;
	common::Vector<ZobristHashing> m_pawnHashes;
	common::Vector<int> m_irreversibleMovesCounts;
	common::Vector<common::FieldIndex> m_wallSlides;
};

inline constexpr int32_t Session::MATERIAL_AT_OPENING = Session::calculateMaterialAtOpening();

} //namespace phase4::engine::board

#endif
