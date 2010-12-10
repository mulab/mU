#include <mU/Combinatorics.h>

namespace mU {
namespace Combinatorics {
var Binomial(uint x, uint y) {
    Integer* r = new Integer();
    mpz_bin_uiui(r->mpz, x, y);
    return r;
}
var Binomial(const Integer& x, uint y) {
    Integer* r = new Integer();
    mpz_bin_ui(r->mpz, x.mpz, y);
    return r;
}
var Fibonacci(uint x) {
    Integer* r = new Integer();
    mpz_fib_ui(r->mpz, x);
    return r;
}
var Lucas(uint x) {
    Integer* r = new Integer();
    mpz_lucnum_ui(r->mpz, x);
    return r;
}
}
}

using namespace mU;

CAPI void VALUE(Binomial)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3 && x[1].isObject($.Integer) && x[2].isObject($.Integer))
		r = Combinatorics::Binomial(x[1].cast<Integer>(), x[2].cast<Integer>().toUI());
}
CAPI void VALUE(Fibonacci)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject($.Integer))
		r = Combinatorics::Fibonacci(x[1].cast<Integer>().toUI());
}
CAPI void VALUE(Lucas)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject($.Integer))
		r = Combinatorics::Lucas(x[1].cast<Integer>().toUI());
}