#pragma once
#include "var.h"
#include <gmp.h>
#ifdef _MSC_VER
#pragma comment(lib,"../Lib/gmp.lib")
#endif

namespace mU {
struct Enum {
    API static bool Inited;
    API static void Init();

    std::tr1::unordered_set<sym> Attributes;
    sym
    Constant, Flat, HoldAll, HoldAllComplete, HoldFirst,
    HoldRest, Listable, Locked, NHoldAll, NHoldFirst,
    NHoldRest, NumericFunction, OneIdentity, Orderless, Protected,
    ReadProtected, SequenceHold, Stub, Temporary;

    std::tr1::unordered_set<sym> Patterns;
    sym
    Optional, Condition, PatternTest, Blank, Pattern, Rule, RuleDelayed,
	Production, Alternatives;

    sym
    Null, Symbol, Key, Object, Tuple, True, False, Infinity,
    Value, Assign, Method, Delayed, Match,
	Integer, Rational, Real, String,
    Fail, Self, List, Sequence, Serial, Interface,
    Slot, Return, Continue, Break, Set, Map,
    Plus, Minus, Times, Power, Sqrt, Radical;
} API extern $;
// TODO: 需要改成静态数组?
inline sym primary(const var& x) {
    switch (x.primary()) {
    case Primary::Null:
        return $.Null;
    case Primary::Symbol:
        return $.Symbol;
    case Primary::Key:
        return $.Key;
    case Primary::Object:
        return $.Object;
    case Primary::Tuple:
        return $.Tuple;
    }
    return 0;
}
inline sym boolean(bool x) {
    return x ? $.True : $.False;
}
inline sym kind(const Key& x) {
    switch (x.kind()) {
    case Key::String:
        return $.String;
    case Key::Integer:
        return $.Integer;
    }
    return 0;
}
template <class Iter>
inline Tuple* list(uint size, Iter begin, const var& h = $.List) {
    Tuple* r = tuple(size + 1);
    r->tuple[0] = h;
	for (uint i = 0; i < size; ++i, ++begin)
        r->tuple[i + 1] = *begin;
    return r;
}
template <class Func>
inline void map(Func f, Tuple& x) {
	for (uint i = 1; i < x.size; ++i)
		x[i] = f(x[i]);
}
API bool isCertain(const Tuple&);
inline bool isCertain(const var& x) {
	if (x.isTuple())
		return isCertain(x.tuple());
	return true;
}
inline bool isNumber(const Object& x) {
	return x.type == $.Integer
		|| x.type == $.Rational
		|| x.type == $.Real
		;
}
inline bool isNumber(const var& x) {
	return x.isObject() && isNumber(x.object());
}

class Kernel;

//重载assign实现f[x1]=y,f[x1][x2]=y等
//重载value实现f[x1],f[x1][x2]等
struct Assign : public Object {
    Assign() : Object($.Assign) {}
    virtual bool operator()(Kernel& k, const Tuple& x, const var& y) {
        return false;
    }
};

struct Value : public Object {
    Value() : Object($.Value) {}
    virtual var operator()(Kernel& k, Tuple& x) {
        return &x;
    }
};

struct Method : public Object {
    Method() : Object($.Method) {}
    virtual var operator()(Kernel& k) {
        return null;
    }
};

class Delayed : public Object {
public:
    Delayed(const var& $data) : Object($.Delayed), data($data) {}
    var data;
};

class Match : public Object {
public:
	Match() : Object($.Match) {}
	virtual bool operator()(Kernel& k, var& r, const var& x) {
		return false;
	}
};

/*!
* \brief
* 整数
*
* \remarks
* mpz_t
*/
class Integer : public Object {
public:
    mpz_t mpz;
    virtual ~Integer() {
        mpz_clear(mpz);
    }
    virtual int compare(const Object& x) const {
        return mpz_cmp(mpz, static_cast<const Integer&>(x).mpz);
    }
    virtual Integer* clone() const {
        Integer* r = new Integer();
        mpz_set(r->mpz, mpz);
        return r;
    }
    Integer() : Object($.Integer) {
        mpz_init(mpz);
    }
    Integer(uint x) : Object($.Integer) {
        mpz_init_set_ui(mpz, x);
    }
    Integer(sint x) : Object($.Integer) {
        mpz_init_set_si(mpz, x);
    }
    Integer(double x) : Object($.Integer) {
        mpz_init_set_d(mpz, x);
    }
    Integer(const char* x, int y) : Object($.Integer) {
        mpz_init_set_str(mpz, x, y);
    }
    uint toUI() const {
        return mpz_get_ui(mpz);
    }
    sint toSI() const {
        return mpz_get_si(mpz);
    }
    double toD() const {
        return mpz_get_d(mpz);
    }
};

/*!
* \brief
* 有理数
*
* \remarks
* mpq_t
*/
class Rational : public Object {
public:
    mpq_t mpq;
    virtual ~Rational() {
        mpq_clear(mpq);
    }
    virtual int compare(const Object& x) const {
        return mpq_cmp(mpq, static_cast<const Rational&>(x).mpq);
    }
    virtual Rational* clone() const {
        Rational* r = new Rational();
        mpq_set(r->mpq, mpq);
        return r;
    }
    Rational() : Object($.Rational) {
        mpq_init(mpq);
    }
    Rational(uint x, uint y) : Object($.Rational) {
        mpq_init(mpq);
        mpq_set_ui(mpq, x, y);
    }
    Rational(sint x, uint y) : Object($.Rational) {
        mpq_init(mpq);
        mpq_set_si(mpq, x, y);
    }
    Rational(double x) : Object($.Rational) {
        mpq_init(mpq);
        mpq_set_d(mpq, x);
    }
    Rational(const char* x, int y) : Object($.Rational) {
        mpq_init(mpq);
        mpq_set_str(mpq, x, y);
    }
    double toD() const {
        return mpq_get_d(mpq);
    }
};

#define LOG_2_10 3.3219280948873623478703194294894
#define LOG_10_2 0.3010299956639811952137388947245

/*!
* \brief
* 实数
*
* \remarks
* mpf_t
*/
class Real : public Object {
public:
    mpf_t mpf;
    virtual ~Real() {
        mpf_clear(mpf);
    }
    virtual int compare(const Object& x) const {
        return mpf_cmp(mpf, static_cast<const Real&>(x).mpf);
    }
    virtual Real* clone() const {
        Real* r = new Real(mpf_get_prec(mpf));
        mpf_set(r->mpf, mpf);
        return r;
    }
    Real(uint n = 0) : Object($.Real) {
        mpf_init2(mpf, n ? static_cast<uint>(LOG_2_10 * n) : 
			mpf_get_default_prec());
    }
    Real(uint x, uint n) : Object($.Real) {
		mpf_init2(mpf, n ? static_cast<uint>(LOG_2_10 * n) : 
			mpf_get_default_prec());
		mpf_set_ui(mpf, x);
    }
    Real(sint x, uint n) : Object($.Real) {
		mpf_init2(mpf, n ? static_cast<uint>(LOG_2_10 * n) : 
			mpf_get_default_prec());
		mpf_set_si(mpf, x);
    }
    Real(double x, uint n) : Object($.Real) {
		mpf_init2(mpf, n ? static_cast<uint>(LOG_2_10 * n) : 
			mpf_get_default_prec());
		mpf_set_d(mpf, x);
    }
    Real(const char* x, int y, uint n) : Object($.Real) {
		mpf_init2(mpf, n ? static_cast<uint>(LOG_2_10 * n) : 
			mpf_get_default_prec());
		mpf_set_str(mpf, x, y);
    }
	uint prec() const {
		return static_cast<uint>(LOG_10_2 * mpf_get_prec(mpf));
	}
    uint toUI() const {
        return mpf_get_ui(mpf);
    }
    sint toSI() const {
        return mpf_get_si(mpf);
    }
    double toD() const {
        return mpf_get_d(mpf);
    }
};

inline double toD(const Object& x) {
	if (x.type == $.Integer)
		return static_cast<const Integer&>(x).toD();
	if (x.type == $.Rational)
		return static_cast<const Rational&>(x).toD();
	if (x.type == $.Real)
		return static_cast<const Real&>(x).toD();
	return 0.0;
}
inline int cmpD(const Object& x, double a, double b = 1.0) {
	if (x.type == $.Integer)
		return mpz_cmp_si(static_cast<const Integer&>(x).mpz, 
		static_cast<long>(a / b));
	if (x.type == $.Rational)
		return mpq_cmp_si(static_cast<const Rational&>(x).mpq, static_cast<long>(a), 
		static_cast<long>(b));
	if (x.type == $.Real)
		return mpf_cmp_d(static_cast<const Real&>(x).mpf, a / b);
	return 0;
}

/*!
* \brief
* 字符串
*
* \remarks
* wstring
*/
class String : public Object {
public:
    wstring str;
    virtual int compare(const Object& x) const {
        return str.compare(static_cast<const String&>(x).str);
    }
    virtual String* clone() const {
        String* r = new String();
        r->str = str;
        return r;
    }
    String() : Object($.String) {}
    String(wcs x) : Object($.String) {
        str.assign(x);
    }
    String(const wstring& x) : Object($.String) {
        str.assign(x);
    }
    uint toUI() const {
        return wcs2uint(wstr(str.c_str()));
    }
    wcs toS() const {
        return str.c_str();
    }
};
}