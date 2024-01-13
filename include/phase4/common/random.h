#ifndef PHASE4_COMMON_RANDOM_H
#define PHASE4_COMMON_RANDOM_H

#include <random>

namespace phase4::common {

class Random {
public:
	Random();

	uint64_t next();

private:
	std::random_device m_random_device;
	std::mt19937_64 m_gen;
	std::uniform_int_distribution<uint64_t> m_dis;
}

Random::Random() :
		m_gen(m_random_device) {
}

uint64_t Random::next() {
	return m_dis(m_gen);
}

} //namespace phase4::common

#endif
