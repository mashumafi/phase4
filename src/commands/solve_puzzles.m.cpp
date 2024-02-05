#include <phase4/engine/fen/fen_validator.h>
#include <phase4/engine/fen/lichess_csv_parser.h>

#include <algorithm>
#include <atomic>
#include <condition_variable>
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

constexpr size_t CSV_LINE_LIMIT = 10000;

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

struct PuzzleStatistics {
	std::atomic<size_t> count = 0;
	std::atomic<size_t> success = 0;
	std::atomic<size_t> checkmate = 0;
	std::atomic<size_t> bestMoveFound = 0;
	std::atomic<size_t> completed = 0;

	std::atomic<uint16_t> lowestFailure = 4000;
	std::atomic<uint16_t> highestSuccess = 0;

	std::atomic<uint16_t> minRating = 4000;
	std::atomic<uint16_t> maxRating = 0;
	std::atomic<uint64_t> maxMoves = 0;

	// Failures
	std::atomic<size_t> invalidCsv = 0;
	std::atomic<size_t> invalidFen = 0;
	std::atomic<size_t> badMoveMiss = 0;
	std::atomic<size_t> badPrincipalVariation = 0;
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

int main() {
	using namespace phase4::engine;
	using namespace phase4::engine::moves::magic;

	MagicBitboards::initWithInternalKeys();

	fen::LichessCsvParser csvReader("/workspaces/phase4/puzzles/lichess_db_puzzle.csv");

	const uint32_t NUM_THREADS = std::max(std::thread::hardware_concurrency() - 1, 1u);
	ThreadPool pool(NUM_THREADS, 100);

	PuzzleStatistics statistics;
	while (auto puzzle = csvReader.nextPuzzle()) {
		if (statistics.count >= CSV_LINE_LIMIT) {
			break;
		}
		++statistics.count;

		auto task = [puzzle = std::move(puzzle), &statistics](board::Session &session) -> void {
			atomic_max(statistics.maxMoves, puzzle->expectedMoves.size());

			ai::search::SearchContext context(&session);
			context.maxDepth = (puzzle->expectedMoves.size() + 6);
			context.maxTime = 5'000;

			auto printFen = [&context, &puzzle]() {
				std::stringstream details;
				details << puzzle->rating << " " << puzzle->fen << " [" << puzzle->badMove << "]";
				for (size_t i = 0; i < puzzle->expectedMoves.size(); ++i) {
					details << " " << puzzle->expectedMoves[i];
				}

				details << " (";
				for (size_t i = 0; i < context.statistics.principalVariation.size(); ++i) {
					if (i > 0)
						details << " ";
					details << context.statistics.principalVariation[i];
				}
				details << ")";
				std::cout << details.str() << std::endl;
			};

			switch (fen::FenValidator::validate(puzzle->fen, puzzle->badMove, puzzle->expectedMoves, context)) {
				case fen::FenValidator::INVALID_FEN:
					++statistics.invalidFen;
					break;
				case fen::FenValidator::BAD_MOVE_FAILURE:
					++statistics.badMoveMiss;
					break;
				case fen::FenValidator::BAD_PRINCIPAL_VARIATION:
					++statistics.badPrincipalVariation;
					printFen();
					break;
				case fen::FenValidator::CHECKMATE:
					atomic_max(statistics.highestSuccess, puzzle->rating);
					++statistics.checkmate;
					++statistics.success;
					++statistics.bestMoveFound;
					break;
				case fen::FenValidator::MOVES_MATCH:
					atomic_max(statistics.highestSuccess, puzzle->rating);
					++statistics.success;
					++statistics.bestMoveFound;
					break;
				case fen::FenValidator::FOUND_BEST:
					atomic_min(statistics.lowestFailure, puzzle->rating);
					++statistics.bestMoveFound;
					break;
				case fen::FenValidator::FAILURE:
					atomic_min(statistics.lowestFailure, puzzle->rating);
					if (puzzle->rating < 1000) {
						printFen();
					}
					break;
			}
			atomic_min(statistics.minRating, puzzle->rating);
			atomic_max(statistics.maxRating, puzzle->rating);
			++statistics.completed;
		};

		pool.enqueue(std::move(task));

		if (statistics.count % (NUM_THREADS * NUM_THREADS) == 0) {
			std::cout << "Submitted: " << (100.0 * statistics.count) / CSV_LINE_LIMIT << std::endl;
		}
	}

	while (statistics.completed < statistics.count) {
		std::cout << "Completed: " << (100.0 * statistics.completed) / statistics.count << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	std::cout << "Total        : " << statistics.count << std::endl;
	std::cout << "Success      : " << statistics.success << std::endl;
	std::cout << "Checkmate    : " << statistics.checkmate << std::endl;
	std::cout << "Best Move    : " << statistics.bestMoveFound << std::endl;
	std::cout << "Max Moves    : " << statistics.maxMoves << std::endl;
	std::cout << "Errors       : " << statistics.badMoveMiss + statistics.invalidCsv + statistics.invalidFen + statistics.badPrincipalVariation << std::endl;
	std::cout << "Result Range : " << statistics.lowestFailure << "-" << statistics.highestSuccess << std::endl;
	std::cout << "Rating Range : " << statistics.minRating << "-" << statistics.maxRating << std::endl;

	return 0;
}