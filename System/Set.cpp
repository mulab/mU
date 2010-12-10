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
namespace {
bool Set(Kernel& k, const var& x, const var& y) {
    if (x.isTuple()) {
        Tuple* t = x.tuple().clone();
        t->tuple[0] = k.eval(t->tuple[0]);
		var c = t;
		return set(k, c.tuple(), y);
    }
	return k.assign(x, y);
}
}
CAPI void VALUE(Set)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		if (Set(k, x[1], x[2]))
			r = x[2];
	}
}
CAPI void VALUE(SetDelayed)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		if (Set(k, x[1], new Tag($.Delayed, x[2])))
        r = null;
	}
}
CAPI void VALUE(Unevaluated)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		r = x[1];
}
CAPI void VALUE(Unset)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		if (Set(k, x[1], null))
			r = null;
	}
}
