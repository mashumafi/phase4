#ifndef PHASE4_ENGINE_BOARD_OPERATORS_BISHOP_OPERATOR_H
#define PHASE4_ENGINE_BOARD_OPERATORS_BISHOP_OPERATOR_H

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/position.h>

#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/moves_generator.h>

#include <phase4/engine/common/bitboard.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>

#include <cstdint>
#include <tuple>

namespace phase4::engine::board::operators {

class BishopOperator {
public:
	static void getLoudMoves(const Position &position, moves::Moves &moves, common::Bitboard evasionMask) {
		using namespace common;

		const PieceColor color = position.colorToMove();
		const PieceColor enemyColor = color.invert();
		Bitboard bishops = position.colorPieceMask(color, PieceType::BISHOP);

		while (bishops != 0) {
			const Bitboard piece = bishops.getLsb();
			bishops = bishops.popLsb();

			const Square from(piece.fastBitScan());
			Bitboard availableMoves = moves::MovesGenerator::getBishopMoves(position.occupancySummary(), from) & position.occupancy(enemyColor);
			availableMoves &= evasionMask;

			while (availableMoves != 0) {
				const Bitboard field = availableMoves.getLsb(); // TODO: skip lsb
				const Square fieldIndex(field.fastBitScan());
				availableMoves = availableMoves.popLsb();

				moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
			}
		}
	}

	static void getQuietMoves(const Position &position, moves::Moves &moves, common::Bitboard evasionMask) {
		using namespace common;

		const PieceColor color = position.colorToMove();
		Bitboard bishops = position.colorPieceMask(color, PieceType::BISHOP);

		while (bishops != 0) {
			const Bitboard piece = bishops.getLsb();
			bishops = bishops.popLsb();

			const Square from(piece.fastBitScan());
			Bitboard availableMoves = moves::MovesGenerator::getBishopMoves(position.occupancySummary(), from) & ~position.occupancySummary();
			availableMoves &= evasionMask;

			while (availableMoves != 0) {
				const Bitboard field = availableMoves.getLsb(); // TODO: skip lsb
				const Square fieldIndex(field.fastBitScan());
				availableMoves = availableMoves.popLsb();

				moves.emplace_back(from, fieldIndex, moves::MoveFlags::QUIET);
			}
		}
	}

	static void getAvailableCaptureMoves(const Position &position, moves::Moves &moves) {
		using namespace common;

		const PieceColor color = position.colorToMove();
		const PieceColor enemyColor = color.invert();
		Bitboard bishops = position.colorPieceMask(color, PieceType::BISHOP);

		while (bishops != 0) {
			const Bitboard piece = bishops.getLsb(); // TODO: skip lsb
			bishops = bishops.popLsb();

			const Square from(piece.fastBitScan());
			Bitboard availableMoves = moves::MovesGenerator::getBishopMoves(position.occupancySummary(), from) & position.occupancy(enemyColor);

			while (availableMoves != 0) {
				const Bitboard field = availableMoves.getLsb(); // TODO: skip lsb
				Square fieldIndex(field.fastBitScan());
				availableMoves = availableMoves.popLsb();

				moves.emplace_back(from, fieldIndex, moves::MoveFlags::CAPTURE);
			}
		}
	}

	static std::tuple<int32_t, int32_t> getMobility(const Position &position, common::PieceColor color, common::Bitboard &fieldsAttackedByColor) {
		using namespace common;

		int32_t centerMobility = 0;
		int32_t outsideMobility = 0;

		Bitboard bishops = position.colorPieceMask(color, PieceType::BISHOP);

		while (bishops != 0) {
			const Bitboard piece = bishops.getLsb(); // TODO: skip lsb
			bishops = bishops.popLsb();

			const Square from(piece.fastBitScan());
			const Bitboard availableMoves = moves::MovesGenerator::getBishopMoves(position.occupancySummary(), from);

			centerMobility += (availableMoves & board::EvaluationConstants::EXTENDED_CENTER).fastCount();
			outsideMobility += (availableMoves & board::EvaluationConstants::OUTSIDE).fastCount();

			fieldsAttackedByColor |= availableMoves;
		}

		return std::make_tuple(centerMobility, outsideMobility);
	}

	static bool isMoveLegal(const Position &position, moves::Move move) {
		using namespace common;

		const PieceColor enemyColor = position.colorToMove().invert();
		const Bitboard availableMoves = moves::MovesGenerator::getBishopMoves(position.occupancySummary(), move.from());
		const Bitboard toField = move.to().asBitboard();

		if (move.flags().isSinglePush() && (availableMoves & toField) != 0 && (position.occupancySummary() & toField) == 0) {
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
