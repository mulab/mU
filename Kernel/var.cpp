#include <mU/Object.h>

namespace mU {
void Key::ruin(Var* x) {
	delete static_cast<Key*>(x);
}
long Key::compare(const Key& x) const {
	long r = kind() - x.kind();
	if (r)
		return r;
	switch (kind()) {
	case Key::Integer:
		return toUI() - x.toUI();
	case Key::String:
		return wcscmp(toS(), x.toS());
	}
	return 0;
}
void Key::print(wostream& o) const {
	o << _W('#');
	switch (kind()) {
	case Key::Integer:
		o << toUI();
		break;
	case Key::String:
		mU::print(toS(), o);
		break;
	}
}
std::unordered_map<uint, var> keys;
Key* key(wcs x) {
	std::pair<std::unordered_map<uint, var>::iterator, bool>
		r =	keys.insert(std::make_pair(reinterpret_cast<uint>(x), null));
	if (r.second)
		r.first->second = new Key(reinterpret_cast<uint>(x));
	return static_cast<Key*>(r.first->second.ptr);
}
Key* key(uint x) {
	std::pair<std::unordered_map<uint, var>::iterator, bool>
		r = keys.insert(std::make_pair((x << 1) + 1, null));
	if (r.second)
		r.first->second = new Key((x << 1) + 1);
	return static_cast<Key*>(r.first->second.ptr);
}
std::unordered_set<wstring> wstrs;
std::unordered_map<sym, wcs> names;
std::unordered_map<sym, Context> contexts;
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
    std::unordered_map<sym, wcs>::const_iterator
    iter = names.find(this);
    if (iter != names.end())
        return iter->second;
    return 0;
}
sym Symbol::clone(wcs w) const {
    sym r = new Symbol(this);
    if (w) {
        names[r] = w;
        contexts[this][reinterpret_cast<uint>(w)] = r;
    } else
        contexts[this][reinterpret_cast<uint>(r)] = r;
    return r;
}
long Symbol::compare(sym x) const {
	if (!this)
		return x ? -1L : 0L;
	if (!x)
		return 1L;
	int r = context->compare(x->context);
	if (r)
		return r;
	wcs a = name();
	wcs b = x->name();
	if (!a)
		return b ? -1L : 0L;
	if (!b)
		return 1L;
	return wcscmp(a, b);
}
void Symbol::print(wostream& o) const {
	mU::print(toS(), o);
}
sym Symbol::symbol(wcs x) const {
    std::pair<Context::iterator, bool>
    r = contexts[this].insert(std::make_pair(reinterpret_cast<uint>(x), null));
    if (!r.second)
        return r.first->second.isSymbol() ? r.first->second.symbol() : 0;
    sym t = clone(x);
    r.first->second = t;
    return t;
}
var Symbol::get(wcs x) const {
    std::unordered_map<sym, Context>::const_iterator
    iter = contexts.find(this);
    if (iter != contexts.end()) {
        Context::const_iterator
        iter2 = iter->second.find(reinterpret_cast<uint>(x));
        if (iter2 != iter->second.end())
            return iter2->second;
    }
    return null;
}
bool Symbol::set(wcs x, const var& y) const {
    if (y)
        contexts[this][reinterpret_cast<uint>(x)] = y;
    else
        contexts[this].erase(reinterpret_cast<uint>(x));
    return true;
}
wstring Symbol::toS(sym x) const {
	wstring s;
	if (x == this)
		return s;
	if (context) {
		s += context->toS(x);
		s += _W('`');
	}
	wcs c = name();
	if (c)
		s += c;
	else {
		wostringstream oss;
		oss << static_cast<const void*>(this);
		s += oss.str();
	}
	return s;
}
void Object::ruin(Var* x) {
	delete static_cast<Object*>(x);
}
namespace Primary {
void(* const ruin[size])(Var*) = {
    Object::ruin,
	Key::ruin,
	Symbol::ruin,    
    Tuple::ruin
};
}
bool var::ahead::operator()(const var& x, const var& y) const {
   long r = x.primary() - y.primary();
    if (r)
        return r < 0;
    switch (x.primary()) {
    case Primary::Object:
        if (x.object().type == y.object().type)
			return x.object().compare(y.object()) < 0;
		return x.object().type < y.object().type;
	case Primary::Key:
	case Primary::Symbol:
		return x.ptr < y.ptr;
    case Primary::Tuple: {
		r = x.tuple().size - y.tuple().size;
		if (r)
			return r < 0;
		for (uint i = 0; i < x.tuple().size; ++i)
			if ((*this)(x.tuple()[i], y.tuple()[i]))
				return true;
	}
		return false;						 
    }
    return 0;	
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
long var::compare(const var& x) const {
    long r = primary() - x.primary();
    if (r)
        return r;
    switch (primary()) {
    case Primary::Symbol:
        return symbol()->compare(x.symbol());
    case Primary::Key:
        return key().compare(x.key());
    case Primary::Object: {
        long r = object().type->compare(x.object().type);
        if (r)
            return r;
    }
		return object().compare(x.object());
    case Primary::Tuple:
		return tuple().compare(x.tuple());
    }
    return 0;
}
bool var::equal(const var& x) const {
	if (primary() != x.primary())
		return false;
	switch (primary()) {
	case Primary::Symbol:
		return symbol() == x.symbol();
	case Primary::Key:
		return key().key == x.key().key;
	case Primary::Object:
		if (object().type != x.object().type)
			return false;
		return object().equal(x.object());
	case Primary::Tuple:
		return tuple().equal(x.tuple());
	}
	// Primary::Null
	return true;
}
size_t var::hash() const {
	switch (primary()) {
	case Primary::Symbol:
		return reinterpret_cast<size_t>(symbol());
	case Primary::Key:
		return static_cast<size_t>(key().key);
	case Primary::Object:
		return object().hash();
	case Primary::Tuple:
		return tuple().hash();
	}
	return 0;
}
void var::print(wostream& o) const {
	switch (primary()) {
	case Primary::Symbol:
		symbol()->print(o);
		return;
	case Primary::Key:
		key().print(o);
		return;
	case Primary::Object:
		object().print(o);
		return;
	case Primary::Tuple:
		tuple().print(o);
		return;
	}
}
void Tuple::ruin(Var* x) {
	Tuple* t = static_cast<Tuple*>(x);
	for (uint i = 0; i < t->size; ++i)
		t->tuple[i].~var();
	delete reinterpret_cast<char*>(x);
}
Tuple* Tuple::clone() const {
    Tuple* r = mU::tuple(size);
    for (uint i = 0; i < size; ++i)
        r->tuple[i] = (*this)[i];
    return r;
}
long Tuple::compare(const Tuple& x) const {
	long r = tuple[0].compare(x.tuple[0]);
	if (r)
		return r;
	if (r = size - x.size)
		return r;
	for (uint i = 1; i < size; ++i)
		if (r = tuple[i].compare(x.tuple[i]))
			break;
	return r;
}
bool Tuple::equal(const Tuple& x) const {
	if (size != x.size)
		return false;
	for (uint i = 0; i < size; ++i)
		if (!tuple[i].equal(x.tuple[i]))
			return false;
	return true;
}
size_t Tuple::hash() const {
	size_t r = 2166136261U;
	const var* iter = tuple;
	const var* end = tuple + size;

	while (iter < end) {
		r = 16777619U * r ^ iter->hash();
		++iter;
	}
	return r;
}
void Tuple::print(wostream& o) const {
	tuple[0].print(o);
	o << _W('[');
	if (size > 1) {
		tuple[1].print(o);
		for (uint i = 2; i < size; ++i) {
			o << _W(',');
			tuple[i].print(o);
		}
	}
	o << _W(']');
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
int wcscmp(wcs a, wcs b) {
	int r;
	while (*a && *b)
		if (r = *a - *b)
			return r;
	if (!*a)
		return *b ? -1L : 0L;
	return *b;
}
size_t wcslen(wcs s) {
	wcs eos = s;
	while( *eos++ ) ;
	return( (size_t)(eos - s - 1) );
}
std::basic_istream<wchar> wcin(new std::stdio_istreambuf(stdin));
std::basic_ostream<wchar> wcout(new std::stdio_ostreambuf(stdout));
std::basic_ostream<wchar> wcerr(new std::stdio_ostreambuf(stderr));
std::basic_ostream<wchar> wclog(new std::stdio_ostreambuf(stdlog));
#endif
*/
}
