#include <mU/System.h>

using namespace mU;

CAPI void VALUE(Function)(Kernel& k, var& r, Tuple& x) {
	if (x.size > 1 && x[0].isTuple($.Function)) {
		const Tuple& t = x[0].tuple();
		if (t.size == 2) {
			k.beginSelf(&x);
			r = k.eval(t[1]);
			k.endSelf();
			return;
		}
		if (t.size == 3) {
			k.beginLocal();
			if (t[1].isTuple($.List)) {
				uint size = std::min(t[1].tuple().size, x.size);
				for (uint i = 1; i < size; ++i)
					k.assign(t[1].tuple()[i], x[i]);
			} else
				k.assign(t[1], x[1]);
			r = k.eval(t[2]);
			k.endLocal();
			return;
		}
	}
}