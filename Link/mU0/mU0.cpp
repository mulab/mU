#include "mU0Object.h"

namespace mU {
mU0::var to_mu0(sym x) {
	return mU0::Sym2(x->toS().c_str());
}
mU0::var to_mu0(const Key& x) {
	return mU0::Sym(x.toS());
}
mU0::var to_mu0(const Object& x) {
	if (x.type == $.Integer) {
		mU0::var r = mU0::Int();
		mpz_set(mU0::CInt(r), x.cast<Integer>().mpz);
		return r;
	}
	if (x.type == $.Rational) {
		mU0::var r = mU0::Rat();
		mpq_set(mU0::CRat(r), x.cast<Rational>().mpq);
		return r;
	}
	if (x.type == $.Real) {
		mU0::var r = mU0::Flt();
		mpf_set(mU0::CFlt(r), x.cast<Real>().mpf);
		return r;
	}
	if (x.type == $.String) {
		mU0::var r = mU0::Str();
		mU0::CStr(r) = x.cast<String>().str;
		return r;
	}
	if (x.type == mU0Object::$mU0Object)
		return x.cast<mU0Object>().obj;
	return mU0::Null;
}
mU0::var to_mu0(const Tuple& x) {
	mU0::var b = mU0::Vec(x.size - 1);
	for (uint i = 1; i < x.size; ++i)
		mU0::At(b, i - 1) = to_mu0(x[i]);
	if (x[0].isSymbol() && x[0].symbol() == $.List)		
		return b;
	mU0::var h = to_mu0(x[0]);
	return mU0::Ex(h, b);
}
mU0::var to_mu0(const var& x) {
	switch (x.primary()) {
	case Primary::Symbol:
		return to_mu0(x.symbol());
	case Primary::Key:
		return to_mu0(x.key());
	case Primary::Object:
		return to_mu0(x.object());
	case Primary::Tuple:
		return to_mu0(x.tuple());
	}
	return mU0::Null;
}
var from_mu0(mU0::Var x) {
	if (x == mU0::Null)
		return null;
	switch (((mU0::type_t)(((x)->id) & mU0::TYPE_MASK))) {
	case mU0::type_int: {
		Integer* r = new Integer();
		mpz_set(r->mpz, mU0::CInt(x));
		return r;
	}
	case mU0::type_rat: {
		Rational* r = new Rational();
		mpq_set(r->mpq, mU0::CRat(x));
		return r;
	}
	case mU0::type_flt: {
		Real* r = new Real();
		mpf_set(r->mpf, mU0::CFlt(x));
		return r;
	}
	case mU0::type_str: {
		String* r = new String();
		r->str = mU0::CStr(x);
		return r;
	}
	case mU0::type_sym: {
		sym r = root;
		wcs s = mU0::ContextName[mU0::Context(x)];
		wcs p = s;
		while (*p)
			if (*p == '`') {
				r = r->symbol(wstr(wstring(s, p)));
				s = ++p;
			} else
				++p;
		r = r->symbol(wstr(mU0::Name(x)));
		return r;
	}
	case mU0::type_vec: {
		uint n = CVec(x).size();
		Tuple* r = tuple(n + 1);
		r->tuple[0] = $.List;
		for (uint i = 0; i < n; ++i)
			r->tuple[i + 1] = from_mu0(mU0::At(x, i));
		return r;
	}
	case mU0::type_ex: {
		mU0::var h = mU0::Head(x);
		mU0::var b = mU0::Body(x);
		uint n = CVec(b).size();
		Tuple* r = tuple(n + 1);
		r->tuple[0] = from_mu0(h);
		for (uint i = 0; i < n; ++i)
			r->tuple[i + 1] = from_mu0(mU0::At(b, i));
		return r;
	}
	}
	// mU0::type_obj
	wostringstream oss;
	mU0::Print(x, oss);
	return new String(oss.str());
}
}