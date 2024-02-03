#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/ai/search/iterative_deepening.h>
#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/board/operators.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/magic/magic_bitboards.h>
#include <phase4/engine/moves/move.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

constexpr size_t CSV_LINE_LIMIT = 1000;

class CsvReader {
public:
	CsvReader(const std::string &filename) :
			file(filename) {
	}

	~CsvReader() {
		if (file.is_open()) {
			file.close();
		}
	}

	int processFile() {
		using namespace phase4::engine;

		if (!file.is_open()) {
			std::cerr << "Error opening the file." << std::endl;
			return 1;
		}

		auto session = std::make_unique<board::Session>();

		std::string line;
		std::vector<std::string> fields;
		fields.reserve(10);
		size_t count = 0;
		while (std::getline(file, line)) {
			++count;
			if (count > CSV_LINE_LIMIT) {
				break;
			}
			if (count == 1) {
				continue;
			}

			{
				fields.clear();
				std::istringstream iss(line);
				std::string field;

				while (std::getline(iss, field, ',')) {
					fields.push_back(field);
				}

				if (fields.size() < 4) {
					std::cerr << "CSV(" << count << ") is not valid: " << fields.size() << std::endl;
					return 2;
				}
			}

			// PuzzleId,FEN,Moves,Rating,RatingDeviation,Popularity,NbPlays,Themes,GameUrl,OpeningTags
			std::string_view FEN = fields[1];
			moves::Moves moves;

			{
				std::istringstream iss(fields[2]);

				std::string token;
				while (iss >> token) {
					std::cout << "parsing:" << token << std::endl;
					moves.emplace_back(token);
				}
			}

			const std::optional<board::Position> position = fen::FenToPosition::parse(FEN);
			if (!position) {
				std::cerr << "Invalid FEN(" << count << "): " << FEN << std::endl;
				return 3;
			}

			session->setPosition(*position);
			auto findRealMove = [&session](moves::Move badMove) -> std::optional<moves::Move> {
				moves::Moves moves;
				board::Operators::getAllMoves(session->m_position, moves);
				for (size_t i = 0; i < moves.size(); ++i) {
					const bool isQuiet = badMove.flags() == moves::MoveFlags::QUIET;
					const bool correctFlags = isQuiet || moves[i].flags() == badMove.flags();
					if (moves[i].from() == badMove.from() && moves[i].to() == badMove.to() && correctFlags) {
						return moves[i];
					}
				}
				return std::nullopt;
			};
			std::optional<moves::Move> badMove = findRealMove(moves[0]);
			if (!badMove) {
				std::cerr << "Could not find the move" << std::endl;
				return 5;
			}
			session->makeMove(*badMove);

			ai::search::SearchContext context(session.get());
			context.maxDepth = 16;
			moves::Move bestMove = ai::search::IterativeDeepening::findBestMove(context, [](const ai::search::SearchStatistics &) {});

			if (bestMove != moves[1]) {
				std::cerr << "Rating:" << fields[3] << std::endl;
				for (size_t i = 0; i < context.statistics.principalVariation.size(); ++i) {
					std::cerr << "Found: " << context.statistics.principalVariation[i] << std::endl;
				}
				for (size_t i = 0; i < moves.size(); ++i) {
					std::cerr << "Expected: " << moves[i] << std::endl;
				}
				return 4;
			}
		}

		return 0;
	}

private:
	std::ifstream file;
};

int main() {
	using namespace phase4::engine::moves::magic;

	MagicBitboards::initWithInternalKeys();

	CsvReader csvReader("/workspaces/phase4/puzzles/lichess_db_puzzle.csv");

	return csvReader.processFile();
}