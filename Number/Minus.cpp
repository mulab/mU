#include <mU/Number.h>

namespace mU {
namespace Number {
var Minus(Kernel& k, const Object& x) {
	if (x.type == $.Integer) {
		Integer* r = new Integer();
		mpz_neg(r->mpz, static_cast<const Integer&>(x).mpz);
		return r;
	}
	if (x.type == $.Rational) {
		Rational* r = new Rational();
		mpq_neg(r->mpq, static_cast<const Rational&>(x).mpq);
		return r;
	}
	if (x.type == $.Real) {
		Real* r = new Real(static_cast<const Real&>(x).prec());
		mpf_neg(r->mpf, static_cast<const Real&>(x).mpf);
		return r;
	}
	return tuple($.Times, new Integer(-1L), &x);
}
}
}

using namespace mU;

CAPI void VALUE(Minus)(Kernel& k, var& r, Tuple& x) {
	if (x.size != 2 || !isNumber(x[1]))
		return;
	r = Number::Minus(k, x[1].object());
}
CAPI void CMETHOD(System_Integer, neg, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpz_neg(cast<Integer>(self).mpz, cast<Integer>(self).mpz);
	r = self;
}
CAPI void CMETHOD(System_Rational, neg, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpq_neg(cast<Rational>(self).mpq, cast<Rational>(self).mpq);
	r = self;
}
CAPI void CMETHOD(System_Real, neg, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpf_neg(cast<Real>(self).mpf, cast<Real>(self).mpf);
	r = self;
}