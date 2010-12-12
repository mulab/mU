#include <mU/utils.h>
#include "EclObject.h"

namespace mU {
cl_object 
read_from_string_clobj, safe_eval_clobj, safe_apply_clobj, safe_funcall_clobj;
cl_object ecl_safe_eval(cl_object form) {
	cl_env_ptr env = ecl_process_env();
	CL_UNWIND_PROTECT_BEGIN(env) {
	/*
		* Code that is protected. Uncaught lisp conditions, THROW,
		* signals such as SIGSEGV and SIGBUS may cause jump to
		* this region.
		*/
    cl_funcall(2, safe_eval_clobj, form);
	} CL_UNWIND_PROTECT_EXIT {
	/*
		* If the exception, lisp condition or other control transfer
		* is caught, this code is executed. After this code, the
		* process will jump to the original destination of the
		* THROW, GOTO or other control statement that was interrupted.
		*/
	} CL_UNWIND_PROTECT_END
	/*
	* We only exit here if NO nonlocal jump was interrupted.
	*/
	;
	if (ecl_nvalues > 1) {
		return ecl_values(1);
		// raise RuntimeError, "ECL says: "+ecl_base_string_pointer_safe(ecl_values(1))
	}
	else
		return ecl_values(0);
}

cl_object ecl_safe_funcall(cl_object func, cl_object arg) {
    cl_object l;
    l = cl_cons(func, cl_cons(arg, Cnil));
    cl_apply(2, safe_funcall_clobj, cl_cons(func, cl_cons(arg, Cnil)));
    if (ecl_nvalues > 1) {
		return ecl_values(1);
        // raise RuntimeError, "ECL says: "+ecl_base_string_pointer_safe(ecl_values(1))
	}
    else
        return ecl_values(0);
}
cl_object ecl_safe_apply(cl_object func, cl_object args) {
    cl_funcall(3, safe_apply_clobj, func, args);
    if (ecl_nvalues > 1) {
		return ecl_values(1);
        // raise RuntimeError, "ECL says: "+ecl_base_string_pointer_safe(ecl_values(1))
	}
    else
		return ecl_values(0);
}
cl_object ecl_safe_read_cstring(const char* s) {
	cl_object o = ecl_cstring_to_base_string_or_nil(s);
	o = ecl_safe_funcall(read_from_string_clobj, o);
	return o;
}
cl_object ecl_safe_eval_cstring(const char* s) {
	cl_object o = ecl_safe_read_cstring(s);
	return ecl_safe_eval(o);
}
namespace {
inline string to_ecl_name(sym x) {
	ostringstream oss;
	wcs w = x->name();
	if (w) {
		string t = wcs2mbs(w);
		const char* s = t.c_str();
		while (*s) {
			if (isupper(*s))
				oss << '*';
			oss << *s;
			++s;
		}
	}
	else
		oss << '-' << static_cast<const void*>(x);
	return oss.str();
}
}
cl_object to_ecl(sym x) {
	if (!x || x == $.False)
		return Cnil;
	if (x == $.True)
		return Ct;
	string s = to_ecl_name(x);
	x = x->context;
	while (x) {
		s = to_ecl_name(x) + '_' + s;
		x = x->context;
	}
	return ecl_safe_read_cstring(s.c_str());
}
cl_object to_ecl(const Key& x) {
	ostringstream oss;
	oss << ':';
	if (x) {
		if (x.kind() == Key::Integer)
			oss << x.toUI();
		else
			oss << wcs2mbs(x.toS());
	}
	return ecl_safe_read_cstring(oss.str().c_str());
}
cl_object to_ecl(const Object& x) {
	if (x.type == $.Integer) {
		const mpz_t& z = x.cast<Integer>().mpz;
		if (mpz_fits_sint_p(z))
			return ecl_make_integer(mpz_get_si(z));
		cl_object r = ecl_alloc_object(t_bignum);
		mpz_init2(r->big.big_num, GMP_LIMB_BITS);
		mpz_set(r->big.big_num, z);
		// return big_register_copy(r);
		return r;
	}
	if (x.type == $.Rational) {
		const mpq_t& q = x.cast<Rational>().mpq;
		cl_object a;
		if (mpz_fits_sint_p(mpq_numref(q)))
			a = ecl_make_integer(mpz_get_si(mpq_numref(q)));
		else {
			a = ecl_alloc_object(t_bignum);
			mpz_init2(a->big.big_num, GMP_LIMB_BITS);
			mpz_set(a->big.big_num, mpq_numref(q));
		}
		cl_object b;
		if (mpz_fits_sint_p(mpq_denref(q)))
			b = ecl_make_integer(mpz_get_si(mpq_denref(q)));
		else {
			b = ecl_alloc_object(t_bignum);
			mpz_init2(b->big.big_num, GMP_LIMB_BITS);
			mpz_set(b->big.big_num, mpq_denref(q));
		}
		return ecl_make_ratio(a, b);
	}
	if (x.type == $.Real)
		return ecl_make_doublefloat(x.cast<Real>().toD());
	if (x.type == $.String) {
		string r = wcs2mbs(x.cast<String>().str);
		r = '"' + r + '"';
		return ecl_safe_read_cstring(r.c_str());
	}
	if (x.type == EclObject::$EclObject)
		return x.cast<EclObject>().obj;
	return Cnil;
}
cl_object to_ecl(const Tuple& x) {
	cl_object r = cl_cons(to_ecl(x[0]), Cnil);
	cl_object ptr = r;
	for (uint i = 1; i < x.size; ++i) {
		cl_rplacd(ptr, cl_cons(to_ecl(x[i]), Cnil));
		ptr = cl_cdr(ptr);
	}
	return r;
}
cl_object to_ecl(const var& x) {
	switch (x.primary()) {
	case Primary::Symbol:
		return to_ecl(x.symbol());
	case Primary::Key:
		return to_ecl(x.key());
	case Primary::Object:
		return to_ecl(x.object());
	case Primary::Tuple:
		return to_ecl(x.tuple());
	}
	return Cnil;
}
namespace {
inline sym from_ecl_name(sym c, const char* s) {
	string t;
	while (*s) {
		if (*s == '*') {
			++s;
			t += *s;
		} else
			t += tolower(*s);
		++s;
	}
	return c->symbol(wstr(mbs2wcs(t)));
}
}
var from_ecl(cl_object x) {
	if (x == Cnil)
		return null;
	switch (type_of(x)) {
	case t_fixnum:
		return new Integer((sint)fixint(x));
	case t_bignum: {
		Integer* r = new Integer();
		mpz_set(r->mpz, x->big.big_num);
		return r;
	}
	break;
	case t_ratio: {
		Rational* r = new Rational();
		cl_object a = cl_numerator(x);
		if (FIXNUMP(a))
			mpz_set_si(mpq_numref(r->mpq), fixint(a));
		else
			mpz_set(mpq_numref(r->mpq), a->big.big_num);
		cl_object b = cl_denominator(x);
		if (FIXNUMP(b))
			mpz_set_si(mpq_denref(r->mpq), fixint(b));
		else
			mpz_set(mpq_denref(r->mpq), b->big.big_num);
		return r;
	}
	break;
	case t_doublefloat:  {
		Real* r = new Real();
		mpf_set_d(r->mpf, ecl_to_double(x));
		return r;
	}
	break;
	case t_symbol: {
		if (x == Ct)
			return $.True;
		const char* s;
		sym r = root;
		cl_object c = cl_symbol_package(x);
		if (c != EclObject::$mU) {
			s = ecl_base_string_pointer_safe(cl_package_name(c));
			r = from_ecl_name(EclObject::$Ecl, s);
		}
		s = ecl_base_string_pointer_safe(cl_symbol_name(x));
		const char* p = s;
		while (*p)
			if (*p == '_') {
				r = from_ecl_name(r, string(s, p).c_str());
				s = ++p;
			} else
				++p;
		r = from_ecl_name(r, s);
		return r;
	}
	break;
	case t_list: {
		std::vector<var> t;
		while (x != Cnil) {
			t.push_back(from_ecl(cl_car(x)));
			x = cl_cdr(x);
		}
		return mU::tuple(t.size(), t.begin());
	}
	break;
	}
	return new String(mbs2wcs(ecl_base_string_pointer_safe(cl_write_to_string(1, x))));
}
/*#Maxima's BFLOAT multiprecision float type can be read with:
#def bfloat_to_python(e):
#  prec=Integer(str(e.car().cddr().car()))
#  mant=Integer(str(e.cdr().car()))
#  exp=Integer(str(e.cddr().car()))
#  return 2^(exp-prec)*mant
*/
}

using namespace mU;

#undef ASSIGN
#define ASSIGN(f) CASSIGN(Ecl,f)
#undef VALUE
#define VALUE(f) CVALUE(Ecl,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(Ecl,f,sig)

CAPI void VALUE(Ecl)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[0].isSymbol()) {
		r = new EclObject(to_ecl(x[1]));
		return;
	}
	if (x[0].isTuple(EclObject::$Ecl)) {
	}
}
CAPI void VALUE(Init)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1) {
		char* argv = "";
		EclObject::Init(0, &argv);
		r = null;
		return;
	}
}
CAPI void VALUE(Eval)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject()) {
		if (x[1].object().type == $.String) {
			r = new EclObject(ecl_safe_eval_cstring(wcs2mbs(x[1].cast<String>().str).c_str()));
			return;
		}
		if (x[1].object().type == EclObject::$EclObject) {
			r = new EclObject(ecl_safe_eval(x[1].cast<EclObject>().obj));
			return;
		}
	}
}
CAPI void METHOD(mU, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = from_ecl(x[1].cast<EclObject>().obj);
}
CAPI void METHOD(Read, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = new EclObject(ecl_safe_read_cstring(wcs2mbs(x[1].cast<String>().str).c_str()));
}