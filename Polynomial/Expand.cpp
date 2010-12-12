#include <mU/Number.h>
#include <mU/Polynomial.h>

namespace mU {
var Expand(Kernel& k, const Tuple& x) {
	return null;
}
var Expand(Kernel& k, const var& x, const var& y) {
	return null;
}
}

using namespace mU;

CAPI void CVALUE(System, Expand)(Kernel& k, var& r, Tuple& x) {
	r = Expand(k, x);
}
