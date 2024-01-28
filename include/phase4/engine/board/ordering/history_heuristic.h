#ifndef PHASE4_ENGINE_BOARD_ORDERING_HISTORY_HEURISTIC_H
#define PHASE4_ENGINE_BOARD_ORDERING_HISTORY_HEURISTIC_H

#include <phase4/engine/common/piece_color.h>

#include <cstdint>

namespace phase4::engine::board::ordering {

class HistoryHeuristic {
public:
	static HistoryHeuristic() {
		_historyMoves = new uint[2][][];
		_historyMoves[Color.White] = new uint[6][];
		_historyMoves[Color.Black] = new uint[6][];

		for (var piece = 0; piece < 6; piece++) {
			_historyMoves[Color.White][piece] = new uint[64];
			_historyMoves[Color.Black][piece] = new uint[64];
		}

		_max = 1;
	}

	static void AddHistoryMove(int color, int piece, int to, int depth) {
		var newValue = _historyMoves[color][piece][to] + (uint)(depth * depth);

		_max = Math.Max(_max, newValue);
		_historyMoves[color][piece][to] = newValue;
	}

	static short GetMoveValue(int color, int piece, int to, uint scale) {
		return (short)(_historyMoves[color][piece][to] * scale / _max);
	}

	static uint GetRawMoveValue(int color, int piece, int to) {
		return _historyMoves[color][piece][to];
	}

	static uint GetMaxValue() {
		return _max;
	}

	static void AgeValues() {
		for (var color = 0; color < 2; color++) {
			for (var piece = 0; piece < 6; piece++) {
				for (var to = 0; to < 64; to++) {
					_historyMoves[color][piece][to] /= 2;
				}
			}
		}

		_max = Math.Max(_max / 2, 1);
	}

	static void Clear() {
		for (var color = 0; color < 2; color++) {
			for (var piece = 0; piece < 6; piece++) {
				Array.Clear(_historyMoves[color][piece], 0, 64);
			}
		}

		_max = 1;
	}

private:
	static readonly uint[][][] _historyMoves;
	static uint _max;
};

} //namespace phase4::engine::board::ordering

#endif
