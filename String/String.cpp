#include <mU/String.h>

using namespace mU;

CAPI void CVALUE(System, StringJoin)(Kernel& k, var& r, Tuple& x) {
	wstring s;
	for (uint i = 1; i < x.size; ++i)
		if (x[i].isObject($.String))
			s += cast<String>(x[i]).str;
		else
			return;
	r = new String(s);
}
CAPI void METHOD(length, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    r = new Integer((uint)cast<String>(self).str.length());
}
CAPI void METHOD(join, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    cast<String>(self).str += cast<String>(x[1]).str;
    r = self;
}