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

// TODO: 节点定义改为
/*
struct node {
	node* child;
	node* sibling;
	// node* childs[];
};
*/
// 可通过语言动态修改语法定义,类似于Lisp,Maxima,LaTeX(\catcode)

namespace mU {
class Kernel;
class Parser {
public:
    API Parser(Kernel&);
    Kernel& kernel;

	struct Char {
        wint unicode;
        wcs named;
        Char(wint _unicode, wcs _named) :
                unicode(_unicode), named(_named) {}
    };
	wchar read();
	
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
	Token scan();
	void accept();
	void skip();
	void match(Token);

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
	enum Tag {
		tag_invalid = -1, tag_sequence, tag_expression, tag_suffix, tag_primary,
		tag_brace, tag_backquote, tag_symbol, tag_blank, tag_pound
	};
	API void start(wistream&);
	API uint sequence(Token = COMMA);
    API uint expression();
    API uint suffix();
    API uint primary();
    API uint brace();
    API uint backquote();
    API uint symbol();
	API uint blank();
	API uint pound();
    API uint parse();
	
	uint lineno;
	uint column;
	API void error();
	API void more();
	
	enum Instr {
		//字节码/Stack/FullForm/MX
		instr_null, instr_symbol, instr_key, instr_integer, instr_float, instr_string, instr_tuple
	};
	API void gen(uint);
    API var code();
    API var eval();
protected:
    wistream* mIn;
    wstring mText;
    Token mLookahead;
    // mLookStack?
    Token mLookback;
    bool mSkip;
    bool mLinebreak;
    std::vector<wchar> mChar;
    struct Node {
        Tag tag;
        int value;
    };
    std::vector<Node> mNode;
    std::tr1::unordered_multimap<uint, uint> mChild;
    std::tr1::unordered_map<uint, wcs> mNote;
    std::vector<uint> mOper;
    std::vector<uint> mOprn;
    struct Frame {
        uint offset;
        int mode;
        uint op;

        Frame(uint $offset, int $mode, uint $op = 0) :
                offset($offset), mode($mode), op($op) {}
    };
    uint node(Tag, int);
    void insert(uint, uint);
    void note(uint, const wstring&);
    void shift(Frame&);
    void reduce(Frame&);
    bool compare(Frame&);

    std::vector<var> mCode;
    std::vector<std::pair<var, uint> > mGen;
    typedef std::tr1::unordered_multimap<uint, uint>::const_iterator NodeIter;
    NodeIter lower(uint);
    NodeIter upper(uint);
    uint count(uint);
    void emit(const var&);
    void head(uint);
};
struct Grammar {
    API static bool Inited;
    API static void Init();
    API Grammar();
    
	std::vector<Parser::Char> character;
    std::tr1::unordered_map<wint, uint> unicode;
    std::tr1::unordered_map<wstring, uint> named;
    uint prec;
    std::vector<Parser::Oper> oper;
    std::tr1::unordered_map<Parser::Token, uint> postfixToken, prefixToken, infixToken;
    std::tr1::unordered_set<Parser::Token> end;
    std::vector<sym> operSymbol;
    std::tr1::unordered_map<sym, uint> postfixSymbol, prefixSymbol, infixSymbol;
	
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
