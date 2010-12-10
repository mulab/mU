#include <mU/String.h>

using namespace mU;

CAPI void VALUE(Join)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1 || !x[1].isObject($.String))
		return;
	uint size = x[1].cast<String>().str.size();
	for (uint i = 2; i < x.size; ++i)
		if (x[i].isObject($.String))
			size += x[i].cast<String>().str.size();
	wstring s;
	s.reserve(size);
	s += x[1].cast<String>().str;
	for (uint i = 2; i < x.size; ++i)
		if (x[i].isObject($.String))
			s += x[i].cast<String>().str;
	r = new String(s);
}
CAPI void METHOD(add, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    self.cast<String>().str += x[1].cast<String>().str;
    r = self;
}
CAPI void METHOD(len, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = new Integer((uint)self.cast<String>().str.length());
}