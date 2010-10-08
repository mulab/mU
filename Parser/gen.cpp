#include <mU/Common.h>
#include <mU/Kernel.h>
#include <mU/Parser.h>
#include <list>

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
    var r;
    if (mCode.size() == 1)
        r = mCode.back();
    else
        r = mU::list(mCode.size(), mCode.begin(), $.Serial);
    mCode.clear();
    return r;
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
Parser::NodeIter Parser::lower(uint i) {
    return mChild.equal_range(i).first;
}
Parser::NodeIter Parser::upper(uint i) {
    return mChild.equal_range(i).second;
}
uint Parser::count(uint i) {
    return mChild.count(i);
}
#define oper(x) emit($.x)
void Parser::gen(uint m) {
    Parser::Node& n = mNode[m];
    NodeIter iter = lower(m);
    switch (n.tag) {
    case tag_sequence: {
        NodeIter end = upper(m);
        while (iter != end) {
            gen(iter->second);
            ++iter;
        }
    }
		break;
    case tag_expression:
		if (grammar.oper[n.value].postfix || grammar.oper[n.value].prefix) {
			head(2);
			emit(grammar.operSymbol[n.value]);
			gen(iter->second);
			break;
		}
		head(3);
		emit(grammar.operSymbol[n.value]);
		gen(iter->second);
		gen((++iter)->second);
        break;
    case tag_suffix:
        switch (n.value) {
        case -1: {
            // FIXME: 看起来unordered_multimap::const_iterator不是双向的，
            // 暂时用一个list中转一下
            NodeIter begin0 = lower(m), end0 = upper(m);
            std::list<uint> temp;
            for (NodeIter iii = begin0; iii != end0; ++iii) {
                temp.push_back(iii->second);
            }
            const std::list<uint>::const_iterator begin = temp.begin();
            std::list<uint>::const_iterator end = temp.end();
            std::list<uint>::const_iterator iter = --end;
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
            head(count(iter->second) + 1);
            n.value = -5;
            break;
        case -4:
            break;
        case -5:
            gen(iter->second);
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
            emit(new Integer(string(s, s + wcslen(s)).c_str(), 10));
        }
			break;
        case instr_float: {
            wcs s = mNote[m];
            emit(new Real(string(s, s + wcslen(s)).c_str(), 10));
        }
			break;
        case instr_string: {
            emit(new String(mNote[m]));
        }
			break;
        case -1:
            gen(iter->second);
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
            head(count(iter->second) + 1);
            oper(List);
            gen(iter->second);
        }
			break;
        }
        break;
    case tag_backquote:
        switch (n.value) {
        case -1: {
            sym c = kernel.context();
            NodeIter end = upper(m);
            while (iter != end) {
                c = c->symbol(mNote[iter->second]);
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
            kernel.beginContext(root->symbol(mNote[iter->second]));
            gen((++iter)->second);
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
			gen(iter->second);
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
        case instr_key: {
            wstringstream wss;
            wss << mNote[m];
            uint k;
            wss >> k;
            emit(key(k));
        }
			break;
		case -1: {
			emit(key(uint2wcs(0)));
        }
			break;
        }
        break;
    }
}
}
