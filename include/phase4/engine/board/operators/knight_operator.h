#ifndef PHASE4_ENGINE_BOARD_OPERATORS_KNIGHT_OPERATOR_H
#define PHASE4_ENGINE_BOARD_OPERATORS_KNIGHT_OPERATOR_H

#include <phase4/engine/ai/score/evaluation_constants.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>

#include <cstdint>

namespace phase4::engine::board::operators {

class KnightOperator {
public:
	static void GetLoudMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		const PieceColor color = position.ColorToMove;
		const PieceColor enemyColor = color.invert();
		Bitset knights = position.Pieces[color.get_raw_value()][PieceType::KNIGHT.get_raw_value()];

		while (knights != 0) {
			const Bitset piece = knights.getLsb();
			knights = knights.popLsb();

			const Square from(piece.bitScan());
			Bitset availableMoves = moves::MovesGenerator::getKnightMoves(from) & position.Occupancy[enemyColor.get_raw_value()];
			availableMoves &= evasionMask;

			while (availableMoves != 0) {
				const Bitset field = availableMoves.getLsb();
				const Square fieldIndex(field.bitScan());
				availableMoves = availableMoves.popLsb();

				moves.push_back(moves::Move(from, fieldIndex, moves::MoveFlags::CAPTURE));
			}
		}
	}

	static void GetQuietMoves(const Position &position, moves::Moves &moves, common::Bitset evasionMask) {
		using namespace common;

		const PieceColor color = position.ColorToMove;
		Bitset knights = position.Pieces[color.get_raw_value()][PieceType::KNIGHT.get_raw_value()];

		while (knights != 0) {
			const Bitset piece = knights.getLsb();
			knights = knights.popLsb();

			const Square from(piece.bitScan());
			Bitset availableMoves = moves::MovesGenerator::getKnightMoves(from) & ~position.OccupancySummary;
			availableMoves &= evasionMask;

			while (availableMoves != 0) {
				const Bitset field = availableMoves.getLsb();
				const Square fieldIndex(field.bitScan());
				availableMoves = availableMoves.popLsb();

				moves.push_back(moves::Move(from, fieldIndex, moves::MoveFlags::QUIET));
			}
		}
	}

	static void GetAvailableCaptureMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.ColorToMove;
		const PieceColor enemyColor = color.invert();
		Bitset knights = position.Pieces[color.get_raw_value()][PieceType::KNIGHT.get_raw_value()];

		while (knights != 0) {
			const Bitset piece = knights.getLsb();
			knights = knights.popLsb();

			const Square from(piece.bitScan());
			Bitset availableMoves = moves::MovesGenerator::getKnightMoves(from) & position.Occupancy[enemyColor.get_raw_value()];

			while (availableMoves != 0) {
				const Bitset field = availableMoves.getLsb();
				const Square fieldIndex(field.bitScan());
				availableMoves = availableMoves.popLsb();

				moves.push_back(moves::Move(from, fieldIndex, moves::MoveFlags::CAPTURE));
			}
		}
	}

	std::tuple<int32_t, int32_t> GetMobility(const Position &position, common::PieceColor color, common::Bitset &fieldsAttackedByColor) {
		using namespace common;

		int32_t centerMobility = 0;
		int32_t outsideMobility = 0;

		Bitset knights = position.Pieces[color.get_raw_value()][PieceType::KNIGHT.get_raw_value()];

		while (knights != 0) {
			const Bitset piece = knights.getLsb();
			knights = knights.popLsb();

			const Square from(piece.bitScan());
			const Bitset availableMoves = moves::MovesGenerator::getKnightMoves(from);

			centerMobility += (availableMoves & ai::score::EvaluationConstants::ExtendedCenter).count();
			outsideMobility += (availableMoves & ai::score::EvaluationConstants::Outside).count();

			fieldsAttackedByColor |= availableMoves;
		}

		return std::make_tuple(centerMobility, outsideMobility);
	}

	static bool IsMoveLegal(const Position &position, moves::Move move) {
		using namespace common;

		const PieceColor enemyColor = position.ColorToMove.invert();
		const Bitset availableMoves = moves::MovesGenerator::getKnightMoves(move.from());
		const Bitset toField = move.to().asBitboard();

		if (move.flags().isSinglePush() && (availableMoves & toField) != 0 && (position.OccupancySummary & toField) == 0) {
			return true;
		}

		if (move.flags().isCapture() && (availableMoves & toField) != 0 && (position.Occupancy[enemyColor.get_raw_value()] & toField) != 0) {
			return true;
		}

		return false;
	}
};

} //namespace phase4::engine::board::operators
#endif
