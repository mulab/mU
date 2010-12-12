#include <mU/System.h>

using namespace mU;

CAPI void VALUE(Clear)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		k.clear(x[1].symbol());
		r = null;
	}
}
CAPI void VALUE(Delayed)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		r = new Tag($.Delayed, x[1]);
}
CAPI void VALUE(Evaluate)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		r = k.eval(x[1]);
}
CAPI void VALUE(Lazy)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		r = k.lazy(x[1]);
}
CAPI void VALUE(Set)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		if (k.assign(x[1], x[2]))
			r = x[2];
	}
}
CAPI void VALUE(SetDelayed)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		if (k.assign(x[1], new Tag($.Delayed, x[2])))
			r = null;
	}
}
CAPI void VALUE(Unevaluated)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		r = x[1];
}
CAPI void VALUE(Unset)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		if (k.assign(x[1], null))
			r = null;
	}
}