#include <phase4/engine/board/ordering/static_exchange_evaluation.h>

#include <phase4/engine/common/piece_type.h>

namespace phase4::engine::board::ordering {

constexpr StaticExchangeEvaluation::Array StaticExchangeEvaluation::PAWN_TABLE = populate(common::PieceType::PAWN);
constexpr StaticExchangeEvaluation::Array StaticExchangeEvaluation::KNIGHT_TABLE = populate(common::PieceType::KNIGHT);
constexpr StaticExchangeEvaluation::Array StaticExchangeEvaluation::BISHOP_TABLE = populate(common::PieceType::BISHOP);
constexpr StaticExchangeEvaluation::Array StaticExchangeEvaluation::ROOK_TABLE = populate(common::PieceType::ROOK);
constexpr StaticExchangeEvaluation::Array StaticExchangeEvaluation::QUEEN_TABLE = populate(common::PieceType::QUEEN);
constexpr StaticExchangeEvaluation::Array StaticExchangeEvaluation::KING_TABLE = populate(common::PieceType::KING);

} //namespace phase4::engine::board::ordering
