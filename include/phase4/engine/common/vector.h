#ifdef NDEBUG
#include <phase4/engine/common/safe_vector.h>
#else
#include <phase4/engine/common/fast_vector.h>
#endif

namespace phase4::engine::common {

#ifdef NDEBUG
using Vector = SafeVector;
#else
using Vector = FastVector;
#endif

} //namespace phase4::engine::common