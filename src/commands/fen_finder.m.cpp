#include <phase4/engine/fen/fen_to_position.h>
#include <phase4/engine/fen/lichess_csv_parser.h>
#include <phase4/engine/fen/position_to_fen.h>

#include <phase4/engine/ai/score/evaluation.h>
#include <phase4/engine/ai/score/evaluators/bishop_evaluator.h>
#include <phase4/engine/ai/score/evaluators/king_safety_evaluator.h>
#include <phase4/engine/ai/score/evaluators/material_evaluator.h>
#include <phase4/engine/ai/score/evaluators/mobility_evaluator.h>
#include <phase4/engine/ai/score/evaluators/pawn_structure_evaluator.h>
#include <phase4/engine/ai/score/evaluators/position_evaluator.h>
#include <phase4/engine/ai/score/evaluators/rook_evaluator.h>

#include <phase4/engine/board/ordering/see_piece.h>
#include <phase4/engine/board/ordering/static_exchange_evaluation.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/position_constants.h>
#include <phase4/engine/common/square.h>

int main(int argc, const char **args) {
	using namespace phase4::engine;
	using namespace phase4::engine::moves::magic;
	using namespace std::literals;

	MagicBitboards::initWithInternalKeys();

	bool bishopSearch = false; // -b
	bool kingSafetySearch = false; // -k
	bool materialSearch = false; // -m
	bool mobilitySearch = false; // -j
	bool pawnStructureSearch = false; // -s
	bool positionSearch = false; // -p
	bool rookSearch = false; // -r
	bool seePieceSearch = false; // -sp
	bool seeSearch = false; // -see
	int32_t phase = common::PositionConstants::PHASE_RESOLUTION;

	for (int i = 1; i < argc; ++i) {
		if (args[i] == "-b"sv) {
			bishopSearch = true;
		}
		if (args[i] == "-k"sv) {
			kingSafetySearch = true;
		}
		if (args[i] == "-m"sv) {
			materialSearch = true;
		}
		if (args[i] == "-j"sv) {
			mobilitySearch = true;
		}
		if (args[i] == "-s"sv) {
			pawnStructureSearch = true;
		}
		if (args[i] == "-p"sv) {
			positionSearch = true;
		}
		if (args[i] == "-r"sv) {
			rookSearch = true;
		}
		if (args[i] == "-sp"sv) {
			seePieceSearch = true;
		}
		if (args[i] == "-see"sv) {
			seeSearch = true;
		}
	}

	if (bishopSearch) {
	}
	if (kingSafetySearch) {
		std::cout << "Search will include KingSafetyEvaluator" << std::endl;
	}
	if (materialSearch) {
	}
	if (mobilitySearch) {
		std::cout << "Search will include MobilityEvaluator" << std::endl;
	}
	if (pawnStructureSearch) {
		std::cout << "Search will include PawnStructureEvaluator" << std::endl;
	}
	if (positionSearch) {
		std::cout << "Search will include PositionEvaluator" << std::endl;
	}
	if (rookSearch) {
		std::cout << "Search will include RookEvaluator" << std::endl;
	}
	if (seePieceSearch) {
		std::cout << "Search will include SeePiece" << std::endl;
	}
	if (seeSearch) {
		std::cout << "Search will include StaticExchangeEvaluation" << std::endl;
	}

	fen::LichessCsvParser csvReader("/workspaces/phase4/puzzles/lichess_db_puzzle.csv");

	while (auto puzzle = csvReader.nextPuzzle()) {
		if (!puzzle) {
			return 1;
		}

		std::optional<board::Position> position = fen::FenToPosition::parse(puzzle->fen);
		if (!position) {
			return 1;
		}

		auto blunderMove = board::PositionMoves::findRealMove(*position, puzzle->blunder);
		if (!blunderMove) {
			return 1;
		}
		board::Position blunderPosition = *position;
		board::PositionMoves::makeMove(blunderPosition, *blunderMove);
		const std::string blunderFen = fen::PositionToFen::encode(blunderPosition);

		common::Bitset fieldsAttackedByWhite;
		common::Bitset fieldsAttackedByBlack;
		if (mobilitySearch) {
			const int32_t score = ai::score::evaluators::MobilityEvaluator::evaluate(*position, phase, common::PositionConstants::PHASE_RESOLUTION - phase, fieldsAttackedByWhite, fieldsAttackedByBlack);
			if (score != 0) {
				std::cout << "Mobility (" << score << "): " << puzzle->fen << std::endl;
			}
		}
		if (bishopSearch) {
		}
		if (kingSafetySearch) {
			const int32_t score = ai::score::evaluators::KingSafetyEvaluator::evaluate(*position, phase, common::PositionConstants::PHASE_RESOLUTION - phase, fieldsAttackedByWhite, fieldsAttackedByBlack);
			if (score != 0) {
				std::cout << "KingSafety (" << score << "): " << puzzle->fen << std::endl;
			}
		}
		if (materialSearch) {
			const int32_t score = ai::score::evaluators::MaterialEvaluator::evaluate(*position);
			if (score != 0 && puzzle->expectedMoves.size() > 10) {
				std::cout << "Material (" << score << "): " << puzzle->fen << " " << puzzle->blunder << " " << puzzle->expectedMoves << std::endl;
			}
		}
		if (pawnStructureSearch) {
			const int32_t score = ai::score::evaluators::PawnStructureEvaluator::evaluateWithoutCache(*position, phase, common::PositionConstants::PHASE_RESOLUTION - phase);
			if (score != 0) {
				std::cout << "PawnStructure (" << score << "): " << puzzle->fen << std::endl;
			}
		}
		if (positionSearch) {
			const int32_t openingScore = ai::score::evaluators::PositionEvaluator::evaluate(*position, phase, 1024 - phase);
			const int32_t endingScore = ai::score::evaluators::PositionEvaluator::evaluate(*position, 1024 - phase, phase);
			if (openingScore != 0 && endingScore != 0 && puzzle->expectedMoves.size() > 10) {
				std::cout << "Position (" << openingScore << "," << endingScore << "): " << puzzle->fen << " " << puzzle->blunder << " " << puzzle->expectedMoves << std::endl;
			}
		}
		if (rookSearch) {
			const int32_t score = ai::score::evaluators::RookEvaluator::evaluate(*position, phase, 1024 - phase);
			if (score != 0) {
				std::cout << "Rook (" << score << "): " << puzzle->fen << std::endl;
			}
		}
		if (seePieceSearch) {
			for (common::Square fieldIndex = common::Square::BEGIN; fieldIndex != common::Square::INVALID; ++fieldIndex) {
				for (common::PieceColor color = common::PieceColor::WHITE; color != common::PieceColor::INVALID; ++color) {
					const uint8_t attacks = board::ordering::SeePiece::getAttackingPiecesWithColor(*position, color, fieldIndex);
					if (common::Bitset(attacks).fastCount() > 4 && (position->colorPieceMask(color.invert(), position->m_pieceTable[fieldIndex]) & fieldIndex.asBitboard()) != 0) {
						std::cout << "See (" << std::bitset<8>(attacks) << ") square:" << fieldIndex << " color:" << color << " " << puzzle->fen << std::endl;
					}
				}
			}
		}
		if (seeSearch) {
			moves::Moves moves;
			board::Operators::getAvailableCaptureMoves(blunderPosition, moves);
			for (size_t moveIndex = 0; moveIndex < moves.size(); ++moveIndex) {
				const common::PieceColor enemyColor = blunderPosition.m_colorToMove.invert();

				const common::PieceType attackingPiece = blunderPosition.m_pieceTable[moves[moveIndex].from()];
				const common::PieceType capturedPiece = blunderPosition.m_pieceTable[moves[moveIndex].to()];

				const uint8_t attackers = board::ordering::SeePiece::getAttackingPiecesWithColor(blunderPosition, blunderPosition.m_colorToMove, moves[moveIndex].to());
				const uint8_t defenders = board::ordering::SeePiece::getAttackingPiecesWithColor(blunderPosition, enemyColor, moves[moveIndex].to());
				const int32_t seeEvaluation = board::ordering::StaticExchangeEvaluation::evaluate(attackingPiece, capturedPiece, attackers, defenders);
				if (seeEvaluation != 0 && common::Bitset(attackers).fastCount() > 2 && common::Bitset(defenders).fastCount() > 2) {
					std::cout << "StaticExchangeEvaluation " << seeEvaluation << " " << moves[moveIndex] << " " << blunderFen << std::endl;
				}
			}
		}
	}

	return 0;
}