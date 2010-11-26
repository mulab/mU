#include <mU/Number.h>

namespace mU {
namespace Number {
var Plus(Kernel& k, const Object& x, const Object& y) {
	if (x.type == $.Integer) {
		if (y.type == $.Integer) {
			Integer* r = new Integer();
			mpz_add(r->mpz, static_cast<const Integer&>(x).mpz, 
				static_cast<const Integer&>(y).mpz);
			return r;
		}
		if (y.type == $.Rational) {
			Rational* r = new Rational();
			mpq_set_z(r->mpq, static_cast<const Integer&>(x).mpz);
			mpq_add(r->mpq, r->mpq, static_cast<const Rational&>(y).mpq);
			return r;
		}
		if (y.type == $.Real) {
			Real* r = new Real(static_cast<const Real&>(y).prec());
			mpf_set_z(r->mpf, static_cast<const Integer&>(x).mpz);
			mpf_add(r->mpf, r->mpf, static_cast<const Real&>(y).mpf);
			return r;
		}
	}
	if (x.type == $.Rational) {
		if (y.type == $.Integer) {
			Rational* r = new Rational();
			mpq_set_z(r->mpq, static_cast<const Integer&>(y).mpz);
			mpq_add(r->mpq, r->mpq, static_cast<const Rational&>(x).mpq);
			return r;
		}
		if (y.type == $.Rational) {
			Rational* r = new Rational();
			mpq_add(r->mpq, static_cast<const Rational&>(x).mpq, 
				static_cast<const Rational&>(y).mpq);
			return r;
		}
		if (y.type == $.Real) {
			Real* r = new Real(static_cast<const Real&>(y).prec());
			mpf_set_q(r->mpf, static_cast<const Rational&>(x).mpq);
			mpf_add(r->mpf, r->mpf, static_cast<const Real&>(y).mpf);
			return r;
		}
	}
	if (x.type == $.Real) {
		if (y.type == $.Integer) {
			Real* r = new Real(static_cast<const Real&>(x).prec());
			mpf_set_z(r->mpf, static_cast<const Integer&>(y).mpz);
			mpf_add(r->mpf, r->mpf, static_cast<const Real&>(x).mpf);
			return r;
		}
		if (y.type == $.Rational) {
			Real* r = new Real(static_cast<const Real&>(x).prec());
			mpf_set_q(r->mpf, static_cast<const Rational&>(y).mpq);
			mpf_add(r->mpf, r->mpf, static_cast<const Real&>(x).mpf);
			return r;
		}
		if (y.type == $.Real) {
			Real* r = new Real(std::min(static_cast<const Real&>(x).prec(), 
				static_cast<const Real&>(y).prec()));
			mpf_add(r->mpf, static_cast<const Real&>(x).mpf, 
				static_cast<const Real&>(y).mpf);
			return r;
		}
	}
	if (x.compare(y) == 0)
		return tuple($.Times, new Integer(2L), &x);
	return tuple($.Plus, &x, &y);
}
}
}

using namespace mU;

CAPI void VALUE(Plus)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1 || !isNumber(x[1]))
		return;
	var c = x[1];
	for (uint i = 2; i < x.size; ++i)
		if (isNumber(x[i]))
			c = Number::Plus(k, c.object(), x[i].object());
	r = c;
}
CAPI void CMETHOD(System_Integer, add, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpz_add(cast<Integer>(self).mpz, cast<Integer>(self).mpz, cast<Integer>(x[1]).mpz);
	r = self;
}
CAPI void CMETHOD(System_Rational, add, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpq_add(cast<Rational>(self).mpq, cast<Rational>(self).mpq, cast<Rational>(x[1]).mpq);
	r = self;
}
CAPI void CMETHOD(System_Real, add, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpf_add(cast<Real>(self).mpf, cast<Real>(self).mpf, cast<Real>(x[1]).mpf);
	r = self;
}