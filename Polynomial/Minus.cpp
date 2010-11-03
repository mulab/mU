#include <mU/Number.h>
#include <mU/Polynomial.h>

namespace mU {
var Minus(Kernel& k, const var& x) {
	if (x.isObject())
		return Number::Minus(k, x.object());
	if (x.isTuple() && x.tuple()[0].isSymbol()) {
		sym h = x.tuple()[0].symbol();
		if (h == $.Plus) {
			Tuple* r = tuple(x.tuple().size);
			r->tuple[0] = x.tuple()[0];
			for (uint i = 1; i < x.tuple().size; ++i)
				r->tuple[i] = Minus(k, x.tuple()[i]);
			return r;
		}
		if (h == $.Times) {
			Tuple* r = tuple(x.tuple().size + 1);
			r->tuple[0] = x.tuple()[0];
			r->tuple[1] = new Integer(-1L);
			for (uint i = 1; i < x.tuple().size; ++i)
				r->tuple[i + 1] = x.tuple()[i];
			var t = r;
			return Times(k, t.tuple());
		}
	}
	return tuple($.Times, new Integer(-1L), x);
}
}

using namespace mU;

CAPI void CVALUE(System, Minus)(Kernel& k, var& r, Tuple& x) {
	if (x.size != 2)
		return;
	r = Minus(k, x[1]);
}