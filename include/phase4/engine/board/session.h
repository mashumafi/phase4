#ifndef PHASE4_ENGINE_BOARD_SESSION_H
#define PHASE4_ENGINE_BOARD_SESSION_H

#include <phase4/engine/common/vector.h>

#include <cstdint>
#include <memory_resource>

namespace phase4::engine::board {

class Session {
public:
	Session(std::pmr::memory_resource *memory_resource = std::pmr::get_default_resource());

	int64_t Hash;
	int64_t PawnHash;

private:
	common::Vector<int> m_killedPieces;
	common::Vector<int64_t> m_enPassants;
	//common::Vector<Castling> _castlings;
	common::Vector<int> m_promotedPieces;
	common::Vector<int64_t> m_hashes;
	common::Vector<int64_t> m_pawnHashes;
	common::Vector<int> m_irreversibleMovesCounts;
	//common::Vector<Position> _wallSlides;
};

Session::Session(std::pmr::memory_resource *memory_resource) :
		m_killedPieces(memory_resource),
		m_enPassants(memory_resource),
		m_promotedPieces(memory_resource),
		m_hashes(memory_resource),
		m_pawnHashes(memory_resource),
		m_irreversibleMovesCounts(memory_resource) {
}

} //namespace phase4::engine::board
#endif
