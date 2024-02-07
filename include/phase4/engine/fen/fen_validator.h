#ifndef PHASE4_ENGINE_FEN_FEN_VALIDATOR_H
#define PHASE4_ENGINE_FEN_FEN_VALIDATOR_H

#include <phase4/engine/fen/fen_to_position.h>
#include <phase4/engine/fen/position_to_fen.h>

#include <phase4/engine/ai/search/iterative_deepening.h>
#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/board/operators.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/position_moves.h>
#include <phase4/engine/board/session.h>

#include <phase4/engine/moves/magic/magic_bitboards.h>
#include <phase4/engine/moves/move.h>

namespace phase4::engine::fen {

class FenValidator {
public:
	enum Result {
		INVALID_FEN,
		BAD_MOVE_FAILURE,
		BAD_PRINCIPAL_VARIATION,
		CHECKMATE,
		MOVES_MATCH,
		FOUND_BEST,
		FAILURE,
	};

	static Result validate(std::string_view fen, moves::Move blunder, const moves::Moves &expectedMoves, ai::search::SearchContext &context) {
		{
			const std::optional<board::Position> position = fen::FenToPosition::parse(fen);
			if (!position) {
				return INVALID_FEN;
			}

			if (fen::PositionToFen::encode(*position) != fen) {
				return INVALID_FEN;
			}

			context.session->setPosition(*position);
		}

		const std::optional<moves::Move> realMove = board::PositionMoves::findRealMove(context.session->position(), blunder);
		if (!realMove) {
			return BAD_MOVE_FAILURE;
		}
		context.session->makeMove(*realMove);

		if (!board::PositionMoves::makeMoves(context.session->position(), expectedMoves)) {
			return BAD_PRINCIPAL_VARIATION;
		}

		auto movesMatch = [&expectedMoves, &context]() -> bool {
			if (expectedMoves.size() > context.statistics.principalVariation.size()) {
				return false;
			}

			for (size_t i = 0; i < expectedMoves.size(); ++i) {
				moves::Move bestMove = context.statistics.principalVariation[i];
				moves::Move expectedMove = expectedMoves[i];
				if (bestMove.to() != expectedMove.to() || bestMove.from() != expectedMove.from())
					return false;
			}

			return true;
		};

		const moves::Move bestMove = ai::search::IterativeDeepening::findBestMove(context, [&context, &movesMatch](const ai::search::SearchStatistics &) {
			if (movesMatch()) {
				context.abortSearch = true;
			}
		});

		if (board::SearchConstants::isScoreNearCheckmate(context.statistics.score) && context.statistics.principalVariation.size() == expectedMoves.size()) {
			return CHECKMATE;
		} else if (movesMatch()) {
			return MOVES_MATCH;
		} else if (bestMove.to() == expectedMoves[0].to() || bestMove.from() == expectedMoves[0].from()) {
			return FOUND_BEST;
		} else {
			return FAILURE;
		}
	}
};

} //namespace phase4::engine::fen

#endif
