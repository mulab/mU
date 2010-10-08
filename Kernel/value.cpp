#include <mU/Common.h>
#include <mU/Kernel.h>

namespace mU {
var Kernel::value(sym x) {
    std::tr1::unordered_map<sym, var>::const_iterator
    iter = owns.find(x);
	if (iter != owns.end())
		return lazy(iter->second);
    return x;
}
var Kernel::value(const Key& x) {
    if (!x)
        return top();
    switch (x.kind()) {
    case Key::String:
        return slot(local(), x);
    case Key::Integer:
        if (top().isTuple()) {
            uint i = static_cast<uint>(x);
            if (i < top().tuple().size)
                return lazy(top().tuple()[i]);
        }
    }
    return $.Fail;
}
var Kernel::value(Tuple& x) {
    push(&x);
    var h = x[0];
    while (h.isTuple())
        h = h.tuple()[0];
    if (h.isSymbol()) {
        var r;
        if (certain(r, h.symbol(), x)) {
            pop();
            return r;
        }
        if (match(r, h.symbol(), x)) {
            pop();
            return r;
        }
        std::tr1::unordered_map<sym, var>::const_iterator
        iter = values.find(h.symbol());
        if (iter != values.end()) {
            pop();
            return cast<Value>(iter->second)(*this, x);
        }
    }
    pop();
    return &x;
}
var Kernel::value(const var& x) {
    switch (x.primary()) {
    case Primary::Symbol:
        return value(x.symbol());
    case Primary::Key:
        return value(x.key());
    case Primary::Tuple:
        return value(x.tuple());
    }
    return x;
}
bool Kernel::certain(var& r, sym h, const Tuple& x) {
	std::tr1::unordered_map<sym, Map>::const_iterator
		iter = certains.find(h);
	if (iter != certains.end()) {
		Map::const_iterator
			iter2 = iter->second.find(&x);
		if (iter2 != iter->second.end()) {
			r = lazy(iter2->second);
			return true;
		}
	}
	return false;
}
bool Kernel::match(var& r, sym h, const Tuple& x) {
	std::tr1::unordered_map<sym, MMap>::const_iterator
		iter = matches.find(h);
	if (iter != matches.end()) {
		MMap::const_iterator
			iter2 = iter->second.begin();
		while (iter2 != iter->second.end()) {
			//println(iter2->first);
			if (match(r, &x, cast<Match>(iter2->second)))
				return true;
			++iter2;
		}
	}
	return false;
}
}