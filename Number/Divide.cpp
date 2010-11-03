#include <mU/Number.h>

namespace mU {
namespace Number {
var Divide(Kernel& k, const Object& x) {
	if (x.type == $.Integer) {
		sint sgn = mpz_sgn(static_cast<const Integer&>(x).mpz);
		if (sgn == 0)
			return $.Infinity;
		var a = new Integer(sgn), b = &x;
		if (sgn < 0) {
			b = new Integer();
			mpz_neg(cast<Integer>(b).mpz, static_cast<const Integer&>(x).mpz);
		}
		Rational* r = new Rational();
		mpq_set_num(r->mpq, cast<Integer>(a).mpz);
		mpq_set_den(r->mpq, cast<Integer>(b).mpz);
		return r;
	}
	if (x.type == $.Rational) {
		Rational* r = new Rational();
		mpq_inv(r->mpq, static_cast<const Rational&>(x).mpq);
		return r;
	}
	if (x.type == $.Real) {
		if (!mpf_sgn(static_cast<const Real&>(x).mpf))
			return $.Infinity;
		Real* r = new Real();
		mpf_ui_div(r->mpf, 1L, static_cast<const Real&>(x).mpf);
		return r;
	}
	return tuple($.Power, &x, new Integer(-1L));	
}
var Divide(Kernel& k, const Object& x, const Object& y) {
	if (y.type == $.Integer) {
		if (!mpz_sgn(static_cast<const Integer&>(y).mpz))
			return $.Infinity;
		if (x.type == $.Integer) {
			Rational* r = new Rational();
			mpq_set_num(r->mpq, static_cast<const Integer&>(x).mpz);
			mpq_set_den(r->mpq, static_cast<const Integer&>(y).mpz);
			mpq_canonicalize(r->mpq);
			Integer* t = new Integer();
			mpq_get_den(t->mpz, r->mpq);
			if (!mpz_cmp_ui(t->mpz, 1)) {
				mpq_get_num(t->mpz, r->mpq);
				return t;
			}
			return r;
		}
		if (x.type == $.Rational) {
			Rational* r = new Rational();
			mpq_set_z(r->mpq, static_cast<const Integer&>(y).mpz);
			mpq_div(r->mpq, static_cast<const Rational&>(x).mpq, r->mpq);
			return r;
		}
		if (x.type == $.Real) {
			Real* r = new Real();
			mpf_set_z(r->mpf, static_cast<const Integer&>(y).mpz);
			mpf_div(r->mpf, static_cast<const Real&>(x).mpf, r->mpf);
			return r;
		}
	}
	if (y.type == $.Rational) {
		if (x.type == $.Integer) {
			Rational* r = new Rational();
			mpq_set_z(r->mpq, static_cast<const Integer&>(x).mpz);
			mpq_div(r->mpq, r->mpq, static_cast<const Rational&>(y).mpq);
			return r;
		}
		if (x.type == $.Rational) {
			Rational* r = new Rational();
			mpq_div(r->mpq, static_cast<const Rational&>(x).mpq, 
				static_cast<const Rational&>(y).mpq);
			return r;
		}
		if (x.type == $.Real) {
			Real* r = new Real();
			mpf_set_q(r->mpf, static_cast<const Rational&>(y).mpq);
			mpf_div(r->mpf, static_cast<const Real&>(x).mpf, r->mpf);
			return r;
		}
	}
	if (y.type == $.Real) {
		if (!mpf_sgn(static_cast<const Real&>(y).mpf))
			return $.Infinity;
		if (x.type == $.Integer) {
			Real* r = new Real();
			mpf_set_z(r->mpf, static_cast<const Integer&>(x).mpz);
			mpf_div(r->mpf, r->mpf, static_cast<const Real&>(y).mpf);
			return r;
		}
		if (x.type == $.Rational) {
			Real* r = new Real();
			mpf_set_q(r->mpf, static_cast<const Rational&>(x).mpq);
			mpf_div(r->mpf, r->mpf, static_cast<const Real&>(y).mpf);
			return r;
		}
		if (x.type == $.Real) {
			Real* r = new Real();
			mpf_div(r->mpf, static_cast<const Real&>(x).mpf, 
				static_cast<const Real&>(y).mpf);
			return r;
		}
	}
	if (x.compare(y) == 0)
		return new Integer(1L);
	return tuple($.Times, &x, Divide(k, y));
}
}
}

using namespace mU;

CAPI void VALUE(Divide)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		if (isNumber(x[1]))
			r = Number::Divide(k, x[1].object());
	if (x.size == 3)
		if (isNumber(x[1]) && isNumber(x[2]))
			r = Number::Divide(k, x[1].object(), x[2].object());
}
CAPI void CMETHOD(System_Integer, div, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpz_div(cast<Integer>(self).mpz, cast<Integer>(self).mpz, cast<Integer>(x[1]).mpz);
	r = self;
}
CAPI void CMETHOD(System_Rational, div, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpq_div(cast<Rational>(self).mpq, cast<Rational>(self).mpq, cast<Rational>(x[1]).mpq);
	r = self;
}
CAPI void CMETHOD(System_Real, div, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpf_div(cast<Real>(self).mpf, cast<Real>(self).mpf, cast<Real>(x[1]).mpf);
	r = self;
}