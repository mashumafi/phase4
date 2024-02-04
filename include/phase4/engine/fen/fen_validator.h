#ifndef PHASE4_ENGINE_FEN_FEN_VALIDATOR_H
#define PHASE4_ENGINE_FEN_FEN_VALIDATOR_H

#include <phase4/engine/fen/fen_to_position.h>
#include <phase4/engine/fen/position_to_fen.h>

#include <phase4/engine/ai/search/iterative_deepening.h>
#include <phase4/engine/ai/search/search_context.h>

#include <phase4/engine/board/operators.h>
#include <phase4/engine/board/position.h>
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

	static Result validate(std::string_view fen, moves::Move badMove, const moves::Moves &expectedMoves, ai::search::SearchContext &context) {
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

		std::optional<moves::Move> realMove = findRealMove(context.session->m_position, badMove);
		if (!realMove) {
			return BAD_MOVE_FAILURE;
		}
		context.session->makeMove(*realMove);

		if (!validatePrincipalVariation(context.session->m_position, expectedMoves, 0)) {
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

		moves::Move bestMove = ai::search::IterativeDeepening::findBestMove(context, [&context, &movesMatch](const ai::search::SearchStatistics &) {
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

private:
	static std::optional<moves::Move> findRealMove(const board::Position &position, moves::Move move) {
		moves::Moves moves;
		board::Operators::getAllMoves(position, moves);
		const bool isQuiet = move.flags() == moves::MoveFlags::QUIET;
		for (size_t i = 0; i < moves.size(); ++i) {
			const bool correctFlags = isQuiet || (moves[i].flags().isPromotion() && moves[i].flags().getPromotionPiece() == move.flags().getPromotionPiece());
			if (moves[i].from() == move.from() && moves[i].to() == move.to() && correctFlags) {
				return moves[i];
			}
		}

		return std::nullopt;
	};

	static bool validatePrincipalVariation(const board::Position &position, const moves::Moves &moves, size_t index) {
		if (index >= moves.size()) {
			return true;
		}

		board::Position mutablePosition = position;
		const std::optional<moves::Move> pvMove = findRealMove(mutablePosition, moves[index]);
		if (!pvMove) {
			std::cerr << "Could not find move " << moves[index] << std::endl;
			return false;
		}
		mutablePosition.makeMove(*pvMove);

		return validatePrincipalVariation(mutablePosition, moves, index + 1);
	}
};

} //namespace phase4::engine::fen

#endif
