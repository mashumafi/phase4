#ifndef PHASE4_ENGINE_COMMON_VECTOR_H
#define PHASE4_ENGINE_COMMON_VECTOR_H

#include <boost/container/static_vector.hpp>

namespace phase4::engine::common {

template <typename T>
using Vector = boost::container::static_vector<T, 512>;

} //namespace phase4::engine::common
#endif
