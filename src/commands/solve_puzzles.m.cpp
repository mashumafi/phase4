#include <phase4/engine/fen/fen_to_position.h>

#include <phase4/engine/ai/search/iterative_deepening.h>
#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/board/operators.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/magic/magic_bitboards.h>
#include <phase4/engine/moves/move.h>

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace phase4::engine {

class ThreadPool {
public:
	ThreadPool(size_t numThreads, size_t maxTasks) :
			m_maxTasks(maxTasks) {
		for (size_t i = 0; i < numThreads; ++i) {
			m_threads.emplace_back([this] { workerThread(); });
		}
	}

	~ThreadPool() {
		shutdown();
	}

	void shutdown() {
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_stop = true;
		}

		m_condition.notify_all();

		for (auto &thread : m_threads) {
			thread.join();
		}
		m_threads.clear();
	}

	void enqueue(std::function<void(board::Session &)> &&task) {
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_condition.wait(lock, [this] { return m_tasks.size() < m_maxTasks; });

			m_tasks.emplace(std::move(task));
		}

		m_condition.notify_one();
	}

private:
	std::vector<std::thread> m_threads = std::vector<std::thread>();
	std::queue<std::function<void(board::Session &)>> m_tasks = std::queue<std::function<void(board::Session &)>>();
	std::mutex m_queueMutex;
	std::condition_variable m_condition;
	size_t m_maxTasks;
	bool m_stop = false;

	void workerThread() {
		auto session = std::make_unique<board::Session>();

		while (true) {
			std::function<void(board::Session &)> task;
			{
				std::unique_lock<std::mutex> lock(m_queueMutex);

				m_condition.wait(lock, [this] { return m_stop || !m_tasks.empty(); });

				if (m_stop && m_tasks.empty()) {
					return;
				}

				task = std::move(m_tasks.front());
				m_tasks.pop();
			}

			task(*session);
			m_condition.notify_all();
		}
	}
};

} //namespace phase4::engine

constexpr size_t CSV_LINE_LIMIT = 1000;

struct PuzzleStatistics {
	std::atomic<size_t> count = 0;
	std::atomic<size_t> success = 0;
	std::atomic<size_t> earlyExit = 0;
	std::atomic<size_t> checkmate = 0;
	std::atomic<size_t> bestMoveFound = 0;
	std::atomic<size_t> completed = 0;

	std::atomic<uint64_t> lowestFailure = 4000;
	std::atomic<uint64_t> highestSuccess = 0;

	std::atomic<uint64_t> minRating = 4000;
	std::atomic<uint64_t> maxRating = 0;
	std::atomic<uint64_t> maxMoves = 0;

	// Failures
	std::atomic<size_t> invalidCsv = 0;
	std::atomic<size_t> invalidFen = 0;
	std::atomic<size_t> badMoveMiss = 0;
};

template <typename T>
void atomic_min(std::atomic<T> &min, T value) {
	T currentMin = min.load();
	while (value < currentMin) {
		min.compare_exchange_strong(currentMin, value);
	}
}

