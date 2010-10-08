#include <mU/System.h>

using namespace mU;

namespace {
var Attribute(Kernel& k, sym x) {
    std::tr1::unordered_map<sym, Kernel::Attribute>::const_iterator
		iter = k.attributes.find(x);
    if (iter != k.attributes.end())
        return mU::list(iter->second.size(), iter->second.begin());
    return null;
}
}
CAPI void VALUE(Attribute)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isSymbol()) {
		r = Attribute(k, x[1].symbol());
		return;
	}
}
CAPI bool ASSIGN(Attribute)(Kernel& k, const Tuple& x, const var& y) {
    if (!x[0].isSymbol() || x.size != 2 || !x[1].isSymbol() || !y.isTuple())
        return false;
    k.attributes.erase(x[1].symbol());
    for (uint i = 1; i < y.tuple().size; ++i)
        if (y.tuple()[i].isSymbol() && $.Attributes.count(y.tuple()[i].symbol()))
            k.attributes[x[1].symbol()].insert(y.tuple()[i].symbol());
        else
            return false;
    return true;
}
CAPI void VALUE(Kind)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isKey()) {
		r = kind(x[1].key());
		return;
	}
}
CAPI void VALUE(Type)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject()) {
		r = x[1].object().type;
		return;
	}
}
CAPI void VALUE(Context)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1) {
		r = k.context();
		return;
	}
	if (x.size == 2 && x[1].isSymbol()) {
		r = x[1].symbol()->context;
		return;
	}
}
CAPI void VALUE(ContextPath)(Kernel& k, var& r, Tuple& x) {
    r = mU::list(k.contextPath().size(), k.contextPath().begin());
}
CAPI void VALUE(Primary)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 2) {
        r = primary(x[1]);
        return;
    }
    x[0] = $.List;
    map(primary, x);
}
CAPI void VALUE(Head)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 2) {
        r = x[1].head();
        return;
    }
    x[0] = $.List;
    for (uint i = 1; i < x.size; ++i)
        x[i] = x[i].head();
}
