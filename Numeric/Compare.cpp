#include <mU/Polynomial.h>
#include <mU/Numeric.h>
#ifdef _MSC_VER
#pragma comment(lib,"Polynomial.lib")
#endif

namespace mU {
int Compare(Kernel& k, const var& x, const var& y) {
	var r = N(k, Plus(k, x, Minus(k, y)));
	if (r.isObject($.Real))
		return mpf_sgn(cast<Real>(r).mpf);
	if (r == $.Infinity)
		return 1;
	if (r.isTuple($.Times) && r.tuple().size == 3 && r.tuple()[2] == $.Infinity)
		return -1;
	return -2;
}
}

using namespace mU;

CAPI void CVALUE(System, Compare)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		sint c = Compare(k, x[1], x[2]);
		if (c == -2)
			r = $.Fail;
		else
			r = new Integer(c);
	}
}
CAPI void CVALUE(System, Less)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		sint c = Compare(k, x[1], x[2]);
		if (c != -2)
			r = boolean(c < 0);
	}
}
CAPI void CVALUE(System, Equal)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		sint c = Compare(k, x[1], x[2]);
		if (c != -2)
			r = boolean(c == 0);
	}
}
CAPI void CVALUE(System, Greater)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		sint c = Compare(k, x[1], x[2]);
		if (c != -2)
			r = boolean(c > 0);
	}
}
CAPI void CVALUE(System, LessEqual)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		sint c = Compare(k, x[1], x[2]);
		if (c != -2)
			r = boolean(c <= 0);
	}
}
CAPI void CVALUE(System, Unequal)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		sint c = Compare(k, x[1], x[2]);
		if (c != -2)
			r = boolean(c != 0);
	}
}
CAPI void CVALUE(System, GreaterEqual)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		sint c = Compare(k, x[1], x[2]);
		if (c != -2)
			r = boolean(c >= 0);
	}
}