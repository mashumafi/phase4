#ifndef PHASE4_COMMON_DISTANCE_H
#define PHASE4_COMMON_DISTANCE_H

#include <phase4/common/square.h>

#include <array>
#include <cstdint>

namespace phase4::common {

class Distance {
public:
	using Table = std::array<std::array<uint8_t, 64>, 64>;

	static constexpr Table populate();

	static constexpr uint8_t lookup(std::size_t i, std::size_t j);

private:
	// Recursive constexpr function to populate the array
	template <std::size_t FROM, std::size_t TO>
	static constexpr uint8_t populateElement();

	// Recursive constexpr function to fill a row of the array
	template <std::size_t FROM, std::size_t... TO>
	static constexpr std::array<uint8_t, sizeof...(TO)> populateRow(std::index_sequence<TO...>);

	// Recursive constexpr function to fill the entire array
	template <std::size_t... FROM>
	static constexpr std::array<std::array<uint8_t, 64>, sizeof...(FROM)> populateArray(std::index_sequence<FROM...>);

	static const Table G_DISTANCE_TABLE;
};

constexpr uint8_t Distance::lookup(std::size_t i, std::size_t j) {
	return G_DISTANCE_TABLE[i][j];
}

template <std::size_t FROM, std::size_t TO>
constexpr uint8_t Distance::populateElement() {
	constexpr Point fromPosition(Square(FROM).asPoint());
	constexpr Point toPosition(Square(TO).asPoint());

	return static_cast<uint8_t>(fromPosition.maxCartesianDistance(toPosition));
}

template <std::size_t FROM, std::size_t... TO>
constexpr std::array<uint8_t, sizeof...(TO)> Distance::populateRow(std::index_sequence<TO...>) {
	return { populateElement<FROM, TO>()... };
}

template <std::size_t... FROM>
constexpr std::array<std::array<uint8_t, 64>, sizeof...(FROM)> Distance::populateArray(std::index_sequence<FROM...>) {
	return { populateRow<FROM>(std::make_index_sequence<64>{})... };
}

constexpr Distance::Table Distance::populate() {
	return populateArray(std::make_index_sequence<64>{});
}

const Distance::Table Distance::G_DISTANCE_TABLE = Distance::populate();

} //namespace phase4::common

#endif
