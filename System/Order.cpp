#include <mU/System.h>

namespace mU {
int Order(Kernel& k, const var& x, const var& y) {
	return x - y;
}
}
using namespace mU;

CAPI void VALUE(Order)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = new Integer((sint)Order(k, x[1], x[2]));
}
CAPI void VALUE(Less)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(Order(k, x[1], x[2]) < 0);
}
CAPI void VALUE(Equal)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(Order(k, x[1], x[2]) == 0);
}
CAPI void VALUE(Greater)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(Order(k, x[1], x[2]) > 0);
}
CAPI void VALUE(LessEqual)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(Order(k, x[1], x[2]) <= 0);
}
CAPI void VALUE(Unequal)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(Order(k, x[1], x[2]) != 0);
}
CAPI void VALUE(GreaterEqual)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(Order(k, x[1], x[2]) >= 0);
}
CAPI void VALUE(SameQ)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(x[1] == x[2]);
}
CAPI void VALUE(UnsameQ)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3)
		r = boolean(x[1] != x[2]);
}