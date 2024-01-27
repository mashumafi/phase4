#ifndef PHASE4_ENGINE_BOARD_OPERATORS_QUEEN_OPERATOR_H
#define PHASE4_ENGINE_BOARD_OPERATORS_QUEEN_OPERATOR_H

#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/evaluation_constants.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>

#include <cstdint>
#include <tuple>

namespace phase4::engine::board::operators {

class QueenOperator {
public:
	static void getLoudMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		const PieceColor color = position.m_colorToMove;
		const PieceColor enemyColor = color.invert();
		Bitset queens = position.m_colorPieceMasks[color.get_raw_value()][PieceType::QUEEN.get_raw_value()];

		while (queens != 0) {
			Bitset piece = queens.getLsb();
			queens = queens.popLsb();

			const Square from(piece.fastBitScan());
			Bitset availableMoves = moves::MovesGenerator::getQueenMoves(position.m_occupancySummary, from) & position.m_occupancyByColor[enemyColor.get_raw_value()];
			availableMoves &= evasionMask;

			while (availableMoves != 0) {
				const Bitset field = availableMoves.getLsb();
				const Square fieldIndex(field.fastBitScan());
				availableMoves = availableMoves.popLsb();

				moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
			}
		}
	}

	static void getQuietMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		const PieceColor color = position.m_colorToMove;
		Bitset queens = position.m_colorPieceMasks[color.get_raw_value()][PieceType::QUEEN.get_raw_value()];

		while (queens != 0) {
			const Bitset piece = queens.getLsb();
			queens = queens.popLsb();

			const Square from(piece.fastBitScan());
			Bitset availableMoves = moves::MovesGenerator::getQueenMoves(position.m_occupancySummary, from) & ~position.m_occupancySummary;
			availableMoves &= evasionMask;

			while (availableMoves != 0) {
				const Bitset field = availableMoves.getLsb();
				const Square fieldIndex(field.fastBitScan());
				availableMoves = availableMoves.popLsb();

				moves.emplace_back(from, fieldIndex, moves::MoveFlags::QUIET);
			}
		}
	}

	static void getAvailableCaptureMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.m_colorToMove;
		const PieceColor enemyColor = color.invert();
		Bitset queens = position.m_colorPieceMasks[color.get_raw_value()][PieceType::QUEEN.get_raw_value()];

		while (queens != 0) {
			const Bitset piece = queens.getLsb();
			queens = queens.popLsb();

			const Square from(piece.fastBitScan());
			Bitset availableMoves = moves::MovesGenerator::getQueenMoves(position.m_occupancySummary, from) & position.m_occupancyByColor[enemyColor.get_raw_value()];

			while (availableMoves != 0) {
				const Bitset field = availableMoves.getLsb();
				const Square fieldIndex(field.fastBitScan());
				availableMoves = availableMoves.popLsb();

				moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
			}
		}
	}

	static std::tuple<int32_t, int32_t> getMobility(const Position &position, common::PieceColor color, common::Bitset &fieldsAttackedByColor) {
		using namespace common;

		int32_t centerMobility = 0;
		int32_t outsideMobility = 0;

		Bitset queens = position.m_colorPieceMasks[color.get_raw_value()][PieceType::QUEEN.get_raw_value()];

		while (queens != 0) {
			const Bitset piece = queens.getLsb();
			queens = queens.popLsb();

			const Square from(piece.fastBitScan());
			Bitset availableMoves = moves::MovesGenerator::getQueenMoves(position.m_occupancySummary, from);

			centerMobility += (availableMoves & common::EvaluationConstants::ExtendedCenter).fastCount();
			outsideMobility += (availableMoves & common::EvaluationConstants::Outside).fastCount();

			fieldsAttackedByColor |= availableMoves;
		}

		return std::make_tuple(centerMobility, outsideMobility);
	}

	static bool isMoveLegal(const Position &position, moves::Move move) {
		using namespace common;

		PieceColor enemyColor = position.m_colorToMove.invert();
		Bitset availableMoves = moves::MovesGenerator::getQueenMoves(position.m_occupancySummary, move.from());
		Bitset toField = move.to().asBitboard();

		if (move.flags().isSinglePush() && (availableMoves & toField) != 0 && (position.m_occupancySummary & toField) == 0) {
			return true;
		}

		if (move.flags().isCapture() && (availableMoves & toField) != 0 && (position.m_occupancyByColor[enemyColor.get_raw_value()] & toField) != 0) {
			return true;
		}

		return false;
	}
};

} //namespace phase4::engine::board::operators

#endif
