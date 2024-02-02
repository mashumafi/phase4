#include <phase4/engine/board/ordering/static_exchange_evaluation.h>

#include <phase4/engine/common/piece_type.h>

namespace phase4::engine::board::ordering {

inline constexpr StaticExchangeEvaluation::Array StaticExchangeEvaluation::TABLE = {
	populate(common::PieceType::PAWN),
	populate(common::PieceType::KNIGHT),
	populate(common::PieceType::BISHOP),
	populate(common::PieceType::ROOK),
	populate(common::PieceType::QUEEN),
	populate(common::PieceType::KING),
};

}
