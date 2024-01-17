#ifndef PHASE4_ENGINE_BOARD_SESSION_H
#define PHASE4_ENGINE_BOARD_SESSION_H

#include <phase4/engine/common/castling.h>
#include <phase4/engine/common/vector.h>

#include <cstdint>

namespace phase4::engine::board {

class Session {
public:
	Session();

	int64_t Hash;
	int64_t PawnHash;

private:
	common::Vector<int> m_killedPieces;
	common::Vector<int64_t> m_enPassants;
	common::Vector<common::Castling> _castlings;
	common::Vector<int> m_promotedPieces;
	common::Vector<int64_t> m_hashes;
	common::Vector<int64_t> m_pawnHashes;
	common::Vector<int> m_irreversibleMovesCounts;
	//common::Vector<Position> _wallSlides;
};

Session::Session() :
		m_killedPieces(),
		m_enPassants(),
		m_promotedPieces(),
		m_hashes(),
		m_pawnHashes(),
		m_irreversibleMovesCounts() {
}

} //namespace phase4::engine::board

#endif
