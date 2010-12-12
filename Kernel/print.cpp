#include <mU/utils.h>
#include <mU/Kernel.h>

namespace mU {
void Kernel::print(sym x, wostream& o) const {
	if (!x) {
		o << _W("Null");
		return;
	}
	sym c = x->context;
	if (c != context() &&
		std::find(contextPath().begin(),
		contextPath().end(), c) ==
		contextPath().end())
		mU::print(x->toS(context()), o);
	else {
		wcs s = x->name();
		if (s)
			mU::print(s, o);
		else
			o << static_cast<const void*>(x);
	}
}
void Kernel::print(const Object& x, wostream& o) const {
	if (isNumber(x)) {
		x.print(o);
		return;
	}
	if (x.type == $.String) {
		o << _W('"');
		x.print(o);
		o << _W('"');
		return;
	}
	print(x.type, o);
	o << _W('`') << static_cast<const void*>(&x);
}
void Kernel::print(const Tuple& x, wostream& o) const {
    print(x[0], o);
    o << _W('[');
    if (x.size > 1) {
        print(x[1], o);
        for (uint i = 2; i < x.size; ++i) {
            o << _W(',');
            print(x[i], o);
        }
    }
    o << _W(']');
}
void Kernel::print(const var& x, wostream& o) const {
    switch (x.primary()) {
	case Primary::Null:
		o << _W("Null");
		return;
    case Primary::Symbol:
        print(x.symbol(), o);
        return;
    case Primary::Key:
        x.key().print(o);
        return;
    case Primary::Object:
        print(x.object(), o);
        return;
    case Primary::Tuple:
        print(x.tuple(), o);
        return;
    }
}
Kernel& Kernel::logging(wcs x) {
	*log << _W('[') << date() << _W("][");
	mU::print(x, *log);
	*log << _W("]: ");
	return *this;
}
}