#include <phase4/engine/common/piece_color.h>
#include <phase4/engine/common/piece_type.h>
#include <phase4/engine/common/random.h>
#include <phase4/engine/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::engine::board {

class ZobristHashing {
public:
	struct Keys {
		static constexpr int BoardSize = 64;
		static constexpr int NumColors = 2;
		static constexpr int NumPieces = 6;
		static constexpr int NumCastlingOptions = 4;
		static constexpr int NumEnPassantOptions = 8;
		static constexpr int NumFieldHashes = NumColors * NumPieces * BoardSize;

		template <size_t Size>
		static constexpr std::array<uint64_t, Size> populateHashArray(common::Random &random) {
            return populateRow(random, std::make_index_sequence<Size>{});
		}

		// Recursive constexpr function to populate the array
		template <std::size_t SQUARE>
		static constexpr uint64_t populateElement(common::Random &random) {
            return random.next();
        }

		// Recursive constexpr function to fill a row of the array
		template <std::size_t... SQUARE>
		static constexpr std::array<uint64_t, sizeof...(SQUARE)> populateRow(common::Random &random, std::index_sequence<SQUARE...>) {
            return { populateElement<SQUARE>(random)... };
        }

		constexpr Keys(common::Random random);

		uint64_t m_colorHash;
		std::array<uint64_t, NumCastlingOptions> m_castlingHashes;
		std::array<uint64_t, NumEnPassantOptions> m_enPassantHashes;
		std::array<uint64_t, NumFieldHashes> m_fieldHashes;
		std::array<uint64_t, BoardSize> m_wallHash;
	};

	static const Keys G_KEYS;
};

constexpr ZobristHashing::Keys::Keys(common::Random random) :
		m_colorHash(random.next()),
		m_castlingHashes(populateHashArray<NumCastlingOptions>(random)),
		m_enPassantHashes(populateHashArray<NumEnPassantOptions>(random)),
		m_fieldHashes(populateHashArray<NumFieldHashes>(random)),
		m_wallHash(populateHashArray<BoardSize>(random)) {
}

constexpr ZobristHashing::Keys G_KEYS(common::Random(123456));

} //namespace phase4::engine::board