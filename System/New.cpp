#include <mU/System.h>

using namespace mU;

CAPI void VALUE(New)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		r = x[1].clone();
		return;
	}
}
CAPI void VALUE(Key)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject($.String)) {
		r = key(wstr(x[1].cast<String>().str));
		return;
	}
}
CAPI void VALUE(Symbol)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject($.String)) {
		r = k.symbol(wstr(x[1].cast<String>().str));
		return;
	}
	if (x.size == 3 && (!x[1] || x[1].isSymbol()) && x[2].isObject($.String)) {
		r = x[1].symbol()->symbol(wstr(x[2].cast<String>().str));
		return;
	}
}