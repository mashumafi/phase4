#ifndef PHASE4_ENGINE_BOARD_ORDERING_STATIC_EXCHANGE_EVALUATION_H
#define PHASE4_ENGINE_BOARD_ORDERING_STATIC_EXCHANGE_EVALUATION_H

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/ordering/see_piece.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/safe_vector.h>
#include <phase4/engine/common/square.h>
#include <phase4/engine/common/util.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board::ordering {

class StaticExchangeEvaluation {
public:
	using AttackersDefendersArray = std::array<std::array<int16_t, 256>, 256>;
	using Array = std::array<AttackersDefendersArray, 6>;

	static inline int16_t evaluate(common::PieceType attackingPiece, common::PieceType capturedPiece, uint8_t attackers, uint8_t defenders) {
		return board::EvaluationConstants::pieceValue(capturedPiece) + TABLE[attackingPiece.get_raw_value()][attackers][defenders];
	}

	static constexpr common::PieceType getLeastValuablePiece(common::Bitset data) {
		const common::Bitset leastValuableDefenderField = data.getLsb(); // TODO: skip lsb
		const uint8_t leastValuableDefenderPiece = leastValuableDefenderField.bitScan();

		return getPieceBySeeIndex(leastValuableDefenderPiece);
	}

private:
	static constexpr common::PieceType getPieceBySeeIndex(int16_t index) {
		switch (index) {
			case SeePiece::PAWN.get_raw_value():
				return common::PieceType::PAWN;
			case SeePiece::KNIGHT1.get_raw_value():
			case SeePiece::KNIGHT2.get_raw_value():
				return common::PieceType::KNIGHT;
			case SeePiece::BISHOP.get_raw_value():
				return common::PieceType::BISHOP;
			case SeePiece::ROOK1.get_raw_value():
			case SeePiece::ROOK2.get_raw_value():
				return common::PieceType::ROOK;
			case SeePiece::QUEEN.get_raw_value():
				return common::PieceType::QUEEN;
			case SeePiece::KING.get_raw_value():
				return common::PieceType::KING;
		}

		return common::PieceType::INVALID;
	}

	static constexpr SeePiece getSeeIndexByPiece(common::PieceType piece) {
		switch (piece.get_raw_value()) {
			case common::PieceType::PAWN.get_raw_value():
				return SeePiece::PAWN;
			case common::PieceType::KNIGHT.get_raw_value():
				return SeePiece::KNIGHT1;
			case common::PieceType::BISHOP.get_raw_value():
				return SeePiece::BISHOP;
			case common::PieceType::ROOK.get_raw_value():
				return SeePiece::ROOK1;
			case common::PieceType::QUEEN.get_raw_value():
				return SeePiece::QUEEN;
			case common::PieceType::KING.get_raw_value():
				return SeePiece::KING;
		}

		return SeePiece::INVALID;
	}

	static constexpr int16_t computeResult(common::PieceType attackingPiece, common::Bitset attackers, common::Bitset defenders) {
		common::SafeVector<int16_t, 7> gainList = {};

		common::PieceType currentPieceOnField = attackingPiece;
		int16_t result = 0;

		gainList.push_back(result);

		if (defenders != 0) {
			common::PieceType leastValuableDefenderPiece = getLeastValuablePiece(defenders);
			defenders = defenders.popLsb();

			result -= EvaluationConstants::pieceValue(currentPieceOnField);
			currentPieceOnField = leastValuableDefenderPiece;

			gainList.push_back(result);

			while (attackers != 0) {
				const common::PieceType leastValuableAttackerPiece = getLeastValuablePiece(attackers);
				attackers = attackers.popLsb();

				result += EvaluationConstants::pieceValue(currentPieceOnField);
				currentPieceOnField = leastValuableAttackerPiece;

				gainList.push_back(result);

				if (gainList.peek() > gainList.peek(2)) {
					return gainList.peek(2);
				}

				if (defenders == 0) {
					return result;
				}

				leastValuableDefenderPiece = getLeastValuablePiece(defenders);
				defenders = defenders.popLsb();

				result -= EvaluationConstants::pieceValue(currentPieceOnField);
				currentPieceOnField = leastValuableDefenderPiece;

				gainList.push_back(result);

				if (gainList.peek() < gainList.peek(2)) {
					return gainList.peek(2);
				}
			}
		}

		return result;
	}

	static constexpr AttackersDefendersArray populate(common::PieceType attackingPiece) {
		AttackersDefendersArray table = {};

		const common::Bitset attackingPieceSeeIndex(getSeeIndexByPiece(attackingPiece).asBitboard());
		for (uint64_t attackerIndex = 0; attackerIndex < 256; ++attackerIndex) {
			const common::Bitset attackers = common::Bitset(attackerIndex) & ~attackingPieceSeeIndex;
			for (uint64_t defenderIndex = 0; defenderIndex < 256; ++defenderIndex) {
				table[attackerIndex][defenderIndex] = computeResult(attackingPiece, attackers, defenderIndex);
			}
		}

		return table;
	}

	static const Array TABLE;
};

} //namespace phase4::engine::board::ordering

#ifndef FAST_BUILD
#include "static_exchange_evaluation.cpp"
#endif

#endif
