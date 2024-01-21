#include <benchmark/benchmark.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/session.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/moves/magic/magic_bitboards.h>
#include <phase4/engine/moves/move.h>
#include <phase4/engine/moves/move_flags.h>

// Make a move, check if king is in check, undo the move
static void MakeMoveUndo(benchmark::State &state) {
	using namespace phase4::engine;

	moves::magic::MagicBitboards::initWithInternalKeys();

	board::Session session;
	session.SetDefaultState();
	constexpr moves::Move move(common::Square::B2, common::Square::C2, moves::MoveFlags::QUIET);

	for (auto _ : state) {
		session.makeMove(move);
		session.isKingChecked(common::PieceColor::BLACK);
		session.undoMove(move);
	}
}
BENCHMARK(MakeMoveUndo);

// Check if move is valid
static void isMoveLegal(benchmark::State &state) {
	using namespace phase4::engine;

	moves::magic::MagicBitboards::initWithInternalKeys();

	phase4::engine::board::Session session;
	session.SetDefaultState();
	constexpr moves::Move move(common::Square::B2, common::Square::C2, moves::MoveFlags::QUIET);

	for (auto _ : state) {
		session.isMoveLegal(move);
	}
}
BENCHMARK(isMoveLegal);

// Make a copy, make a move, check if king is in check
static void CopyMakeMove(benchmark::State &state) {
	using namespace phase4::engine;

	moves::magic::MagicBitboards::initWithInternalKeys();

	phase4::engine::board::Position position;
	position.SetDefaultState();
	constexpr moves::Move move(common::Square::B2, common::Square::C2, moves::MoveFlags::QUIET);

	for (auto _ : state) {
		phase4::engine::board::Position positionCopy = position;
		positionCopy.makeMove(move);
		positionCopy.isKingChecked(common::PieceColor::BLACK);
	}
}
BENCHMARK(CopyMakeMove);

BENCHMARK_MAIN();
