#include <mU/System.h>

using namespace mU;

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
CAPI void VALUE(FreeQ)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(!x[1].depend(x[2]));
}