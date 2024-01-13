#ifndef PHASE4_COMMON_CASTLING_H
#define PHASE4_COMMON_CASTLING_H

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

std::ostream &operator<<(std::ostream &os, const Castling &castling) {
	switch (castling) {
		case Castling::NONE:
			os << "NONE";
			break;
		case Castling::WHITE_SHORT:
			os << "WHITE_SHORT";
			break;
		case Castling::WHITE_LONG:
			os << "WHITE_LONG";
			break;
		case Castling::BLACK_SHORT:
			os << "BLACK_SHORT";
			break;
		case Castling::BLACK_LONG:
			os << "BLACK_LONG";
			break;
		case Castling::WHITE_CASTLING:
			os << "WHITE_CASTLING";
			break;
		case Castling::BLACK_CASTLING:
			os << "BLACK_CASTLING";
			break;
		case Castling::EVERYTHING:
			os << "EVERYTHING";
			break;
		default:
			if (castling > EVERYTHING) {
				os.setstate(std::ios_base::failbit);
				return os;
			}

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

} //namespace phase4::engine::common

#endif
