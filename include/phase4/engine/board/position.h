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

private:
	std::array<std::array<common::Bitset, 6>, 2> m_colorPieceMasks;

public:
	inline constexpr common::Bitset &colorPieceMask(common::PieceColor color, common::PieceType piece) {
		return m_colorPieceMasks[color.get_raw_value()][piece.get_raw_value()];
	}
	inline constexpr common::Bitset colorPieceMask(common::PieceColor color, common::PieceType piece) const {
		return m_colorPieceMasks[color.get_raw_value()][piece.get_raw_value()];
	}

private:
	std::array<common::Bitset, 2> m_occupancyByColor = {};

public:
	inline constexpr common::Bitset &occupancy(common::PieceColor color) {
		return m_occupancyByColor[color.get_raw_value()];
	}
	inline constexpr common::Bitset occupancy(common::PieceColor color) const {
		return m_occupancyByColor[color.get_raw_value()];
	}

	common::Bitset m_occupancySummary;
	common::Bitset m_enPassant;
	common::Castling m_castling = common::Castling::NONE;
	common::PieceColor m_colorToMove = common::PieceColor::WHITE;
	uint16_t m_movesCount = 1;
	size_t m_irreversibleMovesCount = 0;
	uint16_t m_nullMoves = 0;

	std::array<bool, 2> m_castlingDone = {};

private:
	std::array<int32_t, 2> m_material = {};

public:
	inline constexpr int32_t &material(common::PieceColor color) {
		return m_material[color.get_raw_value()];
	}
	inline constexpr int32_t material(common::PieceColor color) const {
		return m_material[color.get_raw_value()];
	}
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

		assert((colorPieceMask(color, piece) & from.asBitboard()) != 0);
		assert((colorPieceMask(color, piece) & to.asBitboard()) == 0);
		assert((m_colorPieceMasks[color.invert().get_raw_value()][piece.get_raw_value()] & to.asBitboard()) == 0);

		const common::Bitset move = from.asBitboard() | to.asBitboard();

		colorPieceMask(color, piece) ^= move;
		occupancy(color) ^= move;
		m_occupancySummary ^= move;

		m_positionEval[color.get_raw_value()][GamePhase::OPENING] -= PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][from];
		m_positionEval[color.get_raw_value()][GamePhase::OPENING] += PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][to];

		m_positionEval[color.get_raw_value()][GamePhase::ENDING] -= PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][from];
		m_positionEval[color.get_raw_value()][GamePhase::ENDING] += PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][to];

		m_pieceTable[from] = PieceType::INVALID;
		m_pieceTable[to] = piece;
	}

	inline void addPiece(common::PieceColor color, common::PieceType piece, common::Square fieldIndex) {
		using namespace phase4::engine::common;

		assert((colorPieceMask(color, piece) & fieldIndex.asBitboard()) == 0);

		const common::Bitset field = fieldIndex.asBitboard();

		colorPieceMask(color, piece) ^= field;
		occupancy(color) ^= field;
		m_occupancySummary ^= field;

		material(color) += board::EvaluationConstants::PIECE_VALUES[piece.get_raw_value()];

		m_positionEval[color.get_raw_value()][GamePhase::OPENING] += piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][fieldIndex];
		m_positionEval[color.get_raw_value()][GamePhase::ENDING] += piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][fieldIndex];

		m_pieceTable[fieldIndex] = piece;
	}

	inline void removePiece(common::PieceColor color, common::PieceType piece, common::Square fieldIndex) {
		using namespace phase4::engine::common;

		assert((colorPieceMask(color, piece) & fieldIndex.asBitboard()) != 0);

		const common::Bitset field = fieldIndex.asBitboard();

		colorPieceMask(color, piece) ^= field;
		occupancy(color) ^= field;
		m_occupancySummary ^= field;

		material(color) -= EvaluationConstants::PIECE_VALUES[piece.get_raw_value()];

		m_positionEval[color.get_raw_value()][GamePhase::OPENING] -= piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::OPENING][fieldIndex];
		m_positionEval[color.get_raw_value()][GamePhase::ENDING] -= piece_square_tables::PieceSquareTablesData::VALUES[piece.get_raw_value()][color.get_raw_value()][GamePhase::ENDING][fieldIndex];

		m_pieceTable[fieldIndex] = PieceType::INVALID;
	}

	bool isKingChecked(common::PieceColor color) const {
		const common::Bitset king = colorPieceMask(color, common::PieceType::KING);
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
		if ((colorPieceMask(PieceColor::WHITE, pieceType) & fieldBB) > 0) {
			return std::make_tuple(PieceColor::WHITE, pieceType);
		}

		if ((colorPieceMask(PieceColor::BLACK, pieceType) & fieldBB) > 0) {
			return std::make_tuple(PieceColor::BLACK, pieceType);
		}

		return {};
	}

	bool isFieldAttacked(common::PieceColor color, common::Square fieldIndex) const {
		using namespace common;

		const PieceColor enemyColor = color.invert();

		const Bitset fileRankAttacks = moves::MovesGenerator::getRookMoves(m_occupancySummary, fieldIndex) & occupancy(enemyColor);
		const Bitset attackingRooks = fileRankAttacks & (colorPieceMask(enemyColor, PieceType::ROOK) | colorPieceMask(enemyColor, PieceType::QUEEN));
		if (attackingRooks != 0) {
			return true;
		}

		const Bitset diagonalAttacks = moves::MovesGenerator::getBishopMoves(m_occupancySummary, fieldIndex) & occupancy(enemyColor);
		const Bitset attackingBishops = diagonalAttacks & (colorPieceMask(enemyColor, PieceType::BISHOP) | colorPieceMask(enemyColor, PieceType::QUEEN));
		if (attackingBishops != 0) {
			return true;
		}

		const Bitset jumpAttacks = moves::MovesGenerator::getKnightMoves(fieldIndex);
		const Bitset attackingKnights = jumpAttacks & colorPieceMask(enemyColor, PieceType::KNIGHT);
		if (attackingKnights != 0) {
			return true;
		}

		const Bitset boxAttacks = moves::MovesGenerator::getKingMoves(fieldIndex);
		const Bitset attackingKings = boxAttacks & colorPieceMask(enemyColor, PieceType::KING);
		if (attackingKings != 0) {
			return true;
		}

		const Bitset field = fieldIndex.asBitboard();
		const Bitset potentialPawns = boxAttacks & colorPieceMask(enemyColor, PieceType::PAWN);
		const Bitset attackingPawns = (color.get_raw_value() == PieceColor::WHITE.get_raw_value()) ? field & ((potentialPawns >> 7) | (potentialPawns >> 9)) : field & ((potentialPawns << 7) | (potentialPawns << 9));

		return (attackingPawns != 0);
	}

	constexpr int32_t getPhaseRatio() const {
		using namespace common;

		const int32_t materialOfWeakerSide = Math::min_int32(material(PieceColor::WHITE), material(PieceColor::BLACK));

		constexpr int32_t openingDelta = board::EvaluationConstants::MATERIAL_AT_OPENING - board::EvaluationConstants::OPENING_ENDGAME_EDGE;
		const int32_t boardDelta = materialOfWeakerSide - board::EvaluationConstants::OPENING_ENDGAME_EDGE;

		return Math::max_int32(boardDelta, 0) * PositionConstants::PHASE_RESOLUTION / openingDelta;
	}

	int32_t getGamePhase() const {
		using namespace common;

		const int32_t materialOfWeakerSide = Math::min_int32(material(PieceColor::WHITE), material(PieceColor::BLACK));
		return materialOfWeakerSide > EvaluationConstants::OPENING_ENDGAME_EDGE ? GamePhase::OPENING : GamePhase::ENDING;
	}

	bool isFieldPassing(common::PieceColor color, common::Square field) const {
		const common::PieceColor enemyColor = color.invert();
		const common::Bitset passingArea = moves::patterns::PassingPatternGenerator::getPattern(color, field);

		return (passingArea & colorPieceMask(enemyColor, common::PieceType::PAWN)) == 0;
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
		if (material(PieceColor::WHITE) < drawEdge && material(PieceColor::BLACK) < drawEdge) {
			const Bitset whiteKnightOrBishopPresent = (colorPieceMask(PieceColor::WHITE, PieceType::KNIGHT) | colorPieceMask(PieceColor::WHITE, PieceType::BISHOP)) != 0;
			const Bitset blackKnightOrBishopPresent = (colorPieceMask(PieceColor::BLACK, PieceType::KNIGHT) | colorPieceMask(PieceColor::BLACK, PieceType::BISHOP)) != 0;

			if (whiteKnightOrBishopPresent != 0 && blackKnightOrBishopPresent != 0) {
				return false;
			}

			if (colorPieceMask(PieceColor::WHITE, PieceType::PAWN) == 0 && colorPieceMask(PieceColor::BLACK, PieceType::PAWN) == 0) {
				return true;
			}
		}

		return false;
	}
};

} //namespace phase4::engine::board

#endif
