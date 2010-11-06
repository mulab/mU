#include <mU/Common.h>
#include <mU/Kernel.h>
#include <mU/Parser.h>

namespace mU {
var Parser::eval() {
    var r;
    while (mLookahead != EOI) {
        try {
            gen(parse());
        } catch (std::exception& e) {
            wcerr
            << L"Parse:"
            << e.what()	<< L"#("
            << lineno << L','
            << column << L')' << endl;
            return r;
        }
        try {
            r = kernel.eval(code());
        } catch (std::exception& e) {
            wcerr
            << L"Eval:"
            << e.what()	<< L"#("
            << lineno << L','
            << column << L')' << endl;
			kernel.start();
            return r;
        }
    }
    return r;
}
var Parser::code() {
	return mCode.back();
}
void Parser::emit(const var& x) {
    if (mGen.empty()) {
        mCode.push_back(x);
        return;
    }
    std::pair<var, uint> &c = mGen.back();
    c.first.tuple()[c.second] = x;
    if (++c.second == c.first.tuple().size)
        mGen.pop_back();
}
void Parser::head(uint n) {
    var r(tuple(n));
    emit(r);
    mGen.push_back(std::make_pair(r, 0));
}
uint Parser::count(uint i) {
    return mChild[i].size();
}
#define oper(x) emit($.x)
void Parser::gen(uint m) {
    Parser::Node& n = mNode[m];
    std::vector<uint>& v = mChild[m];
    typedef std::vector<uint>::const_iterator Iter;
    Iter iter = v.begin(), end = v.end();
    switch (n.tag) {
    case tag_sequence: {
        while (iter != end) {
            gen(*iter);
            ++iter;
        }
    }
		break;
    case tag_expression:
		if (grammar.oper[n.value].postfix || grammar.oper[n.value].prefix) {
			head(2);
			emit(grammar.operSymbol[n.value]);
			gen(*iter);
			break;
		}
		head(3);
		emit(grammar.operSymbol[n.value]);
		gen(*iter);
		gen(*(++iter));
        break;
    case tag_suffix:
        switch (n.value) {
        case -1: {
            Iter begin = iter;
            iter = --end;
            while (iter != begin) {
                --iter;
                gen(*iter);
            }
            gen(*end);
            while (iter != end) {
                gen(*iter);
                ++iter;
            }
        }
        break;
        case -2:
            head(1);
            n.value = -4;
            break;
        case -3:
            head(count(*iter) + 1);
            n.value = -5;
            break;
        case -4:
            break;
        case -5:
            gen(*iter);
            break;
        }
        break;
    case tag_primary:
        switch (n.value) {
        case instr_null: {
            emit(null);
        }
			break;
        case instr_integer: {
            wcs s = mNote[m];
            uint len = wcslen(s);
			emit(new Integer(string(s, s + len).c_str(), 10));
        }
			break;
        case instr_float: {
			wcs s = mNote[m];
            uint len = wcslen(s);
			emit(new Real(string(s, s + len).c_str(), 10, len));
        }
			break;
        case instr_string: {
            emit(new String(mNote[m]));
        }
			break;
        case -1:
            gen(*iter);
            break;
        }
        break;
    case tag_brace:
        switch (n.value) {
        case -1: {
            head(1);
            oper(List);
        }
			break;
        case -2: {
            head(count(*iter) + 1);
            oper(List);
            gen(*iter);
        }
			break;
        }
        break;
    case tag_backquote:
        switch (n.value) {
        case -1: {
            sym c = kernel.context();
            while (iter != end) {
                c = c->symbol(mNote[*iter]);
                ++iter;
            }
            emit(c);
        }
			break;
        }
        break;
    case tag_symbol:
        switch (n.value) {
        case instr_symbol: {
			wcs s = mNote[m];
			if (islower(s[0]))
				emit(key(s));
            else
				emit(kernel.symbol(s));
        }
			break;
        case -1: {
            kernel.beginContext(root->symbol(mNote[*iter]));
            gen(*(++iter));
            kernel.endContext();
        }
			break;
        }
		break;
	case tag_blank:
        switch (n.value) {
		case instr_symbol: {
			head(2);
			oper(Blank);
			gen(*iter);
        }
			break;
		case -1: {
			head(1);
			oper(Blank);
        }
			break;
        }
		break;
	case tag_pound:
        switch (n.value) {
        case -1: {
            wstringstream wss;
            wss << mNote[m];
			uint k = 0;
            wss >> k;
            emit(key(k));
        }
			break;
		case -2: {
            emit(key(mNote[m]));
        }
			break;
		case instr_key: {
			emit(key(uint2wcs(0)));
        }
			break;
        }
        break;
    }
}
}
