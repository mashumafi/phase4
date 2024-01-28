#ifndef PHASE4_ENGINE_BOARD_ORDERING_STATIC_EXCHANGE_EVALUATION_H
#define PHASE4_ENGINE_BOARD_ORDERING_STATIC_EXCHANGE_EVALUATION_H

#include <phase4/engine/board/ordering/see_piece.h>
#include <phase4/engine/board/evaluation_constants.h>

#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/bitset.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board::ordering {

class StaticExchangeEvaluation {
public:
	using Array = std::array<std::array<std::array<int16_t, 256>, 256>, 6>;

	static constexpr int16_t evaluate(int attackingPiece, int capturedPiece, int attacker, int defender);

private:
	static constexpr void populateTable() {
		/*var gainList = new List<int>();
        for (common::PieceType attackingPiece = common::PieceType::PAWN; attackingPiece != common::PieceType::INVALID; ++attackingPiece) {
			for (ulong attackerIndex = 0; attackerIndex < 256; attackerIndex++) {
				for (ulong defenderIndex = 0; defenderIndex < 256; defenderIndex++) {
					var attackingPieceSeeIndex = getSeeIndexByPiece(attackingPiece);
					var attackers = attackerIndex & ~(1ul << attackingPieceSeeIndex);
					var defenders = defenderIndex;

					var currentPieceOnField = attackingPiece;
					var result = 0;

					gainList.Add(result);

					if (defenders != 0) {
						var leastValuableDefenderPiece = GetLeastValuablePiece(defenders);
						defenders = BitOperations.PopLsb(defenders);

						result -= EvaluationConstants.Pieces[currentPieceOnField];
						currentPieceOnField = leastValuableDefenderPiece;

						gainList.Add(result);

						while (attackers != 0) {
							var leastValuableAttackerPiece = GetLeastValuablePiece(attackers);
							attackers = BitOperations.PopLsb(attackers);

							result += EvaluationConstants.Pieces[currentPieceOnField];
							currentPieceOnField = leastValuableAttackerPiece;

							gainList.Add(result);

							if (gainList[^1] > gainList[^3]) {
								result = gainList[^3];
								break;
							}

							if (defenders != 0) {
								leastValuableDefenderPiece = GetLeastValuablePiece(defenders);
								defenders = BitOperations.PopLsb(defenders);

								result -= EvaluationConstants.Pieces[currentPieceOnField];
								currentPieceOnField = leastValuableDefenderPiece;

								gainList.Add(result);

								if (gainList[^1] < gainList[^3]) {
									result = gainList[^3];
									break;
								}
							} else {
								break;
							}
						}
					}

					_table[attackingPiece][attackerIndex][defenderIndex] = (short)result;
					gainList.Clear();
				}
			}
		}*/
	}

	static constexpr int16_t getPieceBySeeIndex(int16_t index) {
		switch (index) {
			case SeePiece::PAWN:
				return common::PieceType::PAWN.get_raw_value();
			case SeePiece::KNIGHT1:
			case SeePiece::KNIGHT2:
				return common::PieceType::KNIGHT.get_raw_value();
			case SeePiece::BISHOP:
				return common::PieceType::BISHOP.get_raw_value();
			case SeePiece::ROOK1:
			case SeePiece::ROOK2:
				return common::PieceType::ROOK.get_raw_value();
			case SeePiece::QUEEN:
				return common::PieceType::QUEEN.get_raw_value();
			case SeePiece::KING:
				return common::PieceType::KING.get_raw_value();
		}

		return -1;
	}

	static constexpr int16_t getSeeIndexByPiece(int16_t piece) {
		switch (piece) {
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

	static constexpr int16_t getLeastValuablePiece(common::Bitset data) {
		const common::Bitset leastValuableDefenderField = data.getLsb();
		const uint8_t leastValuableDefenderPiece = leastValuableDefenderField.bitScan();

		return getPieceBySeeIndex(leastValuableDefenderPiece);
	}

    static constexpr Array populate() {
        Array array = {};
        return array;
    }

	static const Array TABLE;
};

inline constexpr StaticExchangeEvaluation::Array StaticExchangeEvaluation::TABLE = populate();

constexpr int16_t StaticExchangeEvaluation::evaluate(int attackingPiece, int capturedPiece, int attacker, int defender) {
	return static_cast<short>(board::EvaluationConstants::PIECE_VALUES[capturedPiece] + TABLE[attackingPiece][attacker][defender]);
}

} //namespace phase4::engine::board::ordering

#endif
