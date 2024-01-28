#ifndef PHASE4_ENGINE_BOARD_ORDERING_STATIC_EXCHANGE_EVALUATION_H
#define PHASE4_ENGINE_BOARD_ORDERING_STATIC_EXCHANGE_EVALUATION_H

#include <phase4/engine/board/evaluation_constants.h>
#include <phase4/engine/board/ordering/see_piece.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/safe_vector.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board::ordering {

class StaticExchangeEvaluation {
public:
	using AttackersDefendersArray = std::array<std::array<int16_t, 256>, 256>;
	using Array = std::array<AttackersDefendersArray, 6>;

	static constexpr int16_t evaluate(int attackingPiece, int capturedPiece, int attacker, int defender);

private:
	static constexpr common::PieceType getPieceBySeeIndex(int16_t index) {
		switch (index) {
			case SeePiece::PAWN:
				return common::PieceType::PAWN;
			case SeePiece::KNIGHT1:
			case SeePiece::KNIGHT2:
				return common::PieceType::KNIGHT;
			case SeePiece::BISHOP:
				return common::PieceType::BISHOP;
			case SeePiece::ROOK1:
			case SeePiece::ROOK2:
				return common::PieceType::ROOK;
			case SeePiece::QUEEN:
				return common::PieceType::QUEEN;
			case SeePiece::KING:
				return common::PieceType::KING;
		}

		return common::PieceType::INVALID;
	}

	static constexpr int16_t getSeeIndexByPiece(common::PieceType piece) {
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

		return -1;
	}

	static constexpr common::PieceType getLeastValuablePiece(common::Bitset data) {
		const common::Bitset leastValuableDefenderField = data.getLsb();
		const uint8_t leastValuableDefenderPiece = leastValuableDefenderField.bitScan();

		return getPieceBySeeIndex(leastValuableDefenderPiece);
	}

	static constexpr int16_t computeResult(common::PieceType attackingPiece, uint64_t attackerIndex, uint64_t defenderIndex) {
		common::SafeVector<int16_t, 128> gainList = {};

		const common::Square attackingPieceSeeIndex(getSeeIndexByPiece(attackingPiece));
		common::Bitset attackers = common::Bitset(attackerIndex) & ~attackingPieceSeeIndex.asBitboard();
		common::Bitset defenders(defenderIndex);

		common::PieceType currentPieceOnField = attackingPiece;
		int16_t result = 0;

		gainList.push_back(result);

		if (defenders != 0) {
			common::PieceType leastValuableDefenderPiece = getLeastValuablePiece(defenders);
			defenders = defenders.popLsb();

			result -= EvaluationConstants::PIECE_VALUES[currentPieceOnField.get_raw_value()];
			currentPieceOnField = leastValuableDefenderPiece;

			gainList.push_back(result);

			while (attackers != 0) {
				const common::PieceType leastValuableAttackerPiece = getLeastValuablePiece(attackers);
				attackers = attackers.popLsb();

				result += EvaluationConstants::PIECE_VALUES[currentPieceOnField.get_raw_value()];
				currentPieceOnField = leastValuableAttackerPiece;

				gainList.push_back(result);

				if (gainList.at(-1) > gainList.at(-3)) {
					result = gainList.at(-3);
					break;
				}

				if (defenders != 0) {
					leastValuableDefenderPiece = getLeastValuablePiece(defenders);
					defenders = defenders.popLsb();

					result -= EvaluationConstants::PIECE_VALUES[currentPieceOnField.get_raw_value()];
					currentPieceOnField = leastValuableDefenderPiece;

					gainList.push_back(result);

					if (gainList.at(-1) < gainList.at(-3)) {
						result = gainList.at(-3);
						break;
					}
				} else {
					break;
				}
			}
		}

		return result;
	}

	static constexpr AttackersDefendersArray populate(common::PieceType attackingPiece) {
		AttackersDefendersArray table = {};

		for (uint64_t attackerIndex = 0; attackerIndex < 256; ++attackerIndex) {
			for (uint64_t defenderIndex = 0; defenderIndex < 256; ++defenderIndex) {
				table[attackerIndex][defenderIndex] = computeResult(attackingPiece, attackerIndex, defenderIndex);
			}
		}

		return table;
	}

	static const Array TABLE;
};

inline constexpr StaticExchangeEvaluation::Array StaticExchangeEvaluation::TABLE = {
	populate(common::PieceType::PAWN),
	populate(common::PieceType::KNIGHT),
	populate(common::PieceType::BISHOP),
	populate(common::PieceType::ROOK),
	populate(common::PieceType::QUEEN),
	populate(common::PieceType::KING),
};

constexpr int16_t StaticExchangeEvaluation::evaluate(int attackingPiece, int capturedPiece, int attacker, int defender) {
	return static_cast<short>(board::EvaluationConstants::PIECE_VALUES[capturedPiece] + TABLE[attackingPiece][attacker][defender]);
}

} //namespace phase4::engine::board::ordering

#endif
