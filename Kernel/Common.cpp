#include <mU/Common.h>

namespace mU {
bool Enum::Inited = false;
void Enum::Init() {
    if (Inited)
        return;
	sys = SYM(root, System);
#define M(x) $.x = SYS(x);$.PrimaryName[++i] = $.x;
	{int i = -1;M(Null)M(Symbol)M(Key)M(Object)M(Tuple)}
#undef M
#define M(x) $.x = SYS(x);$.Attributes.insert($.x);
    M(Constant)M(Flat)M(HoldAll)M(HoldAllComplete)M(HoldFirst)
    M(HoldRest)M(Listable)M(Locked)M(NHoldAll)M(NHoldFirst)
    M(NHoldRest)M(NumericFunction)M(OneIdentity)M(Orderless)M(Protected)
    M(ReadProtected)M(SequenceHold)M(Stub)M(Temporary)
#undef M
#define M(x) $.x = SYS(x);$.Patterns.insert($.x);
    M(Optional)M(Condition)M(PatternTest)M(Blank)M(Pattern)M(Rule)M(RuleDelayed)
	M(Production)M(Alternatives)
#undef M
#define M(x) $.x = SYS(x);$.Objects.insert($.x);
    M(Integer)M(Rational)M(Real)M(String)
	M(Value)M(Assign)M(Method)M(Delayed)M(Match)
#undef M
#define M(x) $.x = SYS(x);
	M(True)M(False)M(Infinity)M(Fail)M(Function)
	M(Self)M(List)M(Sequence)M(Serial)M(Interface)
    M(Slot)M(Return)M(Continue)M(Break)M(Set)M(Map)
    M(Plus)M(Minus)M(Times)M(Power)M(Sqrt)M(Radical)
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
void Integer::print(wostream& o) const {
	o << mpz_get_str(0, 10, cast<Integer>().mpz);
}
void Rational::print(wostream& o) const {
	o << mpq_get_str(0, 10, cast<Rational>().mpq);
}
void Real::print(wostream& o) const {
	long exp;
	char *s = mpf_get_str(0, &exp, 10, static_cast<uint>(LOG_10_2 * 
		mpf_get_prec(cast<Real>().mpf)), 
		cast<Real>().mpf);
	wstring t(s, s + strlen(s));
	const wchar *buf = t.c_str();
	if (exp > 0) {
		size_t n = wcslen(buf);
		if (exp <= (int)n) {
			if (buf[0] == L'-') {
				o.write(buf, exp + 1);
				o << L'.' << buf + exp + 1;
			} else {
				o.write(buf, exp);
				o << L'.' << buf + exp;
			}
		} else {
			if (buf[0] == L'-')
				o << buf << wstring(exp + 1 - n, L'0') << L'.';
			else
				o << buf << wstring(exp - n, L'0') << L'.';
		}
	} else {
		if (buf[0] == L'-')
			o << L"-0." << wstring(-exp, L'0') << buf + 1;
		else
			o << L"0." << wstring(-exp, L'0') << buf;
	}
}
void String::print(wostream& o) const {
	mU::print(str.c_str(), o);
}
}