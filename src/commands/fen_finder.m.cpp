#include <phase4/engine/fen/fen_to_position.h>
#include <phase4/engine/fen/lichess_csv_parser.h>

#include <phase4/engine/ai/score/evaluation.h>
#include <phase4/engine/ai/score/evaluators/bishop_evaluator.h>
#include <phase4/engine/ai/score/evaluators/king_safety_evaluator.h>
#include <phase4/engine/ai/score/evaluators/material_evaluator.h>
#include <phase4/engine/ai/score/evaluators/mobility_evaluator.h>
#include <phase4/engine/ai/score/evaluators/pawn_structure_evaluator.h>
#include <phase4/engine/ai/score/evaluators/position_evaluator.h>
#include <phase4/engine/ai/score/evaluators/rook_evaluator.h>

#include <phase4/engine/common/position_constants.h>

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
	}

	if (bishopSearch) {
	}
	if (kingSafetySearch) {
	}
	if (materialSearch) {
	}
	if (mobilitySearch) {
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

	fen::LichessCsvParser csvReader("/workspaces/phase4/puzzles/lichess_db_puzzle.csv");

	while (auto puzzle = csvReader.nextPuzzle()) {
		if (!puzzle) {
			return 1;
		}

		std::optional<board::Position> position = fen::FenToPosition::parse(puzzle->fen);
		if (!position) {
			return 1;
		}

		if (bishopSearch) {
		}
		if (kingSafetySearch) {
		}
		if (materialSearch) {
			const int32_t score = ai::score::evaluators::MaterialEvaluator::evaluate(*position);
			if (score != 0 && puzzle->expectedMoves.size() > 10) {
				std::cout << "Material (" << score << "): " << puzzle->fen << " " << puzzle->badMove;
				for (size_t i = 0; i < puzzle->expectedMoves.size(); ++i) {
					std::cout << " " << puzzle->expectedMoves[i];
				}
				std::cout << std::endl;
			}
		}
		if (mobilitySearch) {
		}
		if (pawnStructureSearch) {
			const int32_t score = ai::score::evaluators::PawnStructureEvaluator::evaluateWithoutCache(*position, phase, common::PositionConstants::PHASE_RESOLUTION - phase);
			if (score != 0) {
				std::cout << "PawnStructure (" << score << "): " << puzzle->fen << std::endl;
			}
		}
		if (positionSearch) {
		}
		if (rookSearch) {
			const int32_t score = ai::score::evaluators::RookEvaluator::evaluate(*position, phase, 1024 - phase);
			if (score != 0) {
				std::cout << "Rook (" << score << "): " << puzzle->fen << std::endl;
			}
		}
	}

	return 0;
}