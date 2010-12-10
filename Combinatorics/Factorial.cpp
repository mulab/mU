#include <mU/Combinatorics.h>
#include "Factorial.Impl.h"

namespace mU {
namespace Combinatorics {
var Factorial(uint x) {
    Integer* r = new Integer();
    facz(r->mpz, x);
    uint e = 0;
    while (x >>= 1) e += x;
    mpz_mul_2exp(r->mpz, r->mpz, e);
    return r;
}
}
}

using namespace mU;

CAPI void VALUE(Factorial)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject($.Integer))
		r = Combinatorics::Factorial(x[1].cast<Integer>().toUI());
}