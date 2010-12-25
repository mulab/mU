#include <mU/Parser.h>

namespace mU {
wchar Parser::read() {
    wchar c;
    if (mChar.empty()) {
        c = mIn->get();
        switch (c) {
        case -1:
        case WCHAR_MAX:
            return 0;
        case _W('\r'):
            mIn->get();
        case _W('\n'):
            ++lineno;
            column = 1;
            return _W('\n');
        case _W('\t'):
            column += 4;
            return _W(' ');
        case _W('('):
            if (mIn->peek() == _W('*')) {
                mIn->get();
                int depth = 1;
                int mode = 1;
                while (depth > 0)
                    switch (mIn->get()) {
                    case -1:
                    case WCHAR_MAX:
                        more();
                    case _W('*'):
                        if (mode == 2)
                            mode = 0, ++depth;
                        ++column;
                        mode = 1;
                        break;
                    case _W('('):
                        ++column;
                        mode = 2;
                        break;
                    case _W(')'):
                        if (mode == 1)
                            --depth;
                        ++column;
                        mode = 0;
                        break;
                    case _W('\r'):
                        mIn->get();
                    case _W('\n'):
                        ++lineno;
                        column = 1;
                        mode = 0;
                        break;
                    case _W('\t'):
                        column += 4;
                        mode = 0;
                        break;
                    default:
                        ++column;
                        mode = 0;
                        break;
                    }
                return read();
            }
            break;
        case _W('\\'):
            switch (mIn->peek()) {
            case _W(':'): {
                wchar t[5];
                wint u;
                column += 6;
                mIn->get();
                mIn->get(t, 5);
                wstringstream(t) >> std::hex >> u;
                return (wchar)u;
            }
            break;
            case _W('['): {
                mIn->get();
                wstring t;
                std::getline(*mIn, t, _W(']'));
                std::unordered_map<wstring, uint>::const_iterator
                iter = grammar.named.find(t);
                if (iter != grammar.named.end()) {
                    column += t.size() + 3;
                    return static_cast<wchar>(grammar.character[iter->second].unicode);
                } else
                    error();
            }
            break;
            case _W('\r'):
                mIn->get();
            case _W('\n'): {
                mIn->get();
                ++lineno;
                column = 1;
                return read();
            }
            break;
            }
            break;
        }
    } else {
        c = mChar.back();
        mChar.pop_back();
    }
    ++column;
    return c;
}
}