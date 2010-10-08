#include <mU/Number.h>

namespace mU {
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
			Real* r = new Real();
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
			Real* r = new Real();
			mpf_set_q(r->mpf, static_cast<const Rational&>(x).mpq);
			mpf_add(r->mpf, r->mpf, static_cast<const Real&>(y).mpf);
			return r;
		}
	}
	if (x.type == $.Real) {
		if (y.type == $.Integer) {
			Real* r = new Real();
			mpf_set_z(r->mpf, static_cast<const Integer&>(y).mpz);
			mpf_add(r->mpf, r->mpf, static_cast<const Real&>(x).mpf);
			return r;
		}
		if (y.type == $.Rational) {
			Real* r = new Real();
			mpf_set_q(r->mpf, static_cast<const Rational&>(y).mpq);
			mpf_add(r->mpf, r->mpf, static_cast<const Real&>(x).mpf);
			return r;
		}
		if (y.type == $.Real) {
			Real* r = new Real();
			mpf_add(r->mpf, static_cast<const Real&>(x).mpf, 
				static_cast<const Real&>(y).mpf);
			return r;
		}
	}
	Tuple* r = tuple(3);
	if (x.compare(y) == 0) {
		r->tuple[0] = $.Times;
		r->tuple[1] = new Integer(2L);
		r->tuple[2] = &x;
	} else {
		r->tuple[0] = $.Plus;
		r->tuple[1] = &x;
		r->tuple[2] = &y;
	}
	return r;
}
var Plus(Kernel& k, const Tuple& x) {
    if (x.size == 1) 
		return new Integer(0L);
    if (x.size == 2) 
		return x[1];
    uint pos = 1;
	std::vector<var> t;
    if (isNumber(x[1])) {
        var c = x[1];
        for (pos = 2; pos < x.size && isNumber(x[pos]); ++pos);
        for (uint j = 2; j < pos; ++j)
            c = Plus(k, c.object(), x[j].object());
        if(c.isObject($.Rational))
			mpq_canonicalize(cast<Rational>(c).mpq);
        if (pos == x.size)
			return c;
        if (cmpD(c.object()))
            t.push_back(c);
    }
	MMap mmap;
    for (; pos < x.size; ++pos) {
        var b = x[pos], e;
        if (b.isTuple($.Times)) {
			const Tuple& t = b.tuple();
            if (isNumber(t[1])) {
                e = t[1];
                if (t.size == 3)
                    b = t[2];
				else {
					Tuple* c = tuple(t.size - 1);
					c->tuple[0] = $.Times;
					for(uint i = 2; i < t.size; ++i)
						c->tuple[i - 1] = t[i];
					b = c;
				}
            } else
                e = new Integer(1L);
        } else
            e = new Integer(1L);
        mmap.insert(std::make_pair(b,e));
    }
    {
		MMap::const_iterator iter = mmap.begin();
        while (iter != mmap.end()) {
            var b = iter->first, e = iter->second;
            MMap::const_iterator end = mmap.upper_bound(b);
			++iter;
			for (; iter != end; ++iter)
				e = Plus(k, e.object(),iter->second.object());
			double ed = toD(e.object());
			if (ed != 0.0) {
				if (ed != 1.0) {
					if (b.isTuple($.Times)) {
						Tuple* c = tuple(b.tuple().size + 1);
						c->tuple[0] = $.Times;
						c->tuple[1] = e;
						for (uint i = 1; i < b.tuple().size; ++i)
							c->tuple[i + 1] = b.tuple()[i];
						b = c;
					} else
						b = tuple($.Times, e, b);
				}
				t.push_back(b);
			}
        }
    }
    if (t.size() == 0)
		return new Integer(0L);
    if (t.size() == 1)
		return t[0];
    return mU::list(t.size(), t.begin(), $.Plus);
}
/*
var Plus(const var& x, const var& y) {
	std::vector<var> t;
    if (x.isTuple($.Plus))
        Flatten(t,$.Plus,x.toV());
    else
        t.push_back(x);
	if (y.isV(SYM(Plus)))
		Flatten(t,SYM(Plus),y.toV());
	else
		t.push_back(y);
	std::sort(t.begin(),t.end(),Before);
    return Plus(t);
}
*/
}

using namespace mU;

CAPI void CVALUE(System, Plus)(Kernel& k, var& r, Tuple& x) {
	r = Plus(k, x);
}