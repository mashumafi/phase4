#ifndef PHASE4_ENGINE_BOARD_OPERATORS_KNIGHT_OPERATOR_H
#define PHASE4_ENGINE_BOARD_OPERATORS_KNIGHT_OPERATOR_H

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

	/*static int GetQuietMoves(const Position& position, moves::Moves& moves, common::Bitset evasionMask) {
		using namespace common;

		const PieceColor color = position.ColorToMove;
		Bitset knights = position.Pieces[color][Piece.Knight];

		while (knights != 0) {
			var piece = BitOperations.GetLsb(knights);
			knights = BitOperations.PopLsb(knights);

			var from = BitOperations.BitScan(piece);
			var availableMoves = KnightMovesGenerator.GetMoves(from) & ~position.OccupancySummary;
			availableMoves &= evasionMask;

			while (availableMoves != 0) {
				var field = BitOperations.GetLsb(availableMoves);
				var fieldIndex = BitOperations.BitScan(field);
				availableMoves = BitOperations.PopLsb(availableMoves);

				moves[offset++] = new Move(from, fieldIndex, MoveFlags.Quiet);
			}
		}

		return offset;
	}

	static int GetAvailableCaptureMoves(const Position& position, moves::Moves& moves) {
		using namespace common;

		var color = boardState.ColorToMove;
		var enemyColor = ColorOperations.Invert(color);
		var knights = boardState.Pieces[color][Piece.Knight];

		while (knights != 0) {
			var piece = BitOperations.GetLsb(knights);
			knights = BitOperations.PopLsb(knights);

			var from = BitOperations.BitScan(piece);
			var availableMoves = KnightMovesGenerator.GetMoves(from) & boardState.Occupancy[enemyColor];

			while (availableMoves != 0) {
				var field = BitOperations.GetLsb(availableMoves);
				var fieldIndex = BitOperations.BitScan(field);
				availableMoves = BitOperations.PopLsb(availableMoves);

				moves[offset++] = new Move(from, fieldIndex, MoveFlags.Capture);
			}
		}

		return offset;
	}

	static(int, int) GetMobility(const Position& position, common::Color color, common::Bitset& fieldsAttackedByColor) {
		using namespace common;

		var centerMobility = 0;
		var outsideMobility = 0;

		var knights = boardState.Pieces[color][Piece.Knight];

		while (knights != 0) {
			var piece = BitOperations.GetLsb(knights);
			knights = BitOperations.PopLsb(knights);

			var from = BitOperations.BitScan(piece);
			var availableMoves = KnightMovesGenerator.GetMoves(from);

			centerMobility += (int)BitOperations.Count(availableMoves & EvaluationConstants.ExtendedCenter);
			outsideMobility += (int)BitOperations.Count(availableMoves & EvaluationConstants.Outside);

			fieldsAttackedByColor |= availableMoves;
		}

		return (centerMobility, outsideMobility);
	}

	static bool IsMoveLegal(const Position& position, moves::Move move) {
		using namespace common;

		var enemyColor = ColorOperations.Invert(boardState.ColorToMove);
		var availableMoves = KnightMovesGenerator.GetMoves(move.From);
		var toField = 1ul << move.To;

		if (move.IsSinglePush() && (availableMoves & toField) != 0 && (boardState.OccupancySummary & toField) == 0) {
			return true;
		}

		if (move.IsCapture() && (availableMoves & toField) != 0 && (boardState.Occupancy[enemyColor] & toField) != 0) {
			return true;
		}

		return false;
	}*/
};

} //namespace phase4::engine::board::operators
#endif
