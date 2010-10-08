#include <mU/Number.h>

namespace mU {
var Times(Kernel& k, const Object& x, const Object& y) {
	if (x.type == $.Integer) {
		if (y.type == $.Integer) {
			Integer* r = new Integer();
			mpz_mul(r->mpz, static_cast<const Integer&>(x).mpz, 
				static_cast<const Integer&>(y).mpz);
			return r;
		}
		if (y.type == $.Rational) {
			Rational* r = new Rational();
			mpq_set_z(r->mpq, static_cast<const Integer&>(x).mpz);
			mpq_mul(r->mpq, r->mpq, static_cast<const Rational&>(y).mpq);
			return r;
		}
		if (y.type == $.Real) {
			Real* r = new Real();
			mpf_set_z(r->mpf, static_cast<const Integer&>(x).mpz);
			mpf_mul(r->mpf, r->mpf, static_cast<const Real&>(y).mpf);
			return r;
		}
	}
	if (x.type == $.Rational) {
		if (y.type == $.Integer) {
			Rational* r = new Rational();
			mpq_set_z(r->mpq, static_cast<const Integer&>(y).mpz);
			mpq_mul(r->mpq, r->mpq, static_cast<const Rational&>(x).mpq);
			return r;
		}
		if (y.type == $.Rational) {
			Rational* r = new Rational();
			mpq_mul(r->mpq, static_cast<const Rational&>(x).mpq, 
				static_cast<const Rational&>(y).mpq);
			return r;
		}
		if (y.type == $.Real) {
			Real* r = new Real();
			mpf_set_q(r->mpf, static_cast<const Rational&>(x).mpq);
			mpf_mul(r->mpf, r->mpf, static_cast<const Real&>(y).mpf);
			return r;
		}
	}
	if (x.type == $.Real) {
		if (y.type == $.Integer) {
			Real* r = new Real();
			mpf_set_z(r->mpf, static_cast<const Integer&>(y).mpz);
			mpf_mul(r->mpf, r->mpf, static_cast<const Real&>(x).mpf);
			return r;
		}
		if (y.type == $.Rational) {
			Real* r = new Real();
			mpf_set_q(r->mpf, static_cast<const Rational&>(y).mpq);
			mpf_mul(r->mpf, r->mpf, static_cast<const Real&>(x).mpf);
			return r;
		}
		if (y.type == $.Real) {
			Real* r = new Real();
			mpf_mul(r->mpf, static_cast<const Real&>(x).mpf, 
				static_cast<const Real&>(y).mpf);
			return r;
		}
	}
	Tuple* r = tuple(3);
	if (x.compare(y) == 0) {
		r->tuple[0] = $.Power;
		r->tuple[1] = &x;
		r->tuple[2] = new Integer(2L);
	} else {
		r->tuple[0] = $.Times;
		r->tuple[1] = &x;
		r->tuple[2] = &y;
	}
	return r;
}
var Times(Kernel& k, const Tuple& x) {
	if (x.size == 1)
		return new Integer(1L);
	if (x.size == 2)
		return x[1];
	uint pos = 1;
	std::vector<var> t;
	if (isNumber(x[1])) {
		var c = x[1];
		for (pos = 2; pos < x.size && isNumber(x[pos]); ++pos);
		for (uint j = 2; j < pos; ++j)
			c = Times(k, c.object(), x[j].object());
		if(c.isObject($.Rational))
			mpq_canonicalize(cast<Rational>(c).mpq);
		if (pos == x.size || !cmpD(c.object()))
			return c;
		if (cmpD(c.object(), 1.0))
			t.push_back(c);
	}
	MMap mmap;
	for (; pos < x.size; ++pos) {
		var b = x[pos], e;
		if (b.isTuple($.Power)) {
			const Tuple& t = b.tuple();
			b = t[1];
			e = t[2];
		} else
			e = new Integer(1L);
		mmap.insert(std::make_pair(b,e));
	}
	{
		MMap::const_iterator iter = mmap.begin();
		while (iter != mmap.end()) {
			var b = iter->first, e = iter->second;
			MMap::const_iterator end = mmap.upper_bound(b);
			std::vector<var> v(1, e);
			++iter;
			for (; iter != end; ++iter)
				v.push_back(iter->second);
			std::sort(v.begin(), v.end());
			e = mU::list(v.size(), v.begin(), $.Plus);
			e = Plus(k, e.tuple());
			if (e.isObject()) { 
				if (cmpD(e.object()) == 0)
					continue;
				if (cmpD(e.object(), 1.0))
					b = tuple($.Power, b, e);
			}
			else
				b = tuple($.Power, b, e);
			t.push_back(b);
		}
	}
	if (t.size() == 0)
		return new Integer(1L);
	if (t.size() == 1)
		return t[0];
	/*
	if (t[0].isObject() && cmpFrac(t[0].object(), -1.0) == 0) {
        uint i;
        for (i = 1; i < t.size(); ++i)
            if (t[i].isTuple($.Plus)) {
                t[i] = Expand(t[0], t[i]);
                break;
            }
        if (i < t.size()) {
            if (t.size() == 2) return t[1];
			return mU::list(t.size() - 1, t.begin() + 1, $.Times);
		}
    }
	*/
	return mU::list(t.size(), t.begin(), $.Times);
}
/*
var Kernel::Times(const var& x, const var& y) const {
	std::vector<var> t;
	if (x.isV(SYM(Times)))
		Flatten(t,SYM(Times),x.toV());
	else
		t.push_back(x);
	if (y.isV(SYM(Times)))
		Flatten(t,SYM(Times),y.toV());
	else
		t.push_back(y);
	std::sort(t.begin(),t.end(),Before);
	return Times(t);
}
*/
}

using namespace mU;

CAPI void CVALUE(System, Times)(Kernel& k, var& r, Tuple& x) {
	r = Times(k, x);
}