#ifndef PHASE4_ENGINE_COMMON_RANDOM_H
#define PHASE4_ENGINE_COMMON_RANDOM_H

#include <random>

namespace phase4::engine::common {

class Random {
public:
	constexpr Random(uint64_t seed);

	constexpr uint64_t next();

private:
	uint64_t m_state;
};

constexpr Random::Random(uint64_t seed) :
		m_state(seed) {
}

constexpr uint64_t Random::next() {
	m_state = m_state * 6364136223846793005ULL + 1;
	return m_state;
}

} //namespace phase4::engine::common

#endif
