#include <mU/System.h>

namespace mU {
bool FreeQ(const var& x, const var& y) {
	if (x.equal(y))
		return false;
	if (x.isTuple()) {
		for (uint i = 0; i < x.tuple().size; ++i)
			if (!FreeQ(x.tuple()[i], y))
				return false;
	}
	return true;
}
}

using namespace mU;

CAPI void VALUE(FreeQ)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(FreeQ(x[1], x[2]));
}
CAPI void VALUE(Hash)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		r = new Integer((uint)x[1].hash());
}
CAPI void VALUE(Order)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		int c = x[1].compare(x[2]);
		r = new Integer(c < 0 ? -1L : c > 0);
	}
}
CAPI void VALUE(SameQ)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(x[1] == x[2]);
}
CAPI void VALUE(UnsameQ)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(x[1] != x[2]);
}