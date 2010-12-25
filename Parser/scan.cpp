#include <mU/Parser.h>

namespace mU {
#define get(c) c = read()
#define unget(c) --column;mChar.push_back(c)
#define buffer(c) mText.push_back(c)
#define advance(c) buffer(c);get(c)
#define token(c,t) unget(c);return t

#define id(c)\
do { advance(c); }\
while(isalpha(c) || isdigit(c))

#define digit(c)\
do { advance(c); }\
while(isdigit(c))

#define floatexp(c)\
{\
	get(c);\
	if(c == L'+' || c == L'-')\
	{\
		wchar sgn = c;\
		get(c);\
		if(isdigit(c))\
		{\
			buffer(L'e');\
			buffer(sgn);\
			digit(c);\
			token(c,FLOAT);\
		}\
		unget(c);\
		c = sgn;\
	}\
	else if(isdigit(c))\
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
    if (isalpha(c)) {
        id(c);
        token(c, SYMBOL);
    }
    if (isdigit(c)) {
        digit(c);
        if (c == L'.')
            floatdot(c)
            else if (c == _W('e'))
                floatexp(c)
                token(c, INTEGER);
            }
    switch (c) {
    case _W('\n'):
        newline(c)
        break;
    case _W(' '): {
        while ((c = read()) && c == _W(' '));
        if (c) {
            if (c == _W('\n'))
			newline(c)
			token(c, SPACE);
		}
	}
	break;
    case _W('"'): {
        while ((get(c)) && c != _W('"')) {
            if (c == _W('\\')) {
                get(c);
                switch (c) {
#define M(x) case _W(x):buffer(_W(x));continue;
                M('"')
				M('\\')
				M('\n')
				M('\t')
#undef M
                default:
                    buffer(_W('\\'));
					buffer(c);
					continue;
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
	case _W('`'): {
		if ((get(c)) && c == _W('`')) {
			while (get(c)) {
				if (c == _W('`')) {
					get(c);
					if (c == _W('`'))
						break;
					else {
						buffer(_W('`'));
						buffer(c);
						continue;
					}
				}
				buffer(c);
			}
			if (c)
				return STRING;
			else
				more();
		}
		token(c, BACKQUOTE);
    }
    break;
#define M(x,y) case _W(x):return y;
#define C(a,b,x,y)case _W(a):{get(c);if(c == _W(b))return x;token(c,y);}break;
	M('_', BLANK)
	M(',', COMMA)
	M('$', DOLLAR)
	M('{', LBRACE)
	M('[', LBRACKET)
	M('(', LPAREN)
	M('.', PERIOD)
	M('#', POUND)
	M('?', QUESTIONMARK)
	M('\'', QUOTE)
	M('}', RBRACE)
	M(']', RBRACKET)
	M(')', RPAREN)
	M(';', SEMICOLON)
    C('&', '&', BOOL_AND_OP, AND_OP)
    C('|', '|', BOOL_OR_OP, OR_OP)
    C('!', '=', NE_OP, BANG)
    C('*', '=', MUL_ASSIGN, STAR)
    C('~', '~', TILDE_TILDE, TILDE)
    C('%', '=', MOD_ASSIGN, PERCENT)
    C('@', '@', AT_AT, AT)
    case _W(':'):
        get(c);
		switch (c) {
            M('=', COLON_ASSIGN)
			M('>', COLON_GT)
        }
        token(c, COLON);
    case _W('='):
        get(c);
		switch (c) {
            M('>', ASSIGN_GT)
			M('.', ASSIGN_PERIOD)
            C('=', '=', ASSIGN_EQ, EQ_OP)
        case _W('!'): {
            get(c);
            if (c == _W('='))
				return ASSIGN_NE;
            unget(c);
            token(_W('!'), ASSIGN);
        }
        break;
        }
        token(c, ASSIGN);
    case _W('-'):
        get(c);
		switch (c) {
            M('=', SUB_ASSIGN)
			M('>', PTR_OP)
            M('-', DEC_OP)
        }
        token(c, MINUS);
    case _W('+'):
        get(c);
		switch (c) {
            M('=', ADD_ASSIGN)
			M('+', INC_OP)
        }
        token(c, PLUS);
    case _W('/'):
        get(c);
		switch (c) {
            M('=', DIV_ASSIGN)
			M('.', SLASH_PERIOD)
            M('@', SLASH_AT)
			M(';', SLASH_SEMICOLON)
        case L'/':
            get(c);
			switch (c) {
                M('.', SLASH_SLASH_PERIOD)
                M('@', SLASH_SLASH_AT)
            }
            token(c, SLASH_SLASH);
        }
        token(c, SLASH);
    case _W('<'):
        get(c);
		switch (c) {
            M('=', LE_OP)
			M('<', LEFT_OP)
            M('>', LT_GT)
        }
        token(c, LT_OP);
    case _W('>'):
        get(c);
		switch (c) {
            M('=', GE_OP)
            C('>', '>', RIGHT_GT, RIGHT_OP)
        }
        token(c, GT_OP);
    case _W('^'):
        get(c);
		switch (c) {
            M('=', POW_ASSIGN)
        case _W(':'): {
            get(c);
            if (c == L'=')
				return POW_COLON_ASSIGN;
            unget(c);
            token(_W(':'), CIRCUMFLEX);
        }
        break;
        }
        token(c, CIRCUMFLEX);
    }
#undef C
#undef M
    return EOI;
}
}