#include <mU/System.h>

using namespace mU;

CAPI void VALUE(New)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		r = x[1].clone();
		return;
	}
}