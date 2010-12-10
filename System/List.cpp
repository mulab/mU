#include <mU/System.h>

using namespace mU;

CAPI void VALUE(Apply)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3 && x[2].isTuple()) {
		Tuple* t = x[2].tuple().clone();
		t->tuple[0] = x[1];
		r = k.rewrite(t);
		r = k.value(r.tuple());
	}
}
CAPI bool ASSIGN(List)(Kernel& k, const Tuple& x, const var& y) {
    if (!x[0].isSymbol())
        return false;
    bool flag = true;
    if (y.isTuple($.List)) {
        for (uint i = 1; i < x.size && flag; ++i)
            flag = k.assign(x[i], y.tuple()[std::min(i, y.tuple().size - 1)]);
    } else {
        for (uint i = 1; i < x.size && flag; ++i)
            flag = k.assign(x[i], y);
    }
    return flag;
}
CAPI void VALUE(Map)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3 && x[2].isTuple()) {
		Tuple* t = tuple(x[2].tuple().size);
		t->tuple[0] = $.List;
		for (uint i = 1; i < t->size; ++i) {
			Tuple* c = tuple(2);
			c->tuple[0] = x[1];
			c->tuple[1] = x[2].tuple()[i];
			t->tuple[i] = k.rewrite(c);
			t->tuple[i] = k.value(t->tuple[i].tuple());
		}
		r = t;
	}
}
CAPI void VALUE(Postfix)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		Tuple* t = tuple(x[2], x[1]);
		r = k.rewrite(t);
		r = k.value(r.tuple());
	}
}
CAPI void VALUE(Prefix)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		Tuple* t = tuple(x[1], x[2]);
		r = k.rewrite(t);
		r = k.value(r.tuple());
	}
}