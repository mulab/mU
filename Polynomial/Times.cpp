#include <mU/Number.h>
#include <mU/Polynomial.h>

namespace mU {
var Times(Kernel& k, const Tuple& x) {
	if (x.size == 1)
		return new Integer(1L);
	if (x.size == 2)
		return x[1];
	std::vector<var> r;
	uint pos = 1;
	if (isNumber(x[pos])) {
		var c = x[pos];
		for (++pos; pos < x.size && isNumber(x[pos]); ++pos)
			c = Number::Times(k, c.object(), x[pos].object());
		if(c.isObject($.Rational))
			mpq_canonicalize(cast<Rational>(c).mpq);
		if (pos == x.size || !Number::CmpD(c.object(), 0.0))
			return c;
		if (Number::CmpD(c.object(), 1.0))
			r.push_back(c);
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
	MMap::const_iterator iter = mmap.begin();
	while (iter != mmap.end()) {
		var b = iter->first, e = iter->second;
		MMap::const_iterator end = mmap.upper_bound(b);
		std::vector<var> v(1, e);
		for (++iter; iter != end; ++iter)
			v.push_back(iter->second);
		std::sort(v.begin(), v.end());
		e = mU::list(v.size(), v.begin(), $.Plus);
		e = Plus(k, e.tuple());
		if (e.isObject()) { 
			if (!Number::CmpD(e.object(), 0.0))
				continue;
			if (Number::CmpD(e.object(), 1.0))
				b = tuple($.Power, b, e);
		}
		else
			b = tuple($.Power, b, e);
		r.push_back(b);
	}
	if (r.size() == 0)
		return new Integer(1L);
	if (r.size() == 1)
		return r[0];
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
	return mU::list(r.size(), r.begin(), $.Times);
}
var Times(Kernel& k, const var& x, const var& y) {
	var r = tuple($.Times, x, y);
	r = k.flatten($.Times, r.tuple());
	std::sort(r.tuple().tuple + 1, r.tuple().tuple + r.tuple().size);
	return Times(k, r.tuple());
}
}

using namespace mU;

CAPI void CVALUE(System, Times)(Kernel& k, var& r, Tuple& x) {
	r = Times(k, x);
}