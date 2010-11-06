#include <mU/Parser.h>

namespace mU {
#define get(c) c = read()
#define unget(c) --column;mChar.push_back(c)
#define buffer(c) mText.push_back(c)
#define advance(c) buffer(c);get(c)
#define token(c,t) unget(c);return t
#define iswid(c) (c >= 0x0080 || iswalpha(c))

#define id(c)\
do { advance(c); }\
while(iswid(c) || iswdigit(c))

#define digit(c)\
do { advance(c); }\
while(iswdigit(c))

#define floatexp(c)\
{\
	get(c);\
	if(c == L'+' || c == L'-')\
	{\
		wchar sgn = c;\
		get(c);\
		if(iswdigit(c))\
		{\
			buffer(L'e');\
			buffer(sgn);\
			digit(c);\
			token(c,FLOAT);\
		}\
		unget(c);\
		c = sgn;\
	}\
	else if(iswdigit(c))\
	{\
		buffer(L'e');\
		digit(c);\
		token(c,FLOAT);\
	}\
	unget(c);\
}

#define floatdot(c)\
{\
	digit(c);\
	if(c == L'e')\
	floatexp(c)\
	token(c,FLOAT);\
}

#define newline(c)\
{\
    while((get(c)) && (c == L'\n' || c == L' '));\
    if(c) { token(c,NEWLINE); }\
    return EOI;\
}

Token Parser::scan() {
    mText.clear();
    wchar c;
	get(c);
    if (iswid(c)) {
        id(c);
        token(c, SYMBOL);
    }
    if (iswdigit(c)) {
        digit(c);
        if (c == L'.')
            floatdot(c)
            else if (c == L'e')
                floatexp(c)
                token(c, INTEGER);
            }
    switch (c) {
    case L'\n':
        newline(c)
        break;
    case L' ': {
        while ((c = read()) && c == L' ');
        if (c) {
            if (c == L'\n')
			newline(c)
			token(c, SPACE);
            }
        return EOI;
    }
    break;
    case L'"': {
        while ((get(c)) && c != L'"') {
            if (c == L'\\') {
                get(c);
                switch (c) {
                case L'"':
                    buffer(L'"'); continue;
                case L'\\':
                    buffer(L'\\'); continue;
                case L'n':
                    buffer(L'\n'); continue;
                case L't':
                    buffer(L'\t'); continue;
                default:
                    buffer(L'\\');buffer(c); continue;
                }
            }
            buffer(c);
        }
        if (c)
            return STRING;
        else
            more();
    }
    break;
#define M(x,y) case L##x:return y;
#define C(a,b,x,y)case L##a:{get(c);if(c == L##b)return x;token(c,y);}break;
    M(',', COMMA)M(';', SEMICOLON)
    M('{', LBRACE)M('}', RBRACE)
    M('[', LBRACKET)M(']', RBRACKET)
    M('(', LPAREN)M(')', RPAREN)
    M('\'', QUOTE)M('?', QUESTIONMARK)
    M('.', PERIOD)M('#', POUND)
    M('`', BACKQUOTE)M('$', DOLLAR)
	M('_', BLANK)
    C('&', '&', BOOL_AND_OP, AND_OP)
    C('|', '|', BOOL_OR_OP, OR_OP)
    C('!', '=', NE_OP, BANG)
    C('*', '=', MUL_ASSIGN, STAR)
    C('~', '~', TILDE_TILDE, TILDE)
    C('%', '=', MOD_ASSIGN, PERCENT)
    C('@', '@', AT_AT, AT)
    case L':':
        get(c);
		switch (c) {
            M('=', COLON_ASSIGN)M('>', COLON_GT)
        }
        token(c, COLON);
    case L'=':
        get(c);
		switch (c) {
            M('>', ASSIGN_GT)M('.', ASSIGN_PERIOD)
            C('=', '=', ASSIGN_EQ, EQ_OP)
        case L'!': {
            get(c);
            if (c == L'=')
				return ASSIGN_NE;
            unget(c);
            token(L'!', ASSIGN);
        }
        break;
        }
        token(c, ASSIGN);
    case L'-':
        get(c);
		switch (c) {
            M('=', SUB_ASSIGN)M('>', PTR_OP)
            M('-', DEC_OP)
        }
        token(c, MINUS);
    case L'+':
        get(c);
		switch (c) {
            M('=', ADD_ASSIGN)M('+', INC_OP)
        }
        token(c, PLUS);
    case L'/':
        get(c);
		switch (c) {
            M('=', DIV_ASSIGN)M('.', SLASH_PERIOD)
            M('@', SLASH_AT)M(';', SLASH_SEMICOLON)
        case L'/':
            get(c);
			switch (c) {
                M('.', SLASH_SLASH_PERIOD)
                M('@', SLASH_SLASH_AT)
            }
            token(c, SLASH_SLASH);
        }
        token(c, SLASH);
    case L'<':
        get(c);
		switch (c) {
            M('=', LE_OP)M('<', LEFT_OP)
            M('>', LT_GT)
        }
        token(c, LT_OP);
    case L'>':
        get(c);
		switch (c) {
            M('=', GE_OP)
            C('>', '>', RIGHT_GT, RIGHT_OP)
        }
        token(c, GT_OP);
    case L'^':
        get(c);
		switch (c) {
            M('=', POW_ASSIGN)
        case L':': {
            get(c);
            if (c == L'=')
				return POW_COLON_ASSIGN;
            unget(c);
            token(L':', CIRCUMFLEX);
        }
        break;
        }
        token(c, CIRCUMFLEX);
    }
    return EOI;
}
}