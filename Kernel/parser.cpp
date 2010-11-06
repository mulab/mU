#include <mU/Parser.h>

namespace mU {
//////////////////////////////////////
DLL std::vector<parser::char_t> parser::s_char;
DLL stdext::hash_map<wint_t,size_t> parser::s_unicode;
DLL stdext::hash_map<wstring,size_t> parser::s_named;
DLL size_t parser::s_prec = 1;
DLL std::vector<parser::oper_t> parser::s_oper;
DLL std::map<parser::token_t,size_t> parser::s_postfix_token, parser::s_prefix_token, parser::s_infix_token;
DLL stdext::hash_map<wstring,size_t> parser::s_postfix_symbol, parser::s_prefix_symbol, parser::s_infix_symbol;
DLL std::set<parser::token_t> parser::s_end;
void parser::init()
{
	static bool inited = false;
	if(inited) return;
#define T(...) s_oper.push_back(oper_t(__VA_ARGS__));
	T(SEMICOLON,L"CompoundExpression",L";")
	++s_prec;
	T(LEFT_OP,L"Get",L"<<",false,true)
	T(RIGHT_OP,L"Put",L">>")
	T(RIGHT_GT,L"PutAppend",L">>>")
	++s_prec;
	T(ASSIGN,L"Set",L"=")
	T(COLON_ASSIGN,L"SetDelayed",L":=")
	T(POW_ASSIGN,L"UpSet",L"^=")
	T(POW_COLON_ASSIGN,L"UpSetDelayed",L"^:=")
	T(ASSIGN_PERIOD,L"Unset",L"=.",true)
	++s_prec;
	T(SLASH_SLASH,L"Postfix",L"//")
	++s_prec;
	T(AND_OP,L"Function",L"&",true)
	++s_prec;
	T(ADD_ASSIGN,L"AddTo",L"+=")
	T(SUB_ASSIGN,L"SubtractFrom",L"-=")
	T(MUL_ASSIGN,L"TimesBy",L"*=")
	T(DIV_ASSIGN,L"DivideBy",L"/=")
	T(MOD_ASSIGN,L"ModBy",L"%=")
	++s_prec;
	T(ASSIGN_GT,L"Function",L"=>")
	++s_prec;
	T(SLASH_PERIOD,L"ReplaceAll",L"/.")
	T(SLASH_SLASH_PERIOD,L"ReplaceRepeated",L"//.")
	++s_prec;
	T(PTR_OP,L"Rule",L"->")
	T(COLON_GT,L"RuleDelayed",L":>")
	++s_prec;
	T(SLASH_SEMICOLON,L"Condition",L"/;")
	++s_prec;
	T(TILDE_TILDE,L"StringExpression",L"~~")
	++s_prec;
	T(COLON,L"Optional",L":")
	++s_prec;
	T(OR_OP,L"Alternatives",L"|")
	++s_prec;
	T(PERIOD_PERIOD,L"Repeated",L"..",true)
	++s_prec;
	T(BOOL_OR_OP,L"Or",L"||")
	++s_prec;
	T(BOOL_AND_OP,L"And",L"&&")
	++s_prec;
	T(QUOTE,L"Unevaluated",L"'",false,true)
	++s_prec;
	T(BANG,L"Not",L"!",false,true)
	++s_prec;
	T(ASSIGN_EQ,L"SameQ",L"===")
	T(ASSIGN_NE,L"UnsameQ",L"=!=")
	++s_prec;
	T(EQ_OP,L"Equal",L"==")
	T(LE_OP,L"LessEqual",L"<=")
	T(GE_OP,L"GreaterEqual",L">=")
	T(NE_OP,L"Unequal",L"!=")
	T(LT_OP,L"Less",L"<")
	T(GT_OP,L"Greater",L">")
	++s_prec;
	T(PLUS,L"Plus",L"+")
	++s_prec;
	T(STAR,L"Times",L"*")
	++s_prec;
	T(SLASH,L"Divide",L"/",false,false,true)
	++s_prec;
	T(PERCENT,L"Mod",L"%")
	++s_prec;
	T(PLUS,L"Plus",L"+",false,true)
	T(MINUS,L"Minus",L"-",false,true)
	++s_prec;
	T(PERIOD,L"Dot",L".")
	++s_prec;
	T(CIRCUMFLEX,L"Power",L"^")
	++s_prec;
	T(LT_GT,L"StringJoin",L"<>")
	++s_prec;
	T(TILDE,L"Join",L"~")
	++s_prec;
	T(QUOTE,L"Differential",L"'",true)
	++s_prec;
	T(BANG,L"Factorial",L"!",true)
	++s_prec;
	T(SLASH_AT,L"Map",L"/@")
	T(SLASH_SLASH_AT,L"MapAll",L"//@")
	T(AT_AT,L"Apply",L"@@")
	++s_prec;
	T(AT,L"Prefix",L"@")
	++s_prec;
	T(INC_OP,L"PreIncrement",L"++",false,true)
	T(DEC_OP,L"PreDecrement",L"--",false,true)
	++s_prec;
	T(INC_OP,L"Increment",L"++",true)
	T(DEC_OP,L"Decrement",L"--",true)
	++s_prec;
	T(QUESTIONMARK,L"PatternTest",L"?")
	++s_prec;
#undef T
	for(size_t i = 0; i < s_oper.size(); ++i)
	{
		if(s_oper[i].postfix)
		{
			s_postfix_token[s_oper[i].token] = i;
			s_postfix_symbol[s_oper[i].symbol] = i;
		}
		else if(s_oper[i].prefix)
		{
			s_prefix_token[s_oper[i].token] = i;
			s_prefix_symbol[s_oper[i].symbol] = i;
		}
		else
		{
			s_infix_token[s_oper[i].token] = i;
			s_infix_symbol[s_oper[i].symbol] = i;
		}
	}
	s_end.insert(EOI);
	s_end.insert(COMMA);
	s_end.insert(RBRACKET);
	s_end.insert(RPAREN);
	s_end.insert(RBRACE);
#include "character.h"
	for(size_t i = 0; i < s_char.size(); ++i)
	{
		s_unicode[s_char[i].unicode] = i;
		s_named[s_char[i].named] = i;
	}
	inited = true;
}
parser::parser() { init(); }
parser::parser(wistream &x) { init(); start(x); }
void parser::start(wistream &x) { m_in = &x; clear(); }
void parser::parse()
{
	if(lookahead == EOI)
		error();
	gen(expression());
	if(!linebreak && lookahead != EOI)
		error();
}
var parser::result() { return m_code.back(); }
void parser::clear()
{
	m_child.clear();
	m_node.clear();
	m_note.clear();
	m_code.clear();
	lineno = 1;
	column = 1;
	node(tag_invalid,0);
	accept();
}
size_t parser::node(tag_t t,int v)
{
	node_t r;
	r.tag = t; r.value = v;
	m_node.push_back(r);
	return m_node.size() - 1;
}
void parser::insert(size_t i, size_t j)
{
	m_child.insert(std::make_pair(i,j));
}
const wchar* parser::entry(const wstring &s)
{
	return m_entry.insert(s).first->c_str();
}
void parser::note(size_t i, const wstring &s)
{
	m_note.insert(std::make_pair(i,entry(s)));
}
void parser::error()
{
	throw std::logic_error("parser:error:unexpected token");
}
void parser::more()
{
	throw std::logic_error("parser:more:need more input");
}
void parser::accept()
{
	lookback = lookahead;
	lookahead = scan();
	if(lookahead == NEWLINE)
	{
		linebreak = skip = true;
		lookahead = scan();
	}
	else
	{
        linebreak = false;
        if(lookahead == SPACE)
        {
            skip = true;
            lookahead = scan();
        }
        else
            skip = false;
	}
}
void parser::match(token_t t)
{
	if(lookahead == t) { accept(); return; }
	lookahead == EOI ? more() : error();
}
/*
expression
: [prefix] OP (prefix | primary)
| [infix] (primary | postfix | suffix) OP (prefix | primary)
| [primary] (# | prefix | infix) OP
| [postfix] (primary | postfix | suffix) OP
| [suffix] (primary | postfix) OP
;
*/
void parser::shift(frame_t &s)
{
	std::map<token_t,size_t>::const_iterator iter;
	switch(s.mode)
	{
	case -1://#.
	case -2://prefix.
	case -3://infix.
	//[prefix | primary]
		iter = s_prefix_token.find(lookahead);
		if(iter != s_prefix_token.end())
		{
			accept();
			s.op = iter->second;
			s.mode = -2;
			return;
		}
		s.op = primary();
		s.mode = -4;
		return;
	case -4://primary.
	case -5://postfix.
	//[suffix]
		if(lookahead == LBRACKET)
		{
			s.op = suffix();
			s.mode = -6;
			return;
		}
	case -6://suffix.
	//[infix | postfix]
		if(linebreak || s_end.count(lookahead))
		{
			s.mode = -7;
			return;
		}
		iter = s_infix_token.find(lookahead);
		if(iter != s_infix_token.end())
		{
			accept();
			s.op = iter->second;
			s.mode = -3;
			return;
		}
		iter = s_postfix_token.find(lookahead);
		if(iter != s_postfix_token.end())
		{
			accept();
			s.op = iter->second;
			s.mode = -5;
			return;
		}
		//MINUS -> PLUS MINUS
		if(lookahead == MINUS)
		{
			s.op = s_infix_token[PLUS];
			s.mode = -3;
			return;
		}
		//SPACE -> STAR
		s.op = s_infix_token[STAR];
		s.mode = -3;
		return;
	}
}
void parser::reduce(frame_t &s)
{
	size_t r[3];
	switch(s.mode)
	{
	case -3:case -5:case -7:
		while(!oper_stack.empty() && compare(s))
		{
			size_t oper = oper_stack.top();
			r[0] = node(tag_expression,oper);
			if(s_oper[oper].postfix || s_oper[oper].prefix)
			{
				r[1] = oprn_stack.top(); oprn_stack.pop();
				insert(r[0],r[1]);
			}
			else
			{
				r[1] = oprn_stack.top(); oprn_stack.pop();
				r[2] = oprn_stack.top(); oprn_stack.pop();
				insert(r[0],r[2]);
				insert(r[0],r[1]);
			}
			oper_stack.pop();
			oprn_stack.push(r[0]);
		}
		return;
	case -6:
		{
			r[0] = oprn_stack.top(); oprn_stack.pop();
			while(!oper_stack.empty() && s_oper[oper_stack.top()].postfix)
			{
				r[1] = node(tag_expression,oper_stack.top());
				oper_stack.pop();
				insert(r[1],r[0]);
				r[0] = r[1];
			}
			insert(s.op,r[0]);
		}
		return;
	}
}
bool parser::compare(frame_t &s)
{
	if(oper_stack.size() == s.offset) return false;
	if(s.mode == -7) return true;
	if(s_oper[s.op].prec < s_oper[oper_stack.top()].prec) return true;
	if(s_oper[s.op].prec == s_oper[oper_stack.top()].prec) return s_oper[s.op].rassoc;
	return false;
}
size_t parser::expression()
{
	size_t r[1];
	frame_t s(oper_stack.size(),-1);
	while(s.mode != -7)
	{
		shift(s);
		reduce(s);
		switch(s.mode)
		{
		case -2:case -3:case -5: oper_stack.push(s.op); break;
		case -4:case -6: oprn_stack.push(s.op);	break;
		}
	}
	r[0] = oprn_stack.top(); oprn_stack.pop();
	return r[0];
}

/*
suffix
: ( LBRACKET LBRACKET sequence? RBRACKET RBRACKET
| LBRACKET sequence? RBRACKET
)*
;
*/
size_t parser::suffix()
{
	size_t r[3];
	r[0] = node(tag_suffix,-1);
	while(lookahead == LBRACKET)
	{
		accept();
		if(lookahead == LBRACKET)
		{
			accept();
			if(lookahead == RBRACKET)
			{
				accept();
				match(RBRACKET);
				r[1] = node(tag_suffix,-2);
				insert(r[0],r[1]);
				continue;
			}
			else
			{
				r[1] = sequence();
				match(RBRACKET);
				match(RBRACKET);
				r[2] = node(tag_suffix,-3);
				insert(r[2],r[1]);
				insert(r[0],r[2]);
				continue;
			}
		}
		if(lookahead == RBRACKET)
		{
			accept();
			r[1] = node(tag_suffix,-4);
			insert(r[0],r[1]);
			continue;
		}
		r[1] = sequence();
		match(RBRACKET);
		r[2] = node(tag_suffix,-5);
		insert(r[2],r[1]);
		insert(r[0],r[2]);
		continue;
	}
	return r[0];
}

/*
sequence
: [sequence] expression (COMMA expression)*
;
*/
size_t parser::sequence()
{
	size_t r[2];
	r[0] = node(tag_sequence,-1);
	r[1] = expression();
	insert(r[0],r[1]);
	while(lookahead == COMMA)
	{
		accept();
		r[1] = expression();
		insert(r[0],r[1]);
	}
	return r[0];
}

/*
primary
: LBRACE sequence? RBRACE
| LPAREN expression RPAREN
| (UNDERLINE | UNDERLINE_UNDERLINE) (IDENTIFIER | PERIOD)?
| (POUND | POUND_POUND) INTEGER?
| INTEGER
| REAL
| STRING
| IDENTIFIER (UNDERLINE | UNDERLINE_UNDERLINE) (IDENTIFIER | PERIOD)?
| IDENTIFIER (COLON expression)?
| IDENTIFIER
;
*/
size_t parser::primary()
{
	size_t r[3];
	int value = 0;
	switch(lookahead)
	{
	case LBRACE:
		accept();
		if(lookahead == RBRACE)
		{
			accept();
			r[0] = node(tag_primary,-1);
		}
		else
		{
			r[0] = node(tag_primary,-2);
			r[1] = sequence();
			match(RBRACE);
			insert(r[0],r[1]);
		}
		return r[0];
	case LPAREN:
		accept();
		r[0] = expression();
		match(RPAREN);
		r[1] = node(tag_primary,-3);
		insert(r[1],r[0]);
		return r[1];
	case UNDERLINE:
		if(value == 0) value = -4;
	case UNDERLINE_UNDERLINE:
		if(value == 0) value = -7;
		accept();
		if(!skip)
		{
			if(lookahead == IDENTIFIER)
			{
				r[0] = node(tag_primary,value);
				r[1] = node(tag_primary,instr_symbol);
				note(r[1],text);
				accept();
				insert(r[0],r[1]);
				return r[0];
			}
			if(lookahead == PERIOD)
			{
				accept();
				return node(tag_primary,value - 1);
			}
		}
		return node(tag_primary,value - 2);
	case POUND:
		if(value == 0) value = -10;
	case POUND_POUND:
		if(value == 0) value = -12;
		accept();
		if(lookahead == INTEGER)
		{
			r[0] = node(tag_primary,value);
			r[1] = node(tag_primary,instr_integer);
			note(r[1],text);
			accept();
			insert(r[0],r[1]);
			return r[0];
		}
		return node(tag_primary,value - 1);
	case INTEGER:
		r[0] = node(tag_primary,instr_integer);
		note(r[0],text);
		accept();
		return r[0];
	case REAL:
		r[0] = node(tag_primary,instr_float);
		note(r[0],text);
		accept();
		return r[0];
	case STRING:
		r[0] = node(tag_primary,instr_string);
		note(r[0],text);
		accept();
		return r[0];
	case IDENTIFIER:
		r[0] = node(tag_primary,instr_symbol);
		note(r[0],text);
		accept();
		if(!skip)
		{
			switch(lookahead)
			{
			case UNDERLINE:
				if(value == 0) value = -14;
			case UNDERLINE_UNDERLINE:
				if(value == 0) value = -17;
				accept();
				if(!skip)
				{
					if(lookahead == IDENTIFIER)
					{
						r[1] = node(tag_primary,value);
						r[2] = node(tag_primary,instr_symbol);
						note(r[2],text);
						accept();
						insert(r[1],r[0]);
						insert(r[1],r[2]);
						return r[1];
					}
					if(lookahead == PERIOD)
					{
						accept();
						r[1] = node(tag_primary,value - 1);
						insert(r[1],r[0]);
						return r[1];
					}
				}
				r[1] = node(tag_primary,value - 2);
				insert(r[1],r[0]);
				return r[1];
			case COLON:
				accept();
				r[1] = expression();
				r[2] = node(tag_primary,-20);
				insert(r[2],r[0]);
				insert(r[2],r[1]);
				return r[2];
			case COLON_COLON:
				accept();
				if(lookahead == IDENTIFIER)
				{
					r[1] = node(tag_primary,-21);
					r[2] = node(tag_primary,instr_symbol);
					note(r[2],text);
					accept();
					insert(r[1],r[0]);
					insert(r[1],r[2]);
					return r[1];
				}
				else
					error();
				break;
			}
		}
		return r[0];
	}
	//SPACE -> L"Null"
	if(lookback == COMMA || lookback == SEMICOLON)
	{
		r[0] = node(tag_primary,instr_symbol);
		note(r[0],L"Null");
		return r[0];
	}
	lookahead == EOI ? more() : error();
}
//////////////////////////////////////
}
