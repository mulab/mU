#include <mU/Common.h>
#include <mU/Kernel.h>

namespace mU {
void Kernel::print(wostream& o, wchar x) {
    if (!iswprint(x))
        o << L"\\:" << std::hex
        << ((x >> 12) & 0xF)
        << ((x >> 8) & 0xF)
        << ((x >> 4) & 0xF)
        << (x & 0xF);
    else
        o << x;
}
void Kernel::print(wostream& o, wcs x) {
    uint n = wcslen(x);
    for (uint i = 0; i < n; ++i)
        print(o, x[i]);
}
void Kernel::print(wostream& o, sym x) {
    sym c = x->context;
    if (c != context() &&
            std::find(contextPath().begin(),
                      contextPath().end(), c) ==
            contextPath().end()) {
        wstring s;
        while (c && c != context()) {
            s = _W("`") + s;
            s = c->name() + s;
            c = c->context;
        }
        if (c == context())
            o << _W('`');
        print(o, s.c_str());
    }
    wcs s = x->name();
    if (s)
        print(o, s);
    else
        o << static_cast<const void*>(x);
}
void Kernel::print(wostream& o, const Key& x) {
	if (x) {
		switch (x.kind()) {
		case Key::String: {
			wcs s = x.toS();
			if (isupper(s[0]))
				o << _W('#');
			print(o, s);
		}
			break;
		case Key::Integer:
			o << _W('#') << x.toUI();
			break;
		}
	} else
		o << _W('#');
}
void Kernel::print(wostream& o, const Object& x) {
    if (x.type == $.Integer) {
        o << mpz_get_str(0, 10, static_cast<const Integer&>(x).mpz);
        return;
    }
    if (x.type == $.Rational) {
        o << mpq_get_str(0, 10, static_cast<const Rational&>(x).mpq);
        return;
    }
    if (x.type == $.Real) {
        long exp;
        char *s = mpf_get_str(0, &exp, 10, static_cast<uint>(LOG_10_2 * 
			mpf_get_prec(static_cast<const Real&>(x).mpf)), 
			static_cast<const Real&>(x).mpf);
        wstring t(s, s + strlen(s));
        const wchar *buf = t.c_str();
        if (exp == 0)
            o << L"0." << buf;
        else if (exp > 0) {
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
        return;
    }
    if (x.type == $.String) {
        o << L'"';
        print(o, static_cast<const String&>(x).str.c_str());
        o << L'"';
        return;
    }
    print(o, x.type);
    o << _W('`') << static_cast<const void*>(&x);
}
void Kernel::print(wostream& o, const Tuple& x) {
    print(o, x[0]);
    o << L'[';
    if (x.size > 1) {
        print(o, x[1]);
        for (uint i = 2; i < x.size; ++i) {
            o << L',';
            print(o, x[i]);
        }
    }
    o << L']';
}
void Kernel::print(wostream& o, const var& x) {
    switch (x.primary()) {
    case Primary::Symbol:
        print(o, x.symbol());
        break;
    case Primary::Key:
        print(o, x.key());
        break;
    case Primary::Object:
        print(o, x.object());
        break;
    case Primary::Tuple:
        print(o, x.tuple());
        break;
    }
}
}