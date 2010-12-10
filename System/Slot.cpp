#include <mU/System.h>

using namespace mU;

CAPI void VALUE(Kind)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isKey()) {
		r = kind(x[1].key());
		return;
	}
}
CAPI void VALUE(Slot)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		if (x[2].isKey())
			r = k.slot(x[1], x[2].key());
		else if (x[2].isTuple())
			r = k.slot(x[1], x[2].tuple());
	}
}
CAPI bool ASSIGN(Slot)(Kernel& k, const Tuple& x, const var& y) {
    if (!x[0].isSymbol() || x.size != 3)
        return false;
    if (x[2].isKey())
        return k.set(k.eval(x[1]), x[2].key(), y);
    if (x[2].isTuple())
        return k.set(k.eval(x[1]), x[2].tuple(), y);
    return false;
}