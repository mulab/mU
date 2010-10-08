#pragma once
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <cwctype>
#include <cwchar>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cmath>
#include <ctime>
#include <locale>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <list>
#include <algorithm>
#include <functional>
#include <iterator>
#ifdef _MSC_VER
#include <unordered_set>
#include <unordered_map>
#else
#include <tr1/unordered_set>
#include <tr1/unordered_map>
#endif

#undef API
#ifdef _MSC_VER
#pragma warning(disable:4819)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#define CAPI extern "C" __declspec(dllexport)
#ifdef KERNEL_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define CAPI extern "C"
#define API
#endif

namespace mU {
//#ifdef _MSC_VER
typedef wchar_t wchar;
typedef wint_t wint;
#define WSTR(x) wstr(L## #x)
#define _W(x) L##x
using std::wcslen;
using std::wcin;
using std::wcout;
using std::wcerr;
/*
#else
#include <cstdint>
typedef char16_t wchar;
typedef uint_least16_t wint;
#define WSTR(x) u## #x
#define _W(x) u##x
API size_t wcslen(const wchar*);
API extern std::basic_istream<wchar> wcin;
API extern std::basic_ostream<wchar> wcout;
API extern std::basic_ostream<wchar> wcerr;
#endif
*/
typedef const wchar* wcs;
typedef signed long sint;
#ifdef _MSC_VER
typedef unsigned long uint;
#endif

using std::string;
typedef std::basic_string<wchar> wstring;
using std::istream;
typedef std::basic_istream<wchar> wistream;
using std::ostream;
typedef std::basic_ostream<wchar> wostream;
using std::iostream;
typedef std::basic_iostream<wchar> wiostream;
using std::istringstream;
typedef std::basic_istringstream<wchar> wistringstream;
using std::ostringstream;
typedef std::basic_ostringstream<wchar> wostringstream;
using std::stringstream;
typedef std::basic_stringstream<wchar> wstringstream;
using std::ifstream;
typedef std::basic_ifstream<wchar> wifstream;
using std::ofstream;
typedef std::basic_ofstream<wchar> wofstream;
using std::fstream;
typedef std::basic_fstream<wchar> wfstream;

using std::cin;
using std::cout;
using std::cerr;

using std::ios;
typedef std::basic_ios<wchar> wios;
using std::endl;

class Var;
class var;
class Symbol;
class Key;
class Object;
class Tuple;
typedef const Symbol* sym;

namespace Primary {
enum Type {
    Null = -1,
	Object,
	Key,
	Symbol,
    Tuple
};
const uint size = 4;
const uint mask = size - 1;
API extern void(* const ruin[size])(Var*);
}

class Var {
public:
    uint id;
    explicit Var(uint $id) : id($id) {}
    Var* copy() {
        id += Primary::size;
        return this;
    }
    Var* copy(Var* x) {
        id += Primary::size;
        if (x)
            x->destroy();
        return this;
    }
    void destroy() {
        id -= Primary::size;
        if (id < Primary::size)
            Primary::ruin[id](this);
    }
    uint ref() const {
        return id / Primary::size;
    }
    Primary::Type primary() const {
        return static_cast<Primary::Type>(id & Primary::mask);
    }
};

inline uint wcs2uint(wcs x) {
    return reinterpret_cast<uint>(x);
}
inline wcs uint2wcs(uint x) {
    return reinterpret_cast<wcs>(x);
}
API extern std::tr1::unordered_set<wstring> wstrs;
inline wcs wstr(wcs x) {
    return wstrs.insert(x).first->c_str();
}
API extern std::tr1::unordered_map<sym, wcs> names;
API extern sym root, sys;
class Symbol : public Var {
public:
    API static void ruin(Var*);
    sym context;
    API wcs name() const;
    API int compare(sym) const;
    API sym clone(wcs = 0) const;
    API sym symbol(wcs) const;
#define SYM(x,y) x->symbol(WSTR(y))
#define SYS(x) SYM(sys,x)
    API var get(wcs) const;
    API bool set(wcs, const var&) const;
    explicit Symbol(sym $context = root) : Var(Primary::Symbol), context($context) {}
private:
    ~Symbol() {}
};

class Key : public Var {
public:
    API static void ruin(Var*);
    uint key;
    enum Kind {
        String,
        Integer
    };
    explicit Key(uint $key) : Var(Primary::Key), key($key) {}
    Kind kind() const {
        return static_cast<Kind>(key & 1);
    }
	wcs toS() const {
        return uint2wcs(key);
    }
	uint toUI() const {
        return (key >> 1);
    }
	operator bool() const {
		return key != 0;
	}
private:
    ~Key() {}
};

class Object : public Var {
public:
    API static void ruin(Var*);
    sym type;
    explicit Object(sym $type = 0) : Var(Primary::Object), type($type) {}
    virtual ~Object() {}
    virtual int compare(const Object& $other) const {
        return reinterpret_cast<int>(this) - reinterpret_cast<int>(&$other);
    }
    virtual Object* clone() const {
        return new Object(*this);
    }
};

class var {
public:
    Var* ptr;
    var() : ptr(0) {}
    var(const Var* $ptr) : ptr($ptr ? const_cast<Var*>($ptr)->copy() : 0) {}
    var(const var& x) : ptr(x.ptr ? x.ptr->copy() : 0) {}
    var& operator =(const Var* x) {
        ptr = x ? const_cast<Var*>(x)->copy(ptr) : 0;
        return *this;
    }
    var& operator =(const var& x) {
        ptr = x.ptr ? x.ptr->copy(ptr) : 0;
        return *this;
    }
    ~var() {
        if (ptr) ptr->destroy();
    }
    uint ref() const {
        return ptr ? ptr->ref() : 0;
    }
    Primary::Type primary() const {
        return ptr ? ptr->primary() : Primary::Null;
    }
    bool isSymbol() const {
        return primary() == Primary::Symbol;
    }
    bool isKey() const {
        return primary() == Primary::Key;
    }
	bool isKey(Key::Kind x) const {
		return isKey() && key().kind() == x;
	}
    bool isObject() const {
        return primary() == Primary::Object;
    }
	bool isObject(sym x) const {
		return isObject() &&  object().type == x;
	}
    bool isTuple() const {
        return primary() == Primary::Tuple;
    }
	API bool isTuple(const var&) const;
	API bool isTuple(sym) const;
    sym symbol() const {
        return static_cast<sym>(ptr);
    }
    Key& key() const {
        return static_cast<Key&>(*ptr);
    }
    Object& object() const {
        return static_cast<Object&>(*ptr);
    }
    API Tuple& tuple() const;
    API var head() const;
    API var clone() const;
    API int compare(const var&) const;
    API bool depend(const var&) const;
    template <class T>
    var subs(const T& m) const;
    operator bool() const {
        return ptr != 0;
    }
    bool operator<(const var& $other) const {
        return this->compare($other) < 0;
    }
    bool operator<=(const var& $other) const {
        return this->compare($other) <= 0;
    }
    bool operator==(const var& $other) const {
        return this->compare($other) == 0;
    }
    bool operator==(sym $other) const {
        return ptr == $other;
    }
    bool operator!=(const var& $other) const {
        return this->compare($other) != 0;
    }
    bool operator!=(sym $other) const {
        return ptr != $other;
    }
    bool operator>(const var& $other) const {
        return this->compare($other) > 0;
    }
    bool operator>=(const var& $other) const {
        return this->compare($other) >= 0;
    }
};
API extern const var null;

class Tuple : public Var {
public:
    API static void ruin(Var*);
    uint size;
    var tuple[1];
    API Tuple* clone() const;
    const var& operator[](uint i) const {
        return tuple[i];
    }
    var& operator[](uint i) {
        return tuple[i];
    }
private:
    ~Tuple() {}
};

typedef std::tr1::unordered_map<uint, var> Context;
API extern std::tr1::unordered_map<sym, Context> contexts;
API extern std::tr1::unordered_map<uint, var> keys;
API Key* key(wcs);
API Key* key(uint);
API Tuple* tuple(uint);
inline Tuple* tuple(const var& a) {
    Tuple* r = tuple(1);
    r->tuple[0] = a;
    return r;
}
inline Tuple* tuple(const var& a, const var& b) {
    Tuple* r = tuple(2);
    r->tuple[0] = a;
    r->tuple[1] = b;
    return r;
}
inline Tuple* tuple(const var& a, const var& b, const var& c) {
    Tuple* r = tuple(3);
    r->tuple[0] = a;
    r->tuple[1] = b;
    r->tuple[2] = c;
    return r;
}
inline Tuple* tuple(const var& a, const var& b, const var& c, const var& d) {
    Tuple* r = tuple(4);
    r->tuple[0] = a;
    r->tuple[1] = b;
    r->tuple[2] = c;
    r->tuple[3] = d;
    return r;
}
template <class Iter>
inline Tuple* tuple(uint size, Iter begin) {
	Tuple* r = tuple(size);
	for (uint i = 0; i < size; ++i, ++begin)
		r->tuple[i] = *begin;
	return r;
}
template <class T>
inline T& cast(const var& x) {
    return static_cast<T&>(x.object());
}
template <class T>
var var::subs(const T& m) const {
    typename T::const_iterator
		iter = m.find(*this);
    if (iter != m.end())
        return iter->second;
    if (isTuple()) {
        Tuple* r = tuple().clone();
        for (uint i = 0; i < tuple().size; ++i)
            r->tuple[i] = tuple()[i].subs(m);
        return r;
    }
    return *this;
}
typedef std::map<var, var> Map;
typedef std::multimap<var, var> MMap;
}
