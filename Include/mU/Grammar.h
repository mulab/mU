#pragma once
#include "var.h"

#undef API
#ifdef _MSC_VER
#ifdef PARSER_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

namespace mU {
class Kernel;
enum Token {
	SYMBOL, INTEGER, FLOAT, STRING, BLANK, POUND,
	PTR_OP, INC_OP, DEC_OP, LEFT_OP, RIGHT_OP, RIGHT_GT, LE_OP, GE_OP, EQ_OP, NE_OP,
	BOOL_AND_OP, BOOL_OR_OP, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
	LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE, PERIOD, COMMA, COLON, SEMICOLON,
	QUESTIONMARK, PLUS, MINUS, STAR, SLASH, ASSIGN, AND_OP, OR_OP, BANG, TILDE, TILDE_TILDE,
	PERCENT, CIRCUMFLEX, GT_OP, LT_OP, COLON_ASSIGN, POW_ASSIGN, POW_COLON_ASSIGN, COLON_GT,
	SLASH_PERIOD, SLASH_SEMICOLON, DOLLAR, AT, SLASH_SLASH, AT_AT, ASSIGN_EQ, ASSIGN_PERIOD,
	SLASH_SLASH_PERIOD, ASSIGN_NE, QUOTE, BACKQUOTE, SLASH_AT, SLASH_SLASH_AT, SPACE, LT_GT, ASSIGN_GT,
	NEWLINE, EOI = -1
};
struct Grammar {
    API static bool Inited;
    API static void Init();
    API Grammar();
    
	std::tr1::unordered_set<Token> end;
	struct Char {
		wint unicode;
		wcs named;
		Char(wint _unicode, wcs _named) :
		unicode(_unicode), named(_named) {}
	};
	std::vector<Char> character;
	std::tr1::unordered_map<wint, uint> unicode;
	std::tr1::unordered_map<wstring, uint> named;

	struct Oper {
		Token token;
		wcs name;
		wcs show;
		uint prec;
		//默认为infix
		bool postfix;
		bool prefix;
		bool rassoc;
		Oper(Token, wcs, wcs, bool = false, bool = false, bool = false);
	};
    uint prec;
    std::vector<Oper> oper;
    std::vector<sym> operSymbol;
    std::tr1::unordered_map<sym, uint> postfixSymbol, prefixSymbol, infixSymbol;
	std::tr1::unordered_map<Token, uint> postfixToken, prefixToken, infixToken;
	uint operCOLON, operPERIOD, operPLUS, operSTAR;
	
	API void print(Kernel&, wostream&, wchar);
	API void print(Kernel&, wostream&, wcs);
	API void print(Kernel&, wostream&, sym);
	API void print(Kernel&, wostream&, const Key&);
	API void print(Kernel&, wostream&, const Object&);
	API void print(Kernel&, wostream&, const Tuple&, uint = 0);
	API void print(Kernel&, wostream&, const var&, uint = 0);
} API extern grammar;
inline void println(Kernel& k, const var& x, wostream& o = wcout) {
	grammar.print(k, o, x);
	o << endl;
}
}

#ifndef _MSC_VER
namespace std { namespace tr1 {
template<>
inline size_t
hash<mU::Token>::operator()(mU::Token v) const {
	   return static_cast<size_t>(v);
}
}}
#endif
