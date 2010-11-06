#include <mU/Number.h>
#include <mU/Polynomial.h>
#ifdef _MSC_VER
#pragma comment(lib,"Number.lib")
#endif

namespace mU {
var Plus(Kernel& k, const Tuple& x) {
    if (x.size == 1) 
		return new Integer(0L);
    if (x.size == 2) 
		return x[1];
    std::vector<var> r;
	uint pos = 1;
    if (isNumber(x[pos])) {
        var c = x[pos];
        for (++pos; pos < x.size && isNumber(x[pos]); ++pos)
            c = Number::Plus(k, c.object(), x[pos].object());
        if(c.isObject($.Rational))
			mpq_canonicalize(cast<Rational>(c).mpq);
        if (pos == x.size)
			return c;
        if (cmpD(c.object(), 0.0))
            r.push_back(c);
    }
	MMap mmap;
    for (; pos < x.size; ++pos) {
        var b = x[pos], e;
		if (b == $.Infinity)
			return $.Infinity;
        if (b.isTuple($.Times)) {
			const Tuple& t = b.tuple();
            if (isNumber(t[1])) {
                e = t[1];
                if (t.size == 3) {
					b = t[2];
					if (b == $.Infinity)
						return x[pos];
				}
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
	MMap::const_iterator iter = mmap.begin();
	while (iter != mmap.end()) {
		var b = iter->first, e = iter->second;
		MMap::const_iterator end = mmap.upper_bound(b);
		for (++iter; iter != end; ++iter)
			e = Number::Plus(k, e.object(),iter->second.object());
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
			r.push_back(b);
		}
	}
    if (r.size() == 0)
		return new Integer(0L);
    if (r.size() == 1)
		return r[0];
    return mU::list(r.size(), r.begin(), $.Plus);
}
var Plus(Kernel& k, const var& x, const var& y) {
	var r = tuple($.Plus, x, y);
    r = k.flatten($.Plus, r.tuple());
	std::sort(r.tuple().tuple + 1, r.tuple().tuple + r.tuple().size);
    return Plus(k, r.tuple());
}
}

using namespace mU;

CAPI void CVALUE(System, Plus)(Kernel& k, var& r, Tuple& x) {
	r = Plus(k, x);
}