#include <mU/Number.h>

namespace mU {
namespace Number {
void div(var& r, const Object& x) {
	var t = r;
	if (x.type == $.Integer) {
		sint sgn = mpz_sgn(x.cast<Integer>().mpz);
		if (sgn == 0) {
			r = $.Infinity;
			return;
		}
		var a = new Integer(sgn), b = &x;
		if (sgn < 0) {
			b = new Integer();
			mpz_neg(b.cast<Integer>().mpz, x.cast<Integer>().mpz);
		}
		if (!r.isObject($.Rational))
			r = new Rational();
		mpq_set_num(r.cast<Rational>().mpq, a.cast<Integer>().mpz);
		mpq_set_den(r.cast<Rational>().mpq, b.cast<Integer>().mpz);
		return;
	}
	if (x.type == $.Rational) {
		if (!r.isObject($.Rational))
			r = new Rational();
		mpq_inv(r.cast<Rational>().mpq, x.cast<Rational>().mpq);
		return;
	}
	if (x.type == $.Real) {
		if (!mpf_sgn(x.cast<Real>().mpf)) {
			r = $.Infinity;
			return;
		}
		if (r.isObject($.Real))
			mpf_set_prec(r.cast<Real>().mpf, mpf_get_prec(x.cast<Real>().mpf));
		else
			r = new Real(x.cast<Real>().prec());
		mpf_ui_div(r.cast<Real>().mpf, 1L, x.cast<Real>().mpf);
		return;
	}
	r = tuple($.Power, &x, new Integer(-1L));	
}
void div(var& r, const Object& x, const Object& y) {
	var t = r;
	if (y.type == $.Integer) {
		if (!mpz_sgn(y.cast<Integer>().mpz)) {
			r = $.Infinity;
			return;
		}
		if (x.type == $.Integer) {
			if (!r.isObject($.Rational))
				r = new Rational();
			mpq_set_num(r.cast<Rational>().mpq, x.cast<Integer>().mpz);
			mpq_set_den(r.cast<Rational>().mpq, y.cast<Integer>().mpz);
			mpq_canonicalize(r.cast<Rational>().mpq);
			Integer* t = new Integer();
			mpq_get_den(t->mpz, r.cast<Rational>().mpq);
			if (!mpz_cmp_ui(t->mpz, 1)) {
				mpq_get_num(t->mpz, r.cast<Rational>().mpq);
				r = t;
			}
			return;
		}
		if (x.type == $.Rational) {
			if (!r.isObject($.Rational))
				r = new Rational();
			mpq_set_z(r.cast<Rational>().mpq, y.cast<Integer>().mpz);
			mpq_div(r.cast<Rational>().mpq, x.cast<Rational>().mpq, r.cast<Rational>().mpq);
			return;
		}
		if (x.type == $.Real) {
			Real* r = new Real(x.cast<Real>().prec());
			mpf_set_z(r->mpf, y.cast<Integer>().mpz);
			mpf_div(r->mpf, x.cast<Real>().mpf, r->mpf);
			return;
		}
	}
	if (y.type == $.Rational) {
		if (x.type == $.Integer) {
			if (!r.isObject($.Rational))
				r = new Rational();
			mpq_set_z(r.cast<Rational>().mpq, x.cast<Integer>().mpz);
			mpq_div(r.cast<Rational>().mpq, r.cast<Rational>().mpq, y.cast<Rational>().mpq);
			return;
		}
		if (x.type == $.Rational) {
			if (!r.isObject($.Rational))
				r = new Rational();
			mpq_div(r.cast<Rational>().mpq, x.cast<Rational>().mpq, 
				y.cast<Rational>().mpq);
			return;
		}
		if (x.type == $.Real) {
			if (r.isObject($.Real))
				mpf_set_prec(r.cast<Real>().mpf, mpf_get_prec(x.cast<Real>().mpf));
			else
				r = new Real(x.cast<Real>().prec());
			mpf_set_q(r.cast<Real>().mpf, y.cast<Rational>().mpq);
			mpf_div(r.cast<Real>().mpf, x.cast<Real>().mpf, r.cast<Real>().mpf);
			return;
		}
	}
	if (y.type == $.Real) {
		if (!mpf_sgn(y.cast<Real>().mpf)) {
			r = $.Infinity;
			return;
		}
		if (x.type == $.Integer) {
			if (r.isObject($.Real))
				mpf_set_prec(r.cast<Real>().mpf, mpf_get_prec(y.cast<Real>().mpf));
			else
				r = new Real(y.cast<Real>().prec());
			mpf_set_z(r.cast<Real>().mpf, x.cast<Integer>().mpz);
			mpf_div(r.cast<Real>().mpf, r.cast<Real>().mpf, y.cast<Real>().mpf);
			return;
		}
		if (x.type == $.Rational) {
			if (r.isObject($.Real))
				mpf_set_prec(r.cast<Real>().mpf, mpf_get_prec(y.cast<Real>().mpf));
			else
				r = new Real(y.cast<Real>().prec());
			mpf_set_q(r.cast<Real>().mpf, x.cast<Rational>().mpq);
			mpf_div(r.cast<Real>().mpf, r.cast<Real>().mpf, y.cast<Real>().mpf);
			return;
		}
		if (x.type == $.Real) {
			if (r.isObject($.Real))
				mpf_set_prec(r.cast<Real>().mpf, std::min(
				mpf_get_prec(x.cast<Real>().mpf), 
				mpf_get_prec(y.cast<Real>().mpf)));
			else
				r = new Real(std::min(x.cast<Real>().prec(), 
				y.cast<Real>().prec()));
			mpf_div(r.cast<Real>().mpf, x.cast<Real>().mpf, 
				y.cast<Real>().mpf);
			return;
		}
	}
	if (x.compare(y) == 0)
		r = new Integer(1L);
	else {
		div(r, y);
		r = tuple($.Times, &x, r);
	}
}
}
}

using namespace mU;

CAPI void VALUE(Divide)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		if (isNumber(x[1])) {
			Number::div(r, x[1].object());
			return;
		}
	if (x.size == 3)
		if (isNumber(x[1]) && isNumber(x[2])) {
			Number::div(r, x[1].object(), x[2].object());
			return;
		}
}
CAPI void CMETHOD(System_Integer, div, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpz_div(self.cast<Integer>().mpz, self.cast<Integer>().mpz, x[1].cast<Integer>().mpz);
	r = self;
}
CAPI void CMETHOD(System_Rational, div, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpq_inv(self.cast<Rational>().mpq, self.cast<Rational>().mpq);
	r = self;
}
CAPI void CMETHOD(System_Rational, div, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpq_div(self.cast<Rational>().mpq, self.cast<Rational>().mpq, x[1].cast<Rational>().mpq);
	r = self;
}
CAPI void CMETHOD(System_Real, div, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpf_ui_div(self.cast<Real>().mpf, 1, self.cast<Real>().mpf);
	r = self;
}
CAPI void CMETHOD(System_Real, div, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	mpf_div(self.cast<Real>().mpf, self.cast<Real>().mpf, x[1].cast<Real>().mpf);
	r = self;
}