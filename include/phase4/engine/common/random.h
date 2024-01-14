#ifndef PHASE4_ENGINE_COMMON_RANDOM_H
#define PHASE4_ENGINE_COMMON_RANDOM_H

#include <random>

namespace phase4::engine::common {

class Random {
public:
	Random();
	Random(uint64_t seed);

	uint64_t next();

private:
	std::random_device m_random_device;
	std::mt19937_64 m_gen;
	std::uniform_int_distribution<uint64_t> m_dis;
};

Random::Random() :
		m_gen(m_random_device()),
		m_dis() {}

Random::Random(uint64_t seed) :
		m_gen(seed),
		m_dis() {}

uint64_t Random::next() {
	return m_dis(m_gen);
}

} //namespace phase4::engine::common

#endif
