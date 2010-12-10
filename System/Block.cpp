#include <mU/System.h>

using namespace mU;

CAPI void VALUE(Block)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		k.beginLocal();
		r = k.eval(x[1]);
		k.endLocal();
		return;
	}
	if (x.size == 3 && x[1].isTuple($.List)) {
		std::vector<std::pair<sym, var> > v(x[1].tuple().size);
		k.beginLocal();
		for (uint i = 1; i < x[1].tuple().size; ++i) {
			var c = x[1].tuple()[i];
			if (c.isSymbol()) {
				v[i].first = c.symbol();
				v[i].second = k.value(c.symbol());
			}
			else if (c.isTuple($.Set) && c.tuple().size == 3 && c.tuple()[1].isSymbol()) {
				v[i].first = c.tuple()[1].symbol();
				v[i].second = k.value(c.tuple()[1].symbol());
				k.assign(c.tuple()[1].symbol(), k.eval(c.tuple()[2]));
			} else
				k.eval(c);
		}
		r = k.eval(x[2]);
		k.endLocal();
		std::vector<std::pair<sym, var> >::const_iterator
			iter = v.begin();
		while (iter != v.end()) {
			k.assign(iter->first, iter->second);
			++iter;
		}
	}
}
namespace {
template <class T>
var SubstituteSymbol(const var& x, const T& m) {
	if (x.isSymbol()) {
		typename T::const_iterator
			iter = m.find(x.symbol());
		if (iter != m.end())
			return iter->second;
	} else if (x.isTuple()) {
		Tuple* r = x.tuple().clone();
		for (uint i = 0; i < x.tuple().size; ++i)
			r->tuple[i] = SubstituteSymbol(x.tuple()[i], m);
		return r;
	}
	return x;
}
}
CAPI void VALUE(Module)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3 && x[1].isTuple($.List)) {
		typedef std::unordered_map<sym, sym> UMap;
		UMap m;
		for (uint i = 1; i < x[1].tuple().size; ++i) {
			var c = x[1].tuple()[i];
			if (c.isSymbol())
				m[c.symbol()] = c.symbol()->clone();
			else if (c.isTuple($.Set) && c.tuple().size == 3 && c.tuple()[1].isSymbol()) {
				sym t = c.tuple()[1].symbol()->clone();
				m[c.tuple()[1].symbol()] = t;
				k.assign(t, c.tuple()[2]);
			}
		}
		r = k.eval(SubstituteSymbol(x[2], m));
		UMap::const_iterator
			iter = m.begin();
		while (iter != m.end()) {
			k.destroy(iter->second);
			++iter;
		}
	}
}
CAPI void VALUE(With)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3 && x[1].isTuple($.List)) {
		typedef std::tr1::unordered_map<var, var> UMap;
		UMap m;
		for (uint i = 1; i < x[1].tuple().size; ++i) {
			var c = x[1].tuple()[i];
			if (c.isTuple($.Set) && c.tuple().size == 3)
				m[c.tuple()[1]] = c.tuple()[2];
		}
		r = k.eval(With(x[2], m));
	}
}
