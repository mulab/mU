#include <mU/System.h>

using namespace mU;

namespace {
class Dispatch : public Match {
	Map mMap;
public:
	Dispatch(const Tuple& x) {
		for (uint i = 1; i < x.size; ++i)
			if (x[i].isTuple() 
				&& x[i].tuple()[0] == $.Rule
				&& x[i].tuple().size == 3)
				mMap[x[i].tuple()[1]] = x[i].tuple()[2];
	}
	virtual bool operator()(Kernel& k, var& r, const var& x) {
		Map::const_iterator iter = mMap.find(x);
		if (iter != mMap.end()) {
			r = iter->second;
			return true;
		}
		return false;
	}
};
}
CAPI void VALUE(Dispatch)(Kernel& k, var& r, Tuple& x) {
	r = new Dispatch(x);
}
CAPI void VALUE(MatchC)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		r = matchC(k, x[1]);
		return;
	}
}
CAPI void VALUE(RuleC)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		r = ruleC(k, x[1], x[2]);
		return;
	}
}
CAPI void VALUE(TestC)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		r = testC(k, x[1], x[2]);
		return;
	}
}
CAPI void VALUE(ListC)(Kernel& k, var& r, Tuple& x) {
		r = listC(k, x);
}
CAPI void VALUE(MatchQ)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		if (!(x[2].isObject() && x[2].object().type == $.Match))
			x[2] = matchC(k, x[2]);
		var t;
		r = boolean(k.match(t, x[1], cast<Match>(x[2])));
		return;
	}
}
CAPI void VALUE(Replace)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		if (!(x[2].isObject() && x[2].object().type == $.Match)) {
			if (x[2].isTuple() && x[2].tuple()[0] == $.List)
				x[2] = listC(k,x[2].tuple());
			else
				x[2] = matchC(k, x[2]);
		}
		r = x[1];
		k.match(r, x[1], cast<Match>(x[2]));
		return;
	}
}
CAPI void VALUE(ReplaceAll)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		if (!(x[2].isObject() && x[2].object().type == $.Match)) {
			if (x[2].isTuple() && x[2].tuple()[0] == $.List)
				x[2] = listC(k,x[2].tuple());
			else
				x[2] = matchC(k, x[2]);
		}
		r = k.replace(x[1], cast<Match>(x[2]));
		return;
	}
}
CAPI void VALUE(ReplaceRepeated)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		if (!(x[2].isObject() && x[2].object().type == $.Match)) {
			if (x[2].isTuple() && x[2].tuple()[0] == $.List)
				x[2] = listC(k,x[2].tuple());
			else
				x[2] = matchC(k, x[2]);
		}
		r = k.replace(x[1], cast<Match>(x[2]));
		while (r != x[1]) {
			x[1] = r;
			r = k.replace(x[1], cast<Match>(x[2])); 
		}
		return;
	}
}
CAPI bool ASSIGN(Condition)(Kernel& k, const Tuple& x, const var& y) {
	if (!x[0].isSymbol())
		return false;
	var h = k.eval(x[1]);
	while (h.isTuple())
		h = h.tuple()[0];
	if (h.isSymbol()) {
		if (y)
			k.matches[h.symbol()].insert(std::make_pair(&x, y));
		else
			k.matches[h.symbol()].erase(&x);
		return true;
	}
	return false;
}