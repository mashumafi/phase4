#ifndef PHASE4_ENGINE_COMMON_VECTOR_H
#define PHASE4_ENGINE_COMMON_VECTOR_H

#ifdef NDEBUG
#include <phase4/engine/common/safe_vector.h>
#else
#include <phase4/engine/common/fast_vector.h>
#endif

namespace phase4::engine::common {

#ifdef NDEBUG
template <typename T, std::size_t SIZE = 1024>
using Vector = SafeVector<T, SIZE>;
#else
template <typename T, std::size_t SIZE = 1024>
using Vector = FastVector<T, SIZE>;
#endif

} //namespace phase4::engine::common
#endif
