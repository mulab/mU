#include <mU/Polynomial.h>
#include <mU/Number.h>
#include <algorithm>
/*#include <functional>
using namespace std::placeholders;
*/

namespace mU {
var Plus(Kernel& k, const Tuple& x) {
    if (x.size == 1)
		return new Integer(0L);
    if (x.size == 2) 
		return x[1];
    std::vector<var> r;
	uint pos = 1;
    if (isNumber(x[1])) {
		uint end = 2;
		while (end < x.size && x[end].isObject())
			++end;
		var c = x[1].clone();
		for (pos = 2; pos < end && isNumber(x[pos].object()); ++pos)
            Number::add(c, c.object(), x[pos].object());
        if(c.isObject($.Rational))
			mpq_canonicalize(c.cast<Rational>().mpq);
		if (cmpD(c.object(), 0.0))
			r.push_back(c);
    }
	typedef std::unordered_map<var, std::vector<var> > UMap;
	UMap map;
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
        map[b].push_back(e);
    }
	UMap::const_iterator iter = map.begin();
	while (iter != map.end()) {
		var b = iter->first;
		const std::vector<var>& v = iter->second;
		var e = v[0];
		for (uint i = 1; i < v.size(); ++i)
			Number::add(e, e.object(), v[i].object());
		++iter;
		/*std::for_each(++v.begin(), v.end(), std::bind(Number::add, e, e.object(), 
			std::bind(&var::object, _1)));
			*/
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
	if (!x)
		return y;
	if (!y)
		return x;
	var r;
	if (x.isObject() && y.isObject()) {
		Number::add(r, x.object(), y.object());
		return r;
	}
	r = tuple($.Plus, x, y);
    r = k.flatten($.Plus, r.tuple());
	std::sort(r.tuple().tuple + 1, r.tuple().tuple + r.tuple().size, lessPrimary);
    return Plus(k, r.tuple());
}
}

using namespace mU;

CAPI void CVALUE(System, Plus)(Kernel& k, var& r, Tuple& x) {
	r = Plus(k, x);
}