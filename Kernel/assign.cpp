#include <mU/Match.h>

namespace mU {
bool Kernel::assign(sym x, const var& y) {
    if (y)
        owns[x] = y;
    else
        owns.erase(x);
    return true;
}
bool Kernel::assign(const Key& x, const var& y) {
    switch (x.kind()) {
    case Key::String:
        return set(local(), x, y);
    case Key::Integer:
        if (top().isTuple()) {
            uint i = x.toUI();
            if (i < top().tuple().size) {
                top().tuple()[i] = y;
                return true;
            }
        }
    }
    return false;
}
bool Kernel::assign(const Tuple& x, const var& y) {
    var h = eval(x[0]);
    while (h.isTuple())
        h = h.tuple()[0];
	if (!h.isSymbol()) {
		if (h.isObject())
			h = h.object().type;
		else
			return false;
	}
    std::unordered_map<sym, var>::const_iterator
    iter = assigns.find(h.symbol());
    if (iter != assigns.end())
        return iter->second.cast<Assign>()(*this, x, y);
	var t = x.clone();
	t = rewrite(&t.tuple());
    if (y) {
		if (isCertain(t))
			certains[h.symbol()][t] = y;
		else
			matches[h.symbol()][t] = crule(*this, t, y);
	}
    else {
		if (isCertain(t))
			certains[h.symbol()].erase(t);
		else
			matches[h.symbol()].erase(t);
	}
    return true;
}
bool Kernel::assign(const var& x, const var& y) {
    switch (x.primary()) {
    case Primary::Symbol:
        return assign(x.symbol(), y);
    case Primary::Key:
        return assign(x.key(), y);
    case Primary::Tuple:
        return assign(x.tuple(), y);
    }
    return false;
}
}