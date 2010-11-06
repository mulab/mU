#include <mU/Number.h>

namespace mU {
namespace Number {
var Cast(Kernel& k, const var& x, sym type) {
	if (x.isObject()) {
		if (x.object().type == $.Integer) {
			if (type == $.Rational) {
				Rational* r = new Rational();
				mpq_set_z(r->mpq, cast<Integer>(x).mpz);
				return r;
			}
			if (type == $.Real) {
				Real* r = new Real();
				mpf_set_z(r->mpf, cast<Integer>(x).mpz);
				return r;
			}
		}
		if (x.object().type == $.Rational) {
			if (type == $.Integer) {
				Integer* r = new Integer();
				mpz_set_q(r->mpz, cast<Rational>(x).mpq);
				return r;
			}
			if (type == $.Real) {
				Real* r = new Real();
				mpf_set_q(r->mpf, cast<Rational>(x).mpq);
				return r;
			}
		}
		if (x.object().type == $.Real) {
			if (type == $.Integer) {
				Integer* r = new Integer();
				mpz_set_f(r->mpz, cast<Real>(x).mpf);
				return r;
			}
			if (type == $.Rational) {
				Rational* r = new Rational();
				mpq_set_f(r->mpq, cast<Real>(x).mpf);
				return r;
			}
		}
	}
	return x;
}
}
}

using namespace mU;

CAPI void VALUE(Cast)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3 && x[2].isSymbol())
		r = Number::Cast(k, x[1], x[2].symbol());
}