#include <phase4/engine/board/position_state.h>

#include <phase4/engine/board/board_constants.h>
#include <phase4/engine/board/position.h>
#include <phase4/engine/board/zobrist_hashing.h>
#include <phase4/engine/common/bitset.h>
#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>

#include <doctest/doctest.h>

TEST_CASE("PositionState DEFAULT") {
	using namespace phase4::engine;

	constexpr board::Position position = board::PositionState::DEFAULT;

	CHECK(position.m_hash != board::ZobristHashing(0));
	CHECK(position.m_pawnHash != board::ZobristHashing(0));

	CHECK(position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::PAWN.get_raw_value()] == board::BoardConstants::RANK_2);
	CHECK(position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::KNIGHT.get_raw_value()] == (common::Square::B1.asBitboard() | common::Square::G1.asBitboard()));
	CHECK(position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::BISHOP.get_raw_value()] == (common::Square::C1.asBitboard() | common::Square::F1.asBitboard()));
	CHECK(position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::ROOK.get_raw_value()] == (common::Square::A1.asBitboard() | common::Square::H1.asBitboard()));
	CHECK(position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::QUEEN.get_raw_value()] == common::Square::D1.asBitboard());
	CHECK(position.m_colorPieceMasks[common::PieceColor::WHITE.get_raw_value()][common::PieceType::KING.get_raw_value()] == common::Square::E1.asBitboard());

	CHECK(position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::PAWN.get_raw_value()] == board::BoardConstants::RANK_7);
	CHECK(position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::KNIGHT.get_raw_value()] == (common::Square::B8.asBitboard() | common::Square::G8.asBitboard()));
	CHECK(position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::BISHOP.get_raw_value()] == (common::Square::C8.asBitboard() | common::Square::F8.asBitboard()));
	CHECK(position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::ROOK.get_raw_value()] == (common::Square::A8.asBitboard() | common::Square::H8.asBitboard()));
	CHECK(position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::QUEEN.get_raw_value()] == common::Square::D8.asBitboard());
	CHECK(position.m_colorPieceMasks[common::PieceColor::BLACK.get_raw_value()][common::PieceType::KING.get_raw_value()] == common::Square::E8.asBitboard());

	CHECK(position.m_occupancyByColor[0] == common::Bitset(board::BoardConstants::RANK_1 | board::BoardConstants::RANK_2));
	CHECK(position.m_occupancyByColor[1] == common::Bitset(board::BoardConstants::RANK_7 | board::BoardConstants::RANK_8));

	CHECK(position.m_occupancySummary == common::Bitset(board::BoardConstants::RANK_1 | board::BoardConstants::RANK_2 | board::BoardConstants::RANK_7 | board::BoardConstants::RANK_8));
	CHECK(position.m_enPassant == common::Bitset(0));
	CHECK(position.m_castling == common::Castling::EVERYTHING);
	CHECK(position.m_colorToMove == common::PieceColor::WHITE);
	CHECK(position.m_movesCount == 0);
	CHECK(position.m_irreversibleMovesCount == 0);
	CHECK(position.m_nullMoves == 0);

	CHECK(position.m_castlingDone[0] == false);
	CHECK(position.m_castlingDone[1] == false);

	CHECK(position.m_material[0] != 0);
	CHECK(position.m_material[1] != 0);

	/*int32_t m_positionEval[2][2] = {};

	std::array<common::PieceType, 64> m_pieceTable = {};*/

	CHECK(position.m_walls == 0);
}
