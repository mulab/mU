#include <mU/var.h>

namespace mU {
std::tr1::unordered_set<wstring> wstrs;
std::tr1::unordered_map<sym, wcs> names;
std::tr1::unordered_map<sym, Context> contexts;
std::tr1::unordered_map<uint, var> keys;
sym root = 0, sys;
const var null(0);
void Symbol::ruin(Var* x) {
    Symbol* r = static_cast<Symbol*>(x);
	if (!r->name() && !contexts.empty()) {
        contexts[r->context].erase(reinterpret_cast<uint>(r));
		contexts.erase(r);
	}
    delete r;
}
wcs Symbol::name() const {
    std::tr1::unordered_map<sym, wcs>::const_iterator
    iter = names.find(this);
    if (iter != names.end())
        return iter->second;
    return 0;
}
int Symbol::compare(sym x) const {
    if (!this)
        return x ? -1 : 0;
    if (!x)
        return 1;
    int r = context->compare(x->context);
    if (r)
        return r;
    return reinterpret_cast<long>(this) - reinterpret_cast<long>(x);
}
sym Symbol::clone(wcs w) const {
    sym r = new Symbol(this);
    if (w) {
        names[r] = w;
        contexts[this][wcs2uint(w)] = r;
    } else
        contexts[this][reinterpret_cast<uint>(r)] = r;
    return r;
}
sym Symbol::symbol(wcs x) const {
    std::pair<Context::iterator, bool>
    r = contexts[this].insert(std::make_pair(wcs2uint(x), null));
    if (!r.second)
        return r.first->second.isSymbol() ? r.first->second.symbol() : 0;
    sym t = clone(x);
    r.first->second = t;
    return t;
}
var Symbol::get(wcs x) const {
    std::tr1::unordered_map<sym, Context>::const_iterator
    iter = contexts.find(this);
    if (iter != contexts.end()) {
        Context::const_iterator
        iter2 = iter->second.find(wcs2uint(x));
        if (iter2 != iter->second.end())
            return iter2->second;
    }
    return null;
}
bool Symbol::set(wcs x, const var& y) const {
    if (y)
        contexts[this][wcs2uint(x)] = y;
    else
        contexts[this].erase(wcs2uint(x));
    return true;
}
void Key::ruin(Var* x) {
    delete static_cast<Key*>(x);
}
void Object::ruin(Var* x) {
    delete static_cast<Object*>(x);
}
void Tuple::ruin(Var* x) {
    Tuple* t = static_cast<Tuple*>(x);
    for (uint i = 0; i < t->size; ++i)
        t->tuple[i].~var();
    delete reinterpret_cast<char*>(x);
}
namespace Primary {
void(* const ruin[size])(Var*) = {
    Object::ruin,
	Key::ruin,
	Symbol::ruin,    
    Tuple::ruin
};
}
bool var::isTuple(const var& x) const {
	return isTuple() && tuple()[0] == x;
}
bool var::isTuple(sym x) const {
	return isTuple() && tuple()[0] == x;
}
Tuple& var::tuple() const {
    return static_cast<Tuple&>(*ptr);
}
var var::head() const {
    switch (primary()) {
    case Primary::Symbol:
        return symbol()->context;
    case Primary::Object:
        return object().type;
    case Primary::Tuple:
        return tuple()[0];
    }
    return null;
}
var var::clone() const {
    switch (primary()) {
    case Primary::Symbol:
        return symbol()->clone();
    case Primary::Object:
        return object().clone();
    case Primary::Tuple:
        return tuple().clone();
    }
    return *this;
}
int var::compare(const var& x) const {
    int r = primary() - x.primary();
    if (r)
        return r;
    switch (primary()) {
    case Primary::Symbol:
        return symbol()->compare(x.symbol());
    case Primary::Key: {
        int r = key().kind() - x.key().kind();
        if (r)
            return r;
    }
    return key().key - x.key().key;
    case Primary::Object: {
        int r = object().type->compare(x.object().type);
        if (r)
            return r;
    }
    return object().compare(x.object());
    case Primary::Tuple: {
        int r = tuple()[0].compare(x.tuple()[0]);
        if (r)
            return r;
        if (r = static_cast<int>(tuple().size) - static_cast<int>(x.tuple().size))
            return r;
        for (uint i = 1; i < tuple().size; ++i)
            if (r = tuple()[i].compare(x.tuple()[i]))
                break;
        return r;
    }
    break;
    }
    return 0;
}
bool var::depend(const var& x) const {
	if (compare(x)) {
		if (isTuple()) {
			for (uint i = 0; i < tuple().size; ++i)
				if (tuple()[i].depend(x))
					return true;
		}
		return false;
	}
	return true;
}
Tuple* Tuple::clone() const {
    Tuple* r = mU::tuple(size);
    for (uint i = 0; i < size; ++i)
        r->tuple[i] = (*this)[i];
    return r;
}
Key* key(wcs x) {
    std::pair<std::tr1::unordered_map<uint, var>::iterator, bool>
		r =	keys.insert(std::make_pair(wcs2uint(x), null));
    if (r.second)
        r.first->second = new Key(wcs2uint(x));
    return static_cast<Key*>(r.first->second.ptr);
}
Key* key(uint x) {
    std::pair<std::tr1::unordered_map<uint, var>::iterator, bool>
		r = keys.insert(std::make_pair((x << 1) + 1, null));
    if (r.second)
        r.first->second = new Key((x << 1) + 1);
    return static_cast<Key*>(r.first->second.ptr);
}
Tuple* tuple(uint n) {
    if (n == 0)
        return 0;
	size_t m = sizeof(Tuple) + (n - 1) * sizeof(var);
    Tuple* r = reinterpret_cast<Tuple*>(new char[m]);
    memset(r, 0, m);
    r->id = Primary::Tuple;
    r->size = n;
    return r;
}
/*
#ifndef _MSC_VER
size_t wcslen(const wchar* x) {
	size_t n = 0;
	while(x[n++]);
	return n;
}
std::basic_istream<wchar> wcin(new std::stdio_istreambuf(stdin));
std::basic_ostream<wchar> wcout(new std::stdio_ostreambuf(stdout));
std::basic_ostream<wchar> wcerr(new std::stdio_ostreambuf(stderr));
std::basic_ostream<wchar> wclog(new std::stdio_ostreambuf(stdlog));
#endif
*/
}
