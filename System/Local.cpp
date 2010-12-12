#include <mU/System.h>

using namespace mU;

CAPI void VALUE(Local)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1) {
		r = k.local();
		return;
	}
	if (x.size == 2) {
		r = root->clone();
		k.beginLocal(r.symbol());
		k.eval(x[1]);
		k.endLocal();
		return;
	}
}
CAPI void VALUE(BeginLocal)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isSymbol()) {
		k.beginLocal(x[1].symbol());
		r = k.local();
		return;
	}
}
CAPI void VALUE(EndLocal)(Kernel& k, var& r, Tuple& x) {
	if (k.mLocal.size() > 1) {
		k.endLocal();
		r = k.local();
	}
}