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
#include <cassert>
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

#undef CAPI
#undef API
#ifdef _MSC_VER
#pragma warning(push,1)
#pragma warning(disable:4819)
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
#define __W(x) L ## x
#define _W(x) __W(x)
using std::wcslen;
using std::wcin;
using std::wcout;
using std::wcerr;
using std::wclog;
/*
#else
#include <cstdint>
typedef char16_t wchar;
typedef uint_least16_t wint;
#define __W(x) u ## x
#define _W(x) __W(x)
API size_t wcslen(const wchar*);
API extern std::basic_istream<wchar> wcin;
API extern std::basic_ostream<wchar> wcout;
API extern std::basic_ostream<wchar> wcerr;
API extern std::basic_ostream<wchar> wclog;
#endif
*/
typedef const wchar* wcs;
typedef signed long sint;
#ifndef _WIN32
#define uint another_uint_type
#endif
typedef unsigned long uint;

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

class var;
class Symbol;
class Key;
class Object;
class Tuple;
API Tuple* tuple(uint);
class Var {
	friend class var;
	friend class Symbol;
	friend class Key;
	friend class Object;
	friend class Tuple;
	friend API Tuple* tuple(uint);
    uint id;
    explicit Var(uint $id) : id($id) {}
    Var* copy() {
        id += Primary::size;
        return this;
    }
    void pass(Var*& x) {
        id += Primary::size;
        if (x)
            x->destroy();
        x = this;
    }
    void destroy() {
        id -= Primary::size;
        if (id < Primary::size)
            Primary::ruin[id](this);
    }
public:
	void discard() {
		if (id >= Primary::size)
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
	template <class T>
	T& cast() const {
		return const_cast<T&>(static_cast<const T&>(*this));
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
#define WSTR(x) wstr(_W(#x))
typedef const Symbol* sym;
API extern std::tr1::unordered_map<sym, wcs> names;
API extern sym root, sys;
class Symbol : public Var {
public:
	API static void ruin(Var*);
    sym context;
    explicit Symbol(sym $context = root) : Var(Primary::Symbol), context($context) {}
	API wcs name() const;
    API long compare(sym) const;
    API sym clone(wcs = 0) const;
    API sym symbol(wcs) const;
#define SYM(x,y) x->symbol(WSTR(y))
#define SYS(x) SYM(sys,x)
    API var get(wcs) const;
    API bool set(wcs, const var&) const;
    API wstring toS(sym = 0) const;
	API void print(wostream& o = wcout) const;
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
	API void print(wostream& o = wcout) const;
private:
    ~Key() {}
};

class var {
public:
    Var* ptr;
    var() : ptr(0) {}
    var(const Var* $ptr) : ptr($ptr ? const_cast<Var*>($ptr)->copy() : 0) {}
    var(const var& x) : ptr(x.ptr ? x.ptr->copy() : 0) {}
    var& operator =(const Var* x) {
        if (x)
			const_cast<Var*>(x)->pass(ptr);
		else
			ptr = 0;
        return *this;
    }
    var& operator =(const var& x) {
		if (x.ptr)
			x.ptr->pass(ptr);
		else
			ptr = 0;
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
	bool isObject(sym) const;
    bool isTuple() const {
        return primary() == Primary::Tuple;
    }
	bool isTuple(const var&) const;
	bool isTuple(sym) const;
	template <class T>
	T& cast() const {
		return ptr->cast<T>();
	}
    sym symbol() const {
        return static_cast<sym>(ptr);
    }
    Key& key() const {
        return cast<Key>();
    }
    Object& object() const;
    Tuple& tuple() const;
    API var head() const;
    API var clone() const;
    API long compare(const var&) const;
	API bool equal(const var&) const;
    API size_t hash() const;
	API void print(wostream& o = wcout) const;
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
        return equal($other);
    }
    bool operator==(sym $other) const {
        return ptr == $other;
    }
    bool operator!=(const var& $other) const {
        return !equal($other);
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
} 

namespace std {
#ifndef _MSC_VER
namespace tr1 {
#endif
template<>
class hash<mU::var>	: public unary_function<mU::var, size_t> {
public:
	size_t operator()(const mU::var& x) const {
		return x.hash();
	}
};
#ifndef _MSC_VER
}
#endif
}

namespace mU {
inline bool islower(wchar x) {
	return x > 96 && x < 123;
}
inline bool isupper(wchar x) {
	return x > 64 && x < 91;
}
inline bool isalpha(wchar x) {
	return islower(x) || isupper(x) || x > 0x7F;
}
inline bool isdigit(wchar x) {
	return x > 47 && x < 58;
}
inline void print(wchar x, wostream& o = wcout) {
	if (isgraph(x) && isprint(x))
		o << x;
	else
		o << L"\\:" << std::hex
		<< ((x >> 12) & 0xF)
		<< ((x >> 8) & 0xF)
		<< ((x >> 4) & 0xF)
		<< (x & 0xF);        
}
inline void print(wcs x, wostream& o = wcout) {
	if (!x)
		return;
	while(*x)
		print(*x++, o);
}
inline void print(const wstring& x, wostream& o = wcout) {
	print(x.c_str(), o);
}
inline wostream& operator<<(wostream& o, const var& x) {
	x.print(o);
	return o;
}
typedef std::tr1::unordered_map<uint, var> Context;
API extern std::tr1::unordered_map<sym, Context> contexts;
API extern std::tr1::unordered_map<uint, var> keys;
API Key* key(wcs);
API Key* key(uint);

class Tuple : public Var {
public:
    API static void ruin(Var*);
    uint size;
    var tuple[1];
    API Tuple* clone() const;
	API long compare(const Tuple&) const;
	API bool equal(const Tuple&) const;
	API size_t hash() const;
	API void print(wostream& o = wcout) const;
    const var& operator[](uint i) const {
        return tuple[i];
    }
    var& operator[](uint i) {
        return tuple[i];
    }
private:
    ~Tuple() {}
};
inline bool var::isTuple(const var& x) const {
	return isTuple() && tuple()[0] == x;
}
inline bool var::isTuple(sym x) const {
	return isTuple() && tuple()[0] == x;
}
inline Tuple& var::tuple() const {
	return cast<Tuple>();
}
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
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
