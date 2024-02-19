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

#include <phase4/engine/board/operators.h>
#include <phase4/engine/board/operators/bishop_operator.h>
#include <phase4/engine/board/operators/king_operator.h>
#include <phase4/engine/board/operators/knight_operator.h>
#include <phase4/engine/board/operators/pawn_operator.h>
#include <phase4/engine/board/operators/queen_operator.h>
#include <phase4/engine/board/operators/rook_operator.h>
#include <phase4/engine/board/ordering/see_piece.h>
#include <phase4/engine/board/ordering/static_exchange_evaluation.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/position_constants.h>
#include <phase4/engine/common/square.h>

#include <vector>

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
	bool quietSearch = false; // -quiet
	bool loudSearch = false; // -loud
	bool captureSearch = false; // -capture
	int32_t phase = common::PositionConstants::PHASE_RESOLUTION;
	std::vector<common::PieceType> pieces;

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
		if (args[i] == "-quiet"sv) {
			quietSearch = true;
		}
		if (args[i] == "-loud"sv) {
			loudSearch = true;
		}
		if (args[i] == "-capture"sv) {
			captureSearch = true;
		}
		if (args[i] == "-P"sv) {
			pieces.push_back(common::PieceType::PAWN);
		}
		if (args[i] == "-N"sv) {
			pieces.push_back(common::PieceType::KNIGHT);
		}
		if (args[i] == "-B"sv) {
			pieces.push_back(common::PieceType::BISHOP);
		}
		if (args[i] == "-R"sv) {
			pieces.push_back(common::PieceType::ROOK);
		}
		if (args[i] == "-Q"sv) {
			pieces.push_back(common::PieceType::QUEEN);
		}
		if (args[i] == "-K"sv) {
			pieces.push_back(common::PieceType::KING);
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
	if (quietSearch) {
		std::cout << "Search will include Operator::getQuietMoves" << std::endl;
	}
	if (loudSearch) {
		std::cout << "Search will include Operator::getLoudMoves" << std::endl;
	}
	if (captureSearch) {
		std::cout << "Search will include Operator::getAvailableCaptureMoves" << std::endl;
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

		if (quietSearch) {
			const common::Bitset fieldsAttackedByEnemy = position->getEvasionMask();
			for (const common::PieceType piece : pieces) {
				if (position->colorPieceMask(position->m_colorToMove, piece) == 0) {
					continue;
				}
				moves::Moves moves;
				switch (piece.get_raw_value()) {
					case common::PieceType::PAWN.get_raw_value():
						board::operators::PawnOperator::getQuietMoves(*position, moves, fieldsAttackedByEnemy);
						break;
					case common::PieceType::KNIGHT.get_raw_value():
						board::operators::KnightOperator::getQuietMoves(*position, moves, fieldsAttackedByEnemy);
						break;
					case common::PieceType::BISHOP.get_raw_value():
						board::operators::BishopOperator::getQuietMoves(*position, moves, fieldsAttackedByEnemy);
						break;
					case common::PieceType::ROOK.get_raw_value():
						board::operators::RookOperator::getQuietMoves(*position, moves, fieldsAttackedByEnemy);
						break;
					case common::PieceType::QUEEN.get_raw_value():
						board::operators::QueenOperator::getQuietMoves(*position, moves, fieldsAttackedByEnemy);
						break;
					case common::PieceType::KING.get_raw_value():
						board::operators::KingOperator::getQuietMoves(*position, moves);
						break;
				}
				std::cout << "Quiet (" << moves.size() << "): " << piece << " " << puzzle->fen << std::endl;
			}
		}
		if (loudSearch) {
			const common::Bitset fieldsAttackedByEnemy = position->getEvasionMask();
			for (const common::PieceType piece : pieces) {
				if (position->colorPieceMask(position->m_colorToMove, piece) == 0) {
					continue;
				}
				moves::Moves moves;
				switch (piece.get_raw_value()) {
					case common::PieceType::PAWN.get_raw_value():
						board::operators::PawnOperator::getLoudMoves(*position, moves, fieldsAttackedByEnemy);
						break;
					case common::PieceType::KNIGHT.get_raw_value():
						board::operators::KnightOperator::getLoudMoves(*position, moves, fieldsAttackedByEnemy);
						break;
					case common::PieceType::BISHOP.get_raw_value():
						board::operators::BishopOperator::getLoudMoves(*position, moves, fieldsAttackedByEnemy);
						break;
					case common::PieceType::ROOK.get_raw_value():
						board::operators::RookOperator::getLoudMoves(*position, moves, fieldsAttackedByEnemy);
						break;
					case common::PieceType::QUEEN.get_raw_value():
						board::operators::QueenOperator::getLoudMoves(*position, moves, fieldsAttackedByEnemy);
						break;
					case common::PieceType::KING.get_raw_value():
						board::operators::KingOperator::getLoudMoves(*position, moves);
						break;
				}
				std::cout << "Loud (" << moves.size() << "): " << piece << " " << puzzle->fen << std::endl;
			}
		}
		if (captureSearch) {
			for (const common::PieceType piece : pieces) {
				if (position->colorPieceMask(position->m_colorToMove, piece) == 0) {
					continue;
				}
				moves::Moves moves;
				switch (piece.get_raw_value()) {
					case common::PieceType::PAWN.get_raw_value():
						board::operators::PawnOperator::getAvailableCaptureMoves(*position, moves);
						break;
					case common::PieceType::KNIGHT.get_raw_value():
						board::operators::KnightOperator::getAvailableCaptureMoves(*position, moves);
						break;
					case common::PieceType::BISHOP.get_raw_value():
						board::operators::BishopOperator::getAvailableCaptureMoves(*position, moves);
						break;
					case common::PieceType::ROOK.get_raw_value():
						board::operators::RookOperator::getAvailableCaptureMoves(*position, moves);
						break;
					case common::PieceType::QUEEN.get_raw_value():
						board::operators::QueenOperator::getAvailableCaptureMoves(*position, moves);
						break;
					case common::PieceType::KING.get_raw_value():
						board::operators::KingOperator::getAvailableCaptureMoves(*position, moves);
						break;
				}
				std::cout << "Capure (" << moves.size() << "): " << piece << " " << puzzle->fen << std::endl;
			}
		}
	}

	return 0;
}