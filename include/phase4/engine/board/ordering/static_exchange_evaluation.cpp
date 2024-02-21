#include <phase4/engine/board/ordering/static_exchange_evaluation.h>

#include <phase4/engine/common/piece_type.h>

namespace phase4::engine::board::ordering {

const StaticExchangeEvaluation::Array StaticExchangeEvaluation::PAWN_TABLE = populate(common::PieceType::PAWN);
const StaticExchangeEvaluation::Array StaticExchangeEvaluation::KNIGHT_TABLE = populate(common::PieceType::KNIGHT);
const StaticExchangeEvaluation::Array StaticExchangeEvaluation::BISHOP_TABLE = populate(common::PieceType::BISHOP);
const StaticExchangeEvaluation::Array StaticExchangeEvaluation::ROOK_TABLE = populate(common::PieceType::ROOK);
const StaticExchangeEvaluation::Array StaticExchangeEvaluation::QUEEN_TABLE = populate(common::PieceType::QUEEN);
const StaticExchangeEvaluation::Array StaticExchangeEvaluation::KING_TABLE = populate(common::PieceType::KING);

} //namespace phase4::engine::board::ordering
