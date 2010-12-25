#include <mU/Common.h>
#ifdef _MSC_VER
void (*__gmp_free_func)(void*, size_t);
#endif

namespace mU {
bool Enum::Inited = false;
void Enum::Init() {
    if (Inited)
        return;
#ifdef _MSC_VER
	mp_get_memory_functions(0, 0, &__gmp_free_func);
#endif
	sys = SYM(root, System);
#define M(x) $.x = SYS(x);$.PrimaryName[++i] = $.x;
	{int i = -1;M(Null)M(Symbol)M(Key)M(Object)M(Tuple)}
#undef M
#define M(x) $.x = SYS(x);$.Attributes.insert($.x);
    M(Constant)
	M(Flat)
	M(HoldAll)
	M(HoldAllComplete)
	M(HoldFirst)
    M(HoldRest)
	M(Listable)
	M(Locked)
	M(NHoldAll)
	M(NHoldFirst)
    M(NHoldRest)
	M(NumericFunction)
	M(OneIdentity)
	M(Orderless)
	M(Protected)
    M(ReadProtected)
	M(SequenceHold)
	M(Stub)
	M(Temporary)
#undef M
#define M(x) $.x = SYS(x);$.Patterns.insert($.x);
	M(Alternatives)
	M(Blank)
	M(Condition)
	M(Optional)
	M(Pattern)
	M(PatternTest)
	M(Production)
	M(Rule)
	M(RuleDelayed)
#undef M
#define M(x) $.x = SYS(x);$.Objects.insert($.x);
    M(Integer)M(Rational)M(Real)M(String)
#undef M
#define M(x) $.x = SYS(x);
	M(Assign) 
	M(Break) 
	M(Continue) 
	M(Delayed)
	M(Fail) 
	M(False) 
	M(Function)
	M(Infinity)
	M(Interface)
	M(List)
	M(Map)
	M(Match)
	M(Method)
	M(Minus) 
	M(Plus)
	M(Power) 
	M(Radical)
	M(Return) 	
	M(Self) 
	M(Sequence) 
	M(Serial) 
	M(Set)
	M(Slot) 
	M(Sqrt) 
	M(Times)
	M(True)
	M(Value)
#undef M
    Inited = true;
}
Enum $;
bool isCertain(const Tuple& x) {
	if (x[0].isSymbol() && $.Patterns.count(x[0].symbol()))
		return false;
	for (uint i = 0; i < x.size; ++i)
		if (!isCertain(x[i]))
			return false;
	return true;
}
namespace {
inline size_t abs(int x) {
	return x < 0 ? -x : x;
}
inline size_t hash(mp_limb_t* begin, mp_limb_t* end) {
	size_t r = 2166136261U;
	while (begin < end) {
		r = 16777619U * r ^ *begin;
		++begin;
	}
	return r;
}
}
size_t mpz_hash(mpz_srcptr x) {
	return hash(x->_mp_d, x->_mp_d + abs(x->_mp_size));
}
size_t mpq_hash(mpq_srcptr x) {
	size_t r = mpz_hash(mpq_numref(x));
	r = 16777619U * r ^ mpz_hash(mpq_denref(x));
	return r;
}
size_t mpf_hash(mpf_srcptr x) {
	size_t r = hash(x->_mp_d, x->_mp_d + abs(x->_mp_size));
	r = 16777619U * r ^ x->_mp_exp;
	return r;
}
void Integer::print(wostream& o) const {
	char* buf = mpz_get_str(0, 10, cast<Integer>().mpz);
	o << buf;
	(*__gmp_free_func)(buf, strlen(buf) + 1);
}
void Rational::print(wostream& o) const {
	char* buf = mpq_get_str(0, 10, cast<Rational>().mpq);
	o << buf;
	(*__gmp_free_func)(buf, strlen(buf) + 1);
}
void Real::print(wostream& o) const {
	long exp;
	char* buf = mpf_get_str(0, &exp, 10, static_cast<uint>(LOG_10_2 * 
		mpf_get_prec(cast<Real>().mpf)), 
		cast<Real>().mpf);
	wstring t(buf, buf + strlen(buf));
	(*__gmp_free_func)(buf, strlen(buf) + 1);
	size_t n = t.size();
	wcs s = t.c_str();
	if (exp > 0) {
		if (exp <= (int)n) {
			if (s[0] == _W('-')) {
				o.write(s, exp + 1);
				o << _W('.') << s + exp + 1;
			} else {
				o.write(s, exp);
				o << _W('.') << s + exp;
			}
		} else {
			o << s;
			if (s[0] == _W('-'))
				o << wstring(exp + 1 - n, _W('0'));
			else
				o << wstring(exp - n, _W('0'));
			 o << _W('.');
		}
	} else {
		if (s[0] == _W('-'))
			o << _W("-0.") << wstring(-exp, _W('0')) << s + 1;
		else
			o << _W("0.") << wstring(-exp, _W('0')) << s;
	}
}
void String::print(wostream& o) const {
	mU::print(str.c_str(), o);
}
}