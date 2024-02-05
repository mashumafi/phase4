#include <phase4/engine/board/position_state.h>

#include <phase4/engine/board/position.h>
#include <phase4/engine/board/zobrist_hashing.h>

#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/position_constants.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("PositionState DEFAULT") {
	using namespace phase4::engine;

	const board::Position position = board::PositionState::DEFAULT;

	CHECK(position.m_hash != board::ZobristHashing(0));
	CHECK(position.m_pawnHash != board::ZobristHashing(0));

	CHECK(position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::PAWN) == common::PositionConstants::RANK_2);
	CHECK(position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::KNIGHT) == (common::Square::B1.asBitboard() | common::Square::G1.asBitboard()));
	CHECK(position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::BISHOP) == (common::Square::C1.asBitboard() | common::Square::F1.asBitboard()));
	CHECK(position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::ROOK) == (common::Square::A1.asBitboard() | common::Square::H1.asBitboard()));
	CHECK(position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::QUEEN) == common::Square::D1.asBitboard());
	CHECK(position.colorPieceMask(common::PieceColor::WHITE, common::PieceType::KING) == common::Square::E1.asBitboard());

	CHECK(position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::PAWN) == common::PositionConstants::RANK_7);
	CHECK(position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::KNIGHT) == (common::Square::B8.asBitboard() | common::Square::G8.asBitboard()));
	CHECK(position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::BISHOP) == (common::Square::C8.asBitboard() | common::Square::F8.asBitboard()));
	CHECK(position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::ROOK) == (common::Square::A8.asBitboard() | common::Square::H8.asBitboard()));
	CHECK(position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::QUEEN) == common::Square::D8.asBitboard());
	CHECK(position.colorPieceMask(common::PieceColor::BLACK, common::PieceType::KING) == common::Square::E8.asBitboard());

	CHECK(position.occupancy(common::PieceColor::WHITE) == common::Bitset(common::PositionConstants::RANK_1 | common::PositionConstants::RANK_2));
	CHECK(position.occupancy(common::PieceColor::BLACK) == common::Bitset(common::PositionConstants::RANK_7 | common::PositionConstants::RANK_8));

	CHECK(position.m_occupancySummary == common::Bitset(common::PositionConstants::RANK_1 | common::PositionConstants::RANK_2 | common::PositionConstants::RANK_7 | common::PositionConstants::RANK_8));
	CHECK(position.m_enPassant == common::Bitset(0));
	CHECK(position.m_castling == common::Castling::EVERYTHING);
	CHECK(position.m_colorToMove == common::PieceColor::WHITE);
	CHECK(position.m_movesCount == 1);
	CHECK(position.m_irreversibleMovesCount == 0);
	CHECK(position.m_nullMoves == 0);

	CHECK(position.m_castlingDone[0] == false);
	CHECK(position.m_castlingDone[1] == false);

	CHECK(position.material(common::PieceColor::WHITE) != 0);
	CHECK(position.material(common::PieceColor::BLACK) != 0);

	/*int32_t m_positionEval[2][2] = {};

	std::array<common::PieceType, 64> m_pieceTable = {};*/

	CHECK(position.m_walls == 0);
}