template <typename T>
void atomic_max(std::atomic<T> &max, T value) {
	T currentMax = max.load();
	while (value > currentMax) {
		max.compare_exchange_weak(currentMax, value);
	}
}

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

		std::string header;
		if (!std::getline(file, header) || header != "PuzzleId,FEN,Moves,Rating,RatingDeviation,Popularity,NbPlays,Themes,GameUrl,OpeningTags") {
			return 1;
		}

		const uint32_t NUM_THREADS = std::max(std::thread::hardware_concurrency() - 1, 1u);
		ThreadPool pool(NUM_THREADS, 100);

		PuzzleStatistics statistics;
		std::string line;
		line.reserve(100);
		while (std::getline(file, line)) {
			if (statistics.count >= CSV_LINE_LIMIT) {
				break;
			}
			++statistics.count;

			auto task = [line = std::move(line), &statistics](board::Session &session) -> void {
				std::vector<std::string> fields;
				{
					fields.reserve(10);
					std::istringstream iss(line);
					std::string field;
					field.reserve(20);

					while (std::getline(iss, field, ',')) {
						fields.push_back(field);
					}

					if (fields.size() < 4) {
						++statistics.invalidCsv;
						++statistics.completed;
						return;
					}
				}

				std::string_view FEN = fields[1];
				moves::Moves moves;

				{
					std::istringstream iss(fields[2]);

					std::string token;
					while (iss >> token) {
						moves.emplace_back(token);
					}
				}

				atomic_max(statistics.maxMoves, moves.size());

				const std::optional<board::Position> position = fen::FenToPosition::parse(FEN);
				if (!position) {
					++statistics.invalidFen;
					++statistics.completed;
					return;
				}

				session.setPosition(*position);
				auto findRealMove = [&session](moves::Move badMove) -> std::optional<moves::Move> {
					moves::Moves moves;
					board::Operators::getAllMoves(session.m_position, moves);
					const bool isQuiet = badMove.flags() == moves::MoveFlags::QUIET;
					for (size_t i = 0; i < moves.size(); ++i) {
						const bool correctFlags = isQuiet || moves[i].flags().isPromotion();
						if (moves[i].from() == badMove.from() && moves[i].to() == badMove.to() && correctFlags) {
							return moves[i];
						}
					}

					return std::nullopt;
				};
				std::optional<moves::Move> badMove = findRealMove(moves[0]);
				if (!badMove) {
					++statistics.badMoveMiss;
					++statistics.completed;
					return;
				}
				session.makeMove(*badMove);

				ai::search::SearchContext context(&session);
				context.maxDepth = (moves.size() + 8);
				context.maxTime = 5'000;

				auto movesMatch = [&moves, &context]() -> bool {
					if (moves.size() > context.statistics.principalVariation.size()) {
						return false;
					}

					for (size_t i = 1; i < moves.size(); ++i) {
						moves::Move bestMove = context.statistics.principalVariation[i - 1];
						if (bestMove.to() != moves[i].to() || bestMove.from() != moves[i].from())
							return false;
					}

					return true;
				};

				moves::Move bestMove = ai::search::IterativeDeepening::findBestMove(context, [&context, &movesMatch, &statistics](const ai::search::SearchStatistics &searchStats) {
					if (movesMatch()) {
						context.abortSearch = true;
						if (searchStats.depth < context.maxDepth)
							++statistics.earlyExit;
					}
				});

				uint64_t rating = 0;
				const auto [_, ec] = std::from_chars(fields[3].data(), fields[3].data() + fields[3].size(), rating);
				if (ec == std::errc()) {
					atomic_min(statistics.minRating, rating);
					atomic_max(statistics.maxRating, rating);
				}

				if (board::SearchConstants::isScoreNearCheckmate(context.statistics.score) && context.statistics.principalVariation.size() == (moves.size() - 1)) {
					++statistics.checkmate;
					++statistics.success;
					++statistics.bestMoveFound;
					if (ec == std::errc()) {
						atomic_max(statistics.highestSuccess, rating);
					}
				} else if (movesMatch()) {
					++statistics.success;
					++statistics.bestMoveFound;
					if (ec == std::errc()) {
						atomic_max(statistics.highestSuccess, rating);
					}
				} else if (bestMove.to() == moves[1].to() || bestMove.from() == moves[1].from()) {
					++statistics.bestMoveFound;
					if (ec == std::errc()) {
						atomic_min(statistics.lowestFailure, rating);
					}
				} else {
					if (rating < 1000) {
						std::cout << rating << " " << FEN << " " << *badMove << std::endl;
					}
				}
				++statistics.completed;
			};

			pool.enqueue(std::move(task));

			std::cout << "Submitted: " << (100.0 * statistics.count) / CSV_LINE_LIMIT << std::endl;
		}

		while (statistics.completed < statistics.count) {
			std::cout << "Completed: " << (100.0 * statistics.completed) / statistics.count << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		std::cout << "Total        : " << statistics.count << std::endl;
		std::cout << "Success      : " << statistics.success << std::endl;
		std::cout << "Exit Early   : " << statistics.earlyExit << std::endl;
		std::cout << "Checkmate    : " << statistics.checkmate << std::endl;
		std::cout << "Best Move    : " << statistics.bestMoveFound << std::endl;
		std::cout << "Errors       : " << statistics.badMoveMiss + statistics.invalidCsv + statistics.invalidFen << std::endl;
		std::cout << "Rating Range : " << statistics.lowestFailure << "-" << statistics.highestSuccess << std::endl;
		std::cout << "Rating Range : " << statistics.minRating << "-" << statistics.maxRating << std::endl;

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