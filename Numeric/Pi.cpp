#include <mU/Numeric.h>
#include "NPi.Impl.h"

namespace mU {
void NPi() {
	Real* r = new Real();
	pi_chudnovsky(r->mpf, mpf_get_default_prec());
	return r;
}
}

using namespace mU;

CAPI void VALUE(NPi)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		r = N(k, x[1]);
	if (x.size == 3)
		if (isNumber(x[2])) {
			uint n = static_cast<uint>(toD(x[2].object()));
			r = N(k, x[1], n);
		}
}