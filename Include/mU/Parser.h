#pragma once
#include "Grammar.h"

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
class Parser {
public:
    API Parser(Kernel&);
    Kernel& kernel;
	
	wchar read();
	Token scan();
	void accept();
	void skip();
	void match(Token);

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
    std::tr1::unordered_map<uint, std::vector<uint> > mChild;
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
    uint count(uint);
    void emit(const var&);
    void head(uint);
};
}
