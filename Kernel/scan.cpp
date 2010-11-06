#include <mU/Parser.h>

namespace mU {
//////////////////////////////////////
wchar parser::read()
{
	wchar c;
	if(char_stack.empty())
	{
		c = m_in->get();
		switch(c)
		{
		case -1:
		case WCHAR_MAX: return 0;
		case L'\r': m_in->get();
		case L'\n': ++lineno; column = 1; return L'\n';
		case L'\t': column += 4; return L' ';
		case L'(':
			if(m_in->peek() == L'*')
			{
				m_in->get();
				int depth = 1;
				int mode = 1;
				while(depth > 0)
					switch(m_in->get())
					{
					case -1:
					case WCHAR_MAX: more();
					case L'*':
						if(mode == 2) mode = 0, ++depth;
						++column; mode = 1; break;
					case L'(': ++column; mode = 2; break;
					case L')':
						if(mode == 1) --depth;
						++column; mode = 0; break;
					case L'\r': m_in->get();
					case L'\n': ++lineno; column = 1; mode = 0; break;
					case L'\t': column += 4; mode = 0; break;
					default: ++column; mode = 0; break;
					}
				return read();
			}
			break;
		case L'\\':
			switch(m_in->peek())
			{
			case L':':
				{
					wchar t[5];
					wint_t u;
					column += 6;
					m_in->get();
					m_in->get(t,5);
					wstringstream(t) >> std::hex >> u;
					return (wchar)u;
				}
				break;
			case L'[':
				{
					m_in->get();
					wstring t;
					std::getline(*m_in,t,L']');
					stdext::hash_map<wstring,size_t>::const_iterator
						iter = s_named.find(t);
					if(iter != s_named.end())
					{
						column += t.size() + 3;
						return (wchar)s_char[iter->second].unicode;
					}
					else
						error();
				}
				break;
			case L'\r': m_in->get();
			case L'\n':
				{
					m_in->get();
					++lineno;
					column = 1;
					return read();
				}
				break;
			}
			break;
		}
	}
	else
	{
		c = char_stack.top();
		char_stack.pop();
	}
	++column;
	return c;
}
#define get(c) c = read();
#define unget(c) --column; char_stack.push(c);
#define buffer(c) text.push_back(c);
#define advance(c) buffer(c)get(c)
#define token(c,t) unget(c)return t;
#define iswid(c) (c == L'$' || c >= 0x0080 || iswalpha(c))
#define id(c)\
do { advance(c) }\
while(iswid(c) || iswdigit(c));

#define idcontext(c)\
{\
id(c)\
while(c == L'`')\
{\
	advance(c)\
	if(iswid(c)) id(c)\
	else error();\
}\
token(c,IDENTIFIER)\
}

#define digit(c)\
do { advance(c) }\
while(iswdigit(c));

#define floatexp(c)\
{\
	get(c)\
	if(c == L'+' || c == L'-')\
	{\
		wchar sgn = c;\
		get(c)\
		if(iswdigit(c))\
		{\
			buffer(L'e')\
			buffer(sgn)\
			digit(c)\
			token(c,REAL)\
		}\
		unget(c)\
		c = sgn;\
	}\
	else if(iswdigit(c))\
	{\
		buffer(L'e')\
		digit(c)\
		token(c,REAL)\
	}\
	unget(c)\
}

#define floatdot(c)\
{\
	digit(c)\
	if(c == L'e')\
	floatexp(c)\
	token(c,REAL)\
}

#define newline(c)\
{\
    while((c = read()) && (c == L'\n' || c == L' '));\
    if(c) { token(c,NEWLINE) }\
    return EOI;\
}

parser::token_t parser::scan()
{
	text.clear();
	wchar c = read();
	if(iswid(c))
	idcontext(c)
	if(iswdigit(c))
	{
		digit(c)
		if(c == L'.')
		floatdot(c)
		else if(c == L'e')
		floatexp(c)
		token(c,INTEGER)
	}
	switch(c)
	{
    case L'\n':newline(c)break;
    case L' ':
        {
            while((c = read()) && c == L' ');
            if(c) { if(c == L'\n') newline(c) token(c,SPACE) }
            return EOI;
        }
        break;
	case L'.': 
		{
			get(c)
			if(iswdigit(c))
			{
				buffer(L'.')
				digit(c)
				if(c == L'e') floatexp(c)
				token(c,REAL)
			}
			unget(c)
			return PERIOD;
		}
		break;
	case L'"':
		{
			while((c = read()) && c != L'"')
			{
				if(c == L'\\')
				{
					c = read();
					switch(c)
					{
					case L'"': buffer(L'"') continue;
					case L'\\': buffer(L'\\') continue;
					case L'n': buffer(L'\n') continue;
					case L't': buffer(L'\t') continue;
					default: buffer(L'\\')buffer(c) continue;
					}
				}
				buffer(c)
			}
			if(c) return STRING;
			else more();
		}
		break;
	case L'`':
		{
			advance(c)
			if(iswid(c)) idcontext(c)
			else error();
		}
		break;
#define T(x,y) case L##x:return y;
#define C(a,b,x,y)case L##a:{get(c)if(c == L##b)return x;token(c,y)}break;
	T(',',COMMA)T(';',SEMICOLON)
	T('{',LBRACE)T('}',RBRACE)
	T('[',LBRACKET)T(']',RBRACKET)
	T('(',LPAREN)T(')',RPAREN)
	T('\'',QUOTE)T('?',QUESTIONMARK)
	C('&','&',BOOL_AND_OP,AND_OP)
	C('|','|',BOOL_OR_OP,OR_OP)
	C('!','=',NE_OP,BANG)
	C('*','=',MUL_ASSIGN,STAR)
	C('~','~',TILDE_TILDE,TILDE)
	C('%','=',MOD_ASSIGN,PERCENT)
	C('_','_',UNDERLINE_UNDERLINE,UNDERLINE)
	C('#','#',POUND_POUND,POUND)
	C('@','@',AT_AT,AT)
	case L':':
		get(c) switch(c) {
		T(':',COLON_COLON)T('=',COLON_ASSIGN)
		T('>',COLON_GT)
		} token(c,COLON)
	case L'=':
		get(c) switch(c) {
		T('>',ASSIGN_GT)T('.',ASSIGN_PERIOD)
		C('=','=',ASSIGN_EQ,EQ_OP)
		case L'!':
			{
				get(c)
				if(c == L'=') return ASSIGN_NE;
				unget(c)
				token(L'!',ASSIGN)
			}
			break;
		} token(c,ASSIGN)
	case L'-':
		get(c) switch(c) {
		T('=',SUB_ASSIGN)T('>',PTR_OP)
		T('-',DEC_OP)
		} token(c,MINUS)
	case L'+':
		get(c) switch(c) {
		T('=',ADD_ASSIGN)T('+',INC_OP)
		} token(c,PLUS)
	case L'/':
		get(c) switch(c) {
		T('=',DIV_ASSIGN)T('.',SLASH_PERIOD)
		T('@',SLASH_AT)T(';',SLASH_SEMICOLON)
		case L'/':
			get(c) switch(c) {
			T('.',SLASH_SLASH_PERIOD)
			T('@',SLASH_SLASH_AT)
			} token(c,SLASH_SLASH)
		} token(c,SLASH)
	case L'<':
		get(c) switch(c) {
		T('=',LE_OP)T('<',LEFT_OP)
		T('>',LT_GT)
		} token(c,LT_OP)
	case L'>':
		get(c) switch(c) {
		T('=',GE_OP)
		C('>','>',RIGHT_GT,RIGHT_OP)
		} token(c,GT_OP)
	case L'^':
		get(c) switch(c) {
		T('=',POW_ASSIGN)
		case L':':
			{
				get(c)
				if(c == L'=') return POW_COLON_ASSIGN;
				unget(c)
				token(L':',CIRCUMFLEX)
			}
			break;
		} token(c,CIRCUMFLEX)
	}
	return EOI;
}
//////////////////////////////////////
}
