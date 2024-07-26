#ifndef PHASE4_ENGINE_COMMON_RANDOM_H
#define PHASE4_ENGINE_COMMON_RANDOM_H

#include <random>

namespace phase4::engine::common {

class Random {
public:
	constexpr Random(uint64_t seed);

	constexpr uint64_t next();

	constexpr uint64_t fewBits();

private:
	static constexpr uint64_t m = (1ULL << 63) - 25; // Large 64-bit prime
	static constexpr uint64_t a = 6364136223846793005ull;
	static constexpr uint64_t c = 1442695040888963407ull;

	uint64_t m_state;
};

constexpr Random::Random(uint64_t seed) :
		m_state(seed) {
}

constexpr uint64_t Random::next() {
	// linear congruential
	m_state = (a * m_state + c) % m;
	// xor shift
	m_state ^= (m_state << 21);
	m_state ^= (m_state >> 35);
	m_state ^= (m_state << 4);
	return m_state;
}

inline constexpr uint64_t Random::fewBits() {
	return next() & next() & next();
}

} //namespace phase4::engine::common

#endif
