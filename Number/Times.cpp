#include <mU/Number.h>

namespace mU {
namespace Number {
void mul(var& r, const Object& x, const Object& y) {
	var t = r;
	if (x.type == $.Integer) {
		if (y.type == $.Integer) {
			if (!r.isObject($.Integer))
				r = new Integer();
			mpz_mul(r.cast<Integer>().mpz, x.cast<Integer>().mpz, 
				y.cast<Integer>().mpz);
			return;
		}
		if (y.type == $.Rational) {
			if (!r.isObject($.Rational))
				r = new Rational();
			mpq_set_z(r.cast<Rational>().mpq, x.cast<Integer>().mpz);
			mpq_mul(r.cast<Rational>().mpq, r.cast<Rational>().mpq, y.cast<Rational>().mpq);
			return;
		}
		if (y.type == $.Real) {
			if (r.isObject($.Real))
				mpf_set_prec(r.cast<Real>().mpf, mpf_get_prec(y.cast<Real>().mpf));
			else
				r = new Real(y.cast<Real>().prec());
			mpf_set_z(r.cast<Real>().mpf, x.cast<Integer>().mpz);
			mpf_mul(r.cast<Real>().mpf, r.cast<Real>().mpf, y.cast<Real>().mpf);
			return;
		}
	}
	if (x.type == $.Rational) {
		if (y.type == $.Integer) {
			if (!r.isObject($.Rational))
				r = new Rational();
			mpq_set_z(r.cast<Rational>().mpq, y.cast<Integer>().mpz);
			mpq_mul(r.cast<Rational>().mpq, r.cast<Rational>().mpq, x.cast<Rational>().mpq);
			return;
		}
		if (y.type == $.Rational) {
			if (!r.isObject($.Rational))
				r = new Rational();
			mpq_mul(r.cast<Rational>().mpq, x.cast<Rational>().mpq, 
				y.cast<Rational>().mpq);
			return;
		}
		if (y.type == $.Real) {
			if (r.isObject($.Real))
				mpf_set_prec(r.cast<Real>().mpf, mpf_get_prec(y.cast<Real>().mpf));
			else
				r = new Real(y.cast<Real>().prec());
			mpf_set_q(r.cast<Real>().mpf, x.cast<Rational>().mpq);
			mpf_mul(r.cast<Real>().mpf, r.cast<Real>().mpf, y.cast<Real>().mpf);
			return;
		}
	}
	if (x.type == $.Real) {
		if (y.type == $.Integer) {
			if (r.isObject($.Real))
				mpf_set_prec(r.cast<Real>().mpf, mpf_get_prec(x.cast<Real>().mpf));
			else
				r = new Real(x.cast<Real>().prec());
			mpf_set_z(r.cast<Real>().mpf, y.cast<Integer>().mpz);
			mpf_mul(r.cast<Real>().mpf, r.cast<Real>().mpf, x.cast<Real>().mpf);
			return;
		}
		if (y.type == $.Rational) {
			if (r.isObject($.Real))
				mpf_set_prec(r.cast<Real>().mpf, mpf_get_prec(x.cast<Real>().mpf));
			else
				r = new Real(x.cast<Real>().prec());
			mpf_set_q(r.cast<Real>().mpf, y.cast<Rational>().mpq);
			mpf_mul(r.cast<Real>().mpf, r.cast<Real>().mpf, x.cast<Real>().mpf);
			return;
		}
		if (y.type == $.Real) {
			if (r.isObject($.Real))
				mpf_set_prec(r.cast<Real>().mpf, std::min(
				mpf_get_prec(x.cast<Real>().mpf), 
				mpf_get_prec(y.cast<Real>().mpf)));
			else
				r = new Real(std::min(x.cast<Real>().prec(), 
				y.cast<Real>().prec()));
			mpf_mul(r.cast<Real>().mpf, x.cast<Real>().mpf, 
				y.cast<Real>().mpf);
			return;
		}
	}
	if (x.compare(y) == 0)
		r = tuple($.Power, &x, new Integer(2L));
	else
		r = tuple($.Times, &x, &y);
}
}
}

using namespace mU;

CAPI void VALUE(Times)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1)
		return;
	for (uint i = 1; i < x.size; ++i)
		if (isNumber(x[i]))
			Number::mul(r, r.object(), x[i].object());
}
CAPI void CMETHOD(System_Integer, mul, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpz_mul(self.cast<Integer>().mpz, self.cast<Integer>().mpz, x[1].cast<Integer>().mpz);
	r = self;
}
CAPI void CMETHOD(System_Rational, mul, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpq_mul(self.cast<Rational>().mpq, self.cast<Rational>().mpq, x[1].cast<Rational>().mpq);
	r = self;
}
CAPI void CMETHOD(System_Real, mul, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpf_mul(self.cast<Real>().mpf, self.cast<Real>().mpf, x[1].cast<Real>().mpf);
	r = self;
}