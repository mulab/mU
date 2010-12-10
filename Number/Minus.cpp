#include <mU/Number.h>

namespace mU {
namespace Number {
void neg(var& r, const Object& x) {
	if (x.type == $.Integer) {
		if (!r.isObject($.Integer))
			r = new Integer();
		mpz_neg(r.cast<Integer>().mpz, x.cast<Integer>().mpz);
		return;
	}
	if (x.type == $.Rational) {
		if (!r.isObject($.Rational))
			r = new Rational();
		mpq_neg(r.cast<Rational>().mpq, x.cast<Rational>().mpq);
		return;
	}
	if (x.type == $.Real) {
		if (!r.isObject($.Real))
			r = new Real(x.cast<Real>().prec());
		mpf_neg(r.cast<Real>().mpf, x.cast<Real>().mpf);
		return;
	}
	r = tuple($.Times, new Integer(-1L), &x);
}
}
}

using namespace mU;

CAPI void VALUE(Minus)(Kernel& k, var& r, Tuple& x) {
	if (x.size != 2 || !isNumber(x[1]))
		return;
	Number::neg(r, x[1].object());
}
CAPI void CMETHOD(System_Integer, neg, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpz_neg(self.cast<Integer>().mpz, self.cast<Integer>().mpz);
	r = self;
}
CAPI void CMETHOD(System_Rational, neg, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpq_neg(self.cast<Rational>().mpq, self.cast<Rational>().mpq);
	r = self;
}
CAPI void CMETHOD(System_Real, neg, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpf_neg(self.cast<Real>().mpf, self.cast<Real>().mpf);
	r = self;
}