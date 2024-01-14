#ifndef PHASE4_ENGINE_COMMON_CASTLING_H
#define PHASE4_ENGINE_COMMON_CASTLING_H

#include <phase4/engine/common/util.h>

#include <cstdint>
#include <iostream>

namespace phase4::engine::common {

enum Castling : uint8_t {
	NONE = 0,
	WHITE_SHORT = 1,
	WHITE_LONG = 2,
	BLACK_SHORT = 4,
	BLACK_LONG = 8,
	WHITE_CASTLING = WHITE_SHORT | WHITE_LONG,
	BLACK_CASTLING = BLACK_SHORT | BLACK_LONG,
	EVERYTHING = WHITE_SHORT | WHITE_LONG | BLACK_SHORT | BLACK_LONG
};

std::ostream &operator<<(std::ostream &os, Castling castling) {
	switch (castling) {
		case Castling::NONE:
			return os << "NONE";
		case Castling::WHITE_SHORT:
			return os << "WHITE_SHORT";
		case Castling::WHITE_LONG:
			return os << "WHITE_LONG";
		case Castling::BLACK_SHORT:
			return os << "BLACK_SHORT";
		case Castling::BLACK_LONG:
			return os << "BLACK_LONG";
		case Castling::WHITE_CASTLING:
			return os << "WHITE_CASTLING";
		case Castling::BLACK_CASTLING:
			return os << "BLACK_CASTLING";
		case Castling::EVERYTHING:
			return os << "EVERYTHING";
		default:
			if (unlikely(castling > EVERYTHING)) {
				os.setstate(std::ios_base::failbit);
			} else {
				bool isFirstFlag = true;

				for (uint8_t flag = 1; flag <= EVERYTHING; flag <<= 1) {
					if (castling & flag) {
						if (!isFirstFlag) {
							os << " | ";
						}
						os << static_cast<Castling>(flag);
						isFirstFlag = false;
					}
				}
			}
			return os;
	}
}

} //namespace phase4::engine::common

#endif
