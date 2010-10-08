#include <mU/System.h>

using namespace mU;

CAPI void VALUE(New)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		r = x[1].clone();
		return;
	}
}
CAPI void VALUE(Serial)(Kernel& k, var& r, Tuple& x) {
    for (uint i = 1; i < x.size; ++i) {
        r = k.eval(x[i]);
        if (r.isTuple() && r.tuple()[0].isSymbol()) {
			sym h = r.tuple()[0].symbol();
            if (h == $.Return) {
                r = r.tuple().size == 1 ? null : r.tuple()[1];
                return;
            }
			if (h == $.Break || h == $.Continue)
                break;
        }
    }
}
CAPI void VALUE(Module)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3 && x[1].head() == $.List) {
		std::map<var, sym> m;
		for (uint i = 1; i < x[1].tuple().size; ++i) {
			var c = x[1].tuple()[i];
			if (c.head() == $.Set) {
				sym t = c.tuple()[1].symbol()->clone();
				m[c.tuple()[1].symbol()] = t;
				k.owns[t] = c.tuple()[2];
			} else
				m[c.symbol()] = c.symbol()->clone();
		}
		r = k.eval(x[2].subs(m));
		std::map<var, sym>::const_iterator
			iter = m.begin();
		while (iter != m.end()) {
			k.destroy(iter->second);
			++iter;
		}
		m.clear();
	}
}