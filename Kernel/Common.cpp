#include <mU/Common.h>

namespace mU {
bool Enum::Inited = false;
void Enum::Init() {
    if (Inited)
        return;
	sys = SYM(root, System);
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
#define M(x) $.x = SYS(x);
    M(Null)M(Symbol)M(Key)M(Object)M(Tuple)M(True)M(False)M(Infinity)
    M(Value)M(Assign)M(Method)M(Delayed)M(Match)
	M(Integer)M(Rational)M(Real)M(String)
    M(Fail)M(Self)M(List)M(Sequence)M(Serial)M(Interface)
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
}