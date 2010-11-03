#include <mU/String.h>

using namespace mU;

CAPI void VALUE(Join)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1 || !x[1].isObject($.String))
		return;
	wstring s = cast<String>(x[1]).str;
	for (uint i = 2; i < x.size; ++i)
		if (x[i].isObject($.String))
			s += cast<String>(x[i]).str;
		else
			return;
	r = new String(s);
}
CAPI void METHOD(add, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    cast<String>(self).str += cast<String>(x[1]).str;
    r = self;
}
CAPI void METHOD(len, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = new Integer((uint)cast<String>(self).str.length());
}