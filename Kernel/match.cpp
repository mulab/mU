#include <mU/Kernel.h>

namespace mU {
bool Kernel::match(var& r, const var& x, Match& y) {
	mBind.clear();
	beginLocal();
	bool flag = y(*this, r, x);
	endLocal();
	return flag;
}
var Kernel::replace(const var& x, Match& y) {
	var r;
	bool flag = match(r, x, y);
	if (flag)
		return r;
	if (x.isTuple()) {
		Tuple* t = x.tuple().clone();
		for (uint i = 0; i < t->size; ++i)
			t->tuple[i] = replace(t->tuple[i], y);
		return t;
	}
	return x;
}
}