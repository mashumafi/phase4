#ifndef PHASE4_ENGINE_MOVES_RESULT_H
#define PHASE4_ENGINE_MOVES_RESULT_H

#include <phase4/engine/common/fast_vector.h>
#include <phase4/engine/common/field_index.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/square.h>

#include <optional>

namespace phase4::engine::moves {

struct Result {
	struct Movement {
		common::Square from;
		common::Square to;

		bool operator==(const Result::Movement &that) const {
			return from == that.from && to == that.to;
		}
	};

	struct PieceAndSquare {
		common::PieceType pieceType;
		common::Square at;

		bool operator==(Result::PieceAndSquare that) const {
			return pieceType == that.pieceType && at == that.at;
		}
	};

	common::FastVector<Movement, 2> moved;

	std::optional<PieceAndSquare> added;

	std::optional<common::PieceType> promotion;

	std::optional<PieceAndSquare> killed;
	common::FastVector<PieceAndSquare, 2> removed;

	std::optional<common::FieldIndex> slide;
};

} //namespace phase4::engine::moves

#endif
