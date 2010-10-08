#include <mU/Common.h>
#include <mU/Kernel.h>
#include <mU/Parser.h>

namespace mU {
void Grammar::print(Kernel& k, wostream& o, wchar x) {
    if (x >= 0x0080) {
        std::tr1::unordered_map<wint, uint>::const_iterator
        iter = unicode.find((wint)x);
        if (iter != unicode.end()) {
            o << L"\\[" << character[iter->second].named << L']';
            return;
        }
    }
    k.print(o, x);
}
void Grammar::print(Kernel& k, wostream& o, wcs x) {
    uint n = wcslen(x);
    for (uint i = 0; i < n; ++i)
        print(k, o, x[i]);
}
void Grammar::print(Kernel& k, wostream& o, sym x) {
    sym c = x->context;
    if (c != k.context() &&
            std::find(
                k.contextPath().begin(),
                k.contextPath().end(), c) ==
            k.contextPath().end()) {
        wstring s;
        while (c && c != k.context()) {
            s = _W("`") + s;
            s = c->name() + s;
            c = c->context;
        }
        if (c == k.context())
            o << _W("`");
        print(k, o, s.c_str());
    }
    wcs s = x->name();
    if (s)
        print(k, o, s);
    else
        o << static_cast<const void*>(x);
}
void Grammar::print(Kernel& k, wostream& o, const Key& x) {
	if (x) {
		switch (x.kind()) {
		case Key::String:
			print(k, o, x.toS());
			break;
		case Key::Integer:
			o << _W('#') << x.toUI();
			break;
		}
	} else
		o << _W('#');
}
void Grammar::print(Kernel& k, wostream& o, const Object& x) {
    if (x.type == $.String) {
        print(k, o, static_cast<const String&>(x).str.c_str());
        return;
    }
	if (x.type == $.Delayed) {
		o << _W('&');
		print(k, o, static_cast<const Delayed&>(x).data);
		return;
	}
    k.print(o, x);
}
void Grammar::print(Kernel& k, wostream& o, const var& x, uint y) {
    switch (x.primary()) {
    case Primary::Symbol:
        print(k, o, x.symbol());
        break;
    case Primary::Key:
        print(k, o, x.key());
        break;
    case Primary::Object:
        print(k, o, x.object());
        break;
    case Primary::Tuple:
        print(k, o, x.tuple(), y);
        break;
    }
}
void Grammar::print(Kernel& k, wostream& o, const Tuple& x, uint y) {
    if (x[0].isSymbol()) {
        sym h = x[0].symbol();
        if (h == $.List) {
            o << L'{';
            if (x.size > 1) {
                print(k, o, x[1]);
                for (uint i = 2; i < x.size; ++i) {
                    o << L',';
                    print(k, o, x[i]);
                }
            }
            o << L'}';
            return;
        }
        if (h == $.Serial) {
            print(k, o, x[1]);
            for (uint i = 2; i < x.size; ++i) {
                o << L';';
                print(k, o, x[i]);
            }
            return;
        }
		if (x.size == 1) {
			if (h == $.Blank) {
				o << L'_';
				return;
			}
		} else if (x.size >= 2) {
            if (x.size == 2) {
                if (h == $.Blank && x[1].isSymbol()) {
                    o << L'_';
                    print(k, o, x[1].symbol());
                    return;
                }
                std::tr1::unordered_map<sym, uint>::const_iterator iter = postfixSymbol.find(h);
                if (iter != postfixSymbol.end()) {
					Parser::Oper &op = oper[iter->second];
                    if (op.prec < y) {
                        o << L'(';
                        print(k, o, x[1], op.prec);
                        o << op.show;
                        o << L')';
                    } else {
                        print(k, o, x[1], op.prec);
                        o << op.show;
                    }
                    return;
                }
                iter = prefixSymbol.find(h);
                if (iter != prefixSymbol.end()) {
                    Parser::Oper &op = oper[iter->second];
                    if (op.prec < y) {
                        o << L'(';
                        o << op.show;
                        print(k, o, x[1], h == $.Minus ? y : op.prec);
                        o << L')';
                    } else {
                        o << op.show;
                        print(k, o, x[1], h == $.Minus ? y : op.prec);
                    }
                    return;
                }
            } else if (x.size > 2) {
                std::tr1::unordered_map<sym, uint>::const_iterator iter = infixSymbol.find(h);
                if (iter != infixSymbol.end()) {
                    Parser::Oper &op = oper[iter->second];
                    if (op.prec < y) o << L'(';
                    print(k, o, x[1], op.rassoc ? op.prec : op.prec + 1);
					if (x.size == 3) {
						if (h == $.Pattern && x[2].head() == $.Blank) {
							print(k, o, x[2]);
							if (op.prec < y) o << L')';
							return;
						}
						if (h == $.Slot && x[2].isKey() && x[2].key().kind() == Key::Integer) {
							print(k, o, x[2]);
							if (op.prec < y) o << L')';
							return;
						}
					}
                    if (h == $.Plus) {
                        for (uint i = 2; i < x.size; ++i) {
                            if (x[i].head() != $.Minus)
                                o << L'+';
                            print(k, o, x[i], op.prec);
                        }
                    } else if (h == $.Times) {
                        for (uint i = 2; i < x.size; ++i) {
                            // Orderless,因此不用考虑Symbol紧跟Integer的情况
                            // if (!(x[i - 1].isObject() && x[i].isSymbol()))
                                o << L' ';
                            print(k, o, x[i], op.prec);
                        }
                    } else {
                        for (uint i = 2; i < x.size; ++i) {
                            o << op.show;
                            print(k, o, x[i], op.prec);
                        }
                    }
                    if (op.prec < y) o << L')';
                    return;
                }
            }
        }
    }
    print(k, o, x[0], prec);
    o << L'[';
    if (x.size > 1) {
        print(k, o, x[1]);
        for (uint i = 2; i < x.size; ++i) {
            o << L',';
            print(k, o, x[i]);
        }
    }
    o << L']';
}
}