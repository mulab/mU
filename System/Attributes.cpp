#include <mU/System.h>

namespace mU {
var Attributes(Kernel& k, sym x) {
	std::tr1::unordered_map<sym, Kernel::Attribute>::const_iterator
		iter = k.attributes.find(x);
	if (iter != k.attributes.end())
		return mU::list(iter->second.size(), iter->second.begin());
	return null;
}
}

using namespace mU;

CAPI void VALUE(Attributes)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isSymbol()) {
		r = Attributes(k, x[1].symbol());
		return;
	}
}
CAPI bool ASSIGN(Attributes)(Kernel& k, const Tuple& x, const var& y) {
    if (!x[0].isSymbol() || x.size != 2 || !x[1].isSymbol() || !y.isTuple())
        return false;
    k.attributes.erase(x[1].symbol());
	Kernel::Attribute& attr = k.attributes[x[1].symbol()];
	for (uint i = 1; i < y.tuple().size; ++i)
		if (y.tuple()[i].isSymbol() && $.Attributes.count(y.tuple()[i].symbol()))
			attr.insert(y.tuple()[i].symbol());
		else {
			if (attr.size() == 0)
				k.attributes.erase(x[1].symbol());
			return false;
		}
	if (attr.size() == 0)
		k.attributes.erase(x[1].symbol());
    return true;
}
CAPI void VALUE(ClearAttributes)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3 && x[1].isSymbol()) {
		std::tr1::unordered_map<sym, Kernel::Attribute>::iterator
			iter = k.attributes.find(x[1].symbol());
		if (iter == k.attributes.end())
			return;
		if (x[2].isSymbol())
			iter->second.erase(x[2].symbol());
		else if (x[2].isTuple()) {
			for (uint i = 1; i < x[2].tuple().size; ++i)
				if (x[2].tuple()[i].isSymbol())
					iter->second.erase(x[2].tuple()[i].symbol());
		}
		r = mU::list(iter->second.size(), iter->second.begin());
	}
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
CAPI void VALUE(Primary)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		r = primary(x[1]);
		return;
	}
	x[0] = $.List;
	map(primary, x);
}
CAPI void VALUE(SetAttributes)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3 && x[1].isSymbol()) {
		Kernel::Attribute& attr = k.attributes[x[1].symbol()];
		if (x[2].isSymbol() && $.Attributes.count(x[2].symbol()))
			attr.insert(x[2].symbol());
		else if (x[2].isTuple()) {
			for (uint i = 1; i < x[2].tuple().size; ++i)
				if (x[2].tuple()[i].isSymbol() && $.Attributes.count(x[2].tuple()[i].symbol()))
					attr.insert(x[2].tuple()[i].symbol());
		}
		r = mU::list(attr.size(), attr.begin());
		if (attr.size() == 0)
			k.attributes.erase(x[1].symbol());
	}
}
CAPI void VALUE(Type)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject()) {
		r = x[1].object().type;
		return;
	}
}
