#include <mU/Number.h>
#include <mU/Polynomial.h>

namespace mU {
var Divide(Kernel& k, const var& x) {
	if (x.isObject())
		return Number::Divide(k, x.object());
	if (x.isTuple() && x.tuple()[0].isSymbol()) {
		sym h = x.tuple()[0].symbol();
		if (h == $.Times) {
			Tuple* r = tuple(x.tuple().size);
			r->tuple[0] = x.tuple()[0];
			for (uint i = 1; i < x.tuple().size; ++i)
				r->tuple[i] = Divide(k, x.tuple()[i]);
			return r;
		}
		if (h == $.Power) {
			Tuple* r = tuple(3);
			r->tuple[0] = x.tuple()[0];
			r->tuple[1] = x.tuple()[1];
			r->tuple[2] = Minus(k, x.tuple()[2]);
			return r;
		}
	}
	if (x == $.Infinity)
		return new Integer(0L);
	return tuple($.Power, x, new Integer(-1L));
}
var Divide(Kernel& k, const var& x, const var& y) {
	if (x.isObject() && y.isObject())
		return Number::Divide(k, x.object(), y.object());
	return Times(k, x, Divide(k, y));
}
}

using namespace mU;

CAPI void CVALUE(System, Divide)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		r = Divide(k, x[1]);
	if (x.size == 3)
		r = Divide(k, x[1], x[2]);
}