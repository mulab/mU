#include <mU/Kernel.h>

namespace mU {
bool Kernel::assign(sym x, const var& y) {
    if (y)
        owns[x] = y;
    else
        owns.erase(x);
    return true;
}
bool Kernel::assign(const Key& x, const var& y) {
	if (!x)
		return false;
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
    var h = x[0];
    while (h.isTuple())
        h = h.tuple()[0];
    if (h.isSymbol()) {
        std::tr1::unordered_map<sym, var>::const_iterator
        iter = assigns.find(h.symbol());
        if (iter != assigns.end())
            return iter->second.cast<Assign>()(*this, x, y);
        if (y)
            certains[h.symbol()][&x] = y;
        else
            certains[h.symbol()].erase(&x); 
        return true;
    }
    return false;
}
bool Kernel::rule(const Tuple& x, const Match* y) {
	var h = x[0];
	while (h.isTuple())
		h = h.tuple()[0];
	if (h.isSymbol()) {
		std::tr1::unordered_map<sym, var>::const_iterator
			iter = assigns.find(h.symbol());
		if (iter != assigns.end())
			return iter->second.cast<Assign>()(*this, x, y);
		if (y)
			matches[h.symbol()][&x] = y;
		else
			matches[h.symbol()].erase(&x);
		return true;
	}
	return false;
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