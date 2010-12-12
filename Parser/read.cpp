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
        case L'\r':
            mIn->get();
        case L'\n':
            ++lineno;
            column = 1;
            return L'\n';
        case L'\t':
            column += 4;
            return L' ';
        case L'(':
            if (mIn->peek() == L'*') {
                mIn->get();
                int depth = 1;
                int mode = 1;
                while (depth > 0)
                    switch (mIn->get()) {
                    case -1:
                    case WCHAR_MAX:
                        more();
                    case L'*':
                        if (mode == 2)
                            mode = 0, ++depth;
                        ++column;
                        mode = 1;
                        break;
                    case L'(':
                        ++column;
                        mode = 2;
                        break;
                    case L')':
                        if (mode == 1)
                            --depth;
                        ++column;
                        mode = 0;
                        break;
                    case L'\r':
                        mIn->get();
                    case L'\n':
                        ++lineno;
                        column = 1;
                        mode = 0;
                        break;
                    case L'\t':
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
        case L'\\':
            switch (mIn->peek()) {
            case L':': {
                wchar t[5];
                wint u;
                column += 6;
                mIn->get();
                mIn->get(t, 5);
                wstringstream(t) >> std::hex >> u;
                return (wchar)u;
            }
            break;
            case L'[': {
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
            case L'\r':
                mIn->get();
            case L'\n': {
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
