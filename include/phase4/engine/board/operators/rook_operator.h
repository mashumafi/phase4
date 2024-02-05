#ifndef PHASE4_ENGINE_BOARD_OPERATORS_ROOK_OPERATOR_H
#define PHASE4_ENGINE_BOARD_OPERATORS_ROOK_OPERATOR_H

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>

#include <cstdint>
#include <tuple>

namespace phase4::engine::board::operators {

class RookOperator {
public:
	static void getLoudMoves(Position position, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		const PieceColor color = position.m_colorToMove;
		const PieceColor enemyColor = color.invert();
		Bitset rooks = position.colorPieceMask(color, PieceType::ROOK);

		while (rooks != 0) {
			const Bitset piece = rooks.getLsb(); // TODO: skip lsb
			rooks = rooks.popLsb();

			const Square from(piece.fastBitScan());
			Bitset availableMoves = moves::MovesGenerator::getRookMoves(position.m_occupancySummary, from) & position.occupancy(enemyColor);
			availableMoves &= evasionMask;

			while (availableMoves != 0) {
				const Bitset field = availableMoves.getLsb(); // TODO: skip lsb
				const Square fieldIndex(field.fastBitScan());
				availableMoves = availableMoves.popLsb();

				moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
			}
		}
	}

	static void getQuietMoves(Position position, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		const PieceColor color = position.m_colorToMove;
		Bitset rooks = position.colorPieceMask(color, PieceType::ROOK);

		while (rooks != 0) {
			const Bitset piece = rooks.getLsb(); // TODO: skip lsb
			rooks = rooks.popLsb();

			const Square from(piece.fastBitScan());
			Bitset availableMoves = moves::MovesGenerator::getRookMoves(position.m_occupancySummary, from) & ~position.m_occupancySummary;
			availableMoves &= evasionMask;

			while (availableMoves != 0) {
				const Bitset field = availableMoves.getLsb(); // TODO: skip lsb
				const Square fieldIndex(field.fastBitScan());
				availableMoves = availableMoves.popLsb();

				moves.emplace_back(from, fieldIndex, moves::MoveFlags::QUIET);
			}
		}
	}

	static void getAvailableCaptureMoves(Position position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.m_colorToMove;
		const PieceColor enemyColor = color.invert();
		Bitset rooks = position.colorPieceMask(color, PieceType::ROOK);

		while (rooks != 0) {
			const Bitset piece = rooks.getLsb(); // TODO: skip lsb
			rooks = rooks.popLsb();

			Square from(piece.fastBitScan());
			Bitset availableMoves = moves::MovesGenerator::getRookMoves(position.m_occupancySummary, from) & position.occupancy(enemyColor);

			while (availableMoves != 0) {
				const Bitset field = availableMoves.getLsb(); // TODO: skip lsb
				const Square fieldIndex(field.fastBitScan());
				availableMoves = availableMoves.popLsb();

				moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
			}
		}
	}

	static std::tuple<int32_t, int32_t> getMobility(Position position, common::PieceColor color, common::Bitset &fieldsAttackedByColor) {
		using namespace common;

		int32_t centerMobility = 0;
		int32_t outsideMobility = 0;

		Bitset rooks = position.colorPieceMask(color, PieceType::ROOK);

		while (rooks != 0) {
			const Bitset piece = rooks.getLsb(); // TODO: skip lsb
			rooks = rooks.popLsb();

			const Square from(piece.fastBitScan());
			const Bitset availableMoves = moves::MovesGenerator::getRookMoves(position.m_occupancySummary, from);

			centerMobility += (availableMoves & board::EvaluationConstants::EXTENDED_CENTER).fastCount();
			outsideMobility += (availableMoves & board::EvaluationConstants::OUTSIDE).fastCount();

			fieldsAttackedByColor |= availableMoves;
		}

		return std::make_tuple(centerMobility, outsideMobility);
	}

	static bool isMoveLegal(Position position, moves::Move move) {
		using namespace common;

		const PieceColor enemyColor = position.m_colorToMove.invert();
		const Bitset availableMoves = moves::MovesGenerator::getRookMoves(position.m_occupancySummary, move.from());
		const Bitset toField = move.to().asBitboard();

		if (move.flags().isSinglePush() && (availableMoves & toField) != 0 && (position.m_occupancySummary & toField) == 0) {
			return true;
		}

		if (move.flags().isCapture() && (availableMoves & toField) != 0 && (position.occupancy(enemyColor) & toField) != 0) {
			return true;
		}

		return false;
	}
};

} //namespace phase4::engine::board::operators

#endif
