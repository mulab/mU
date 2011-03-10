#include <mU/Parser.h>
#include <stdexcept>

namespace mU {
void Parser::start(wistream& in) {
    mText.clear();
    mChar.clear();
    mNode.clear();
    mNote.clear();
    mChild.clear();
    mOper.clear();
    mOprn.clear();
    mGen.clear();
    mCode.clear();
    mIn = &in;
    lineno = 1;
    column = 1;
    node(tag_invalid, 0);
    accept();
}
uint Parser::parse() {
    if (mLookahead == EOI)
        error();
    uint r = expression();
    if (!mLinebreak && mLookahead != EOI)
        error();
    return r;
}
uint Parser::node(Tag t, int v) {
    Node r;
    r.tag = t;
    r.value = v;
    mNode.push_back(r);
    return mNode.size() - 1;
}
void Parser::insert(uint i, uint j) {
    mChild[i].push_back(j);
}
void Parser::note(uint i, const wstring& s) {
    mNote.insert(std::make_pair(i, wstr(s.c_str())));
}
void Parser::error() {
    throw std::logic_error("error");
}
void Parser::more() {
    throw std::logic_error("more");
}
void Parser::accept() {
    mLookback = mLookahead;
    mLookahead = scan();
    skip();
}
void Parser::skip() {
    if (mLookahead == NEWLINE) {
        mLinebreak = mSkip = true;
        mLookahead = scan();
    } else {
        mLinebreak = false;
        if (mLookahead == SPACE) {
            mSkip = true;
            mLookahead = scan();
        } else
            mSkip = false;
    }
}
void Parser::match(Token t) {
    if (mLookahead == t) {
        accept();
        return;
    }
    mLookahead == EOI ? more() : error();
}

/*
sequence
: [sequence] expression (DELIMITER expression)*
;
*/
uint Parser::sequence(Token t) {
    uint r[2];
    r[0] = node(tag_sequence, 0);
    r[1] = expression();
    insert(r[0], r[1]);
    while (mLookahead == t) {
        accept();
        r[1] = expression();
        insert(r[0], r[1]);
    }
    return r[0];
}

/*
expression
: [prefix] PREFIX (prefix | primary)
| [infix] (primary | postfix | suffix) INFIX prefix
| [primary] (# | prefix | infix) PRIMARY
| [postfix] (primary | postfix | suffix) POSTFIX
| [suffix] (primary | postfix) SUFFIX
;
*/
void Parser::shift(Frame &s) {
    std::unordered_map<Token, uint>::const_iterator iter;
    switch (s.mode) {
    case -1://#.
    case -2://prefix.
    case -3://infix.
        //->prefix | primary
        iter = grammar.prefixToken.find(mLookahead);
        if (iter != grammar.prefixToken.end()) {
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
        //->suffix
        if (mLookahead == LBRACKET) {
            s.op = suffix();
            s.mode = -6;
            return;
        }
    case -6://suffix.
        //->infix | postfix
        if (mLinebreak || grammar.end.count(mLookahead)) {
            s.mode = -7;
            return;
        }
        iter = grammar.infixToken.find(mLookahead);
        if (iter != grammar.infixToken.end()) {
            accept();
            s.op = iter->second;
            s.mode = -3;
            return;
        }
        iter = grammar.postfixToken.find(mLookahead);
        if (iter != grammar.postfixToken.end()) {
            accept();
            s.op = iter->second;
            s.mode = -5;
            return;
        }
		if (mLookahead == BLANK && mLookback == SYMBOL) {
			//SYMBOL BLANK -> SYMBOL COLON BLANK
			s.op = grammar.operCOLON;
			s.mode = -3;
			return;
		}
		if (mLookahead == POUND) {
			//POUND -> PERIOD POUND
			s.op = grammar.operPERIOD;
			s.mode = -3;
			return;
		}
		if (mLookahead == MINUS) {
			//MINUS -> PLUS MINUS
			s.op = grammar.operPLUS;
			s.mode = -3;
			return;
		}
        //SPACE -> STAR
		s.op = grammar.operSTAR;
        s.mode = -3;
        return;
    }
}
void Parser::reduce(Frame &s) {
    uint r[3];
    switch (s.mode) {
    case -3:
    case -5:
    case -7:
        while (!mOper.empty() && compare(s)) {
            uint oper = mOper.back();
            r[0] = node(tag_expression, oper);
            if (grammar.oper[oper].postfix || grammar.oper[oper].prefix) {
                r[1] = mOprn.back();
                mOprn.pop_back();
                insert(r[0], r[1]);
            } else {
                r[1] = mOprn.back();
                mOprn.pop_back();
                r[2] = mOprn.back();
                mOprn.pop_back();
                insert(r[0], r[2]);
                insert(r[0], r[1]);
            }
            mOper.pop_back();
            mOprn.push_back(r[0]);
        }
        return;
    case -6: {
        r[0] = mOprn.back();
        mOprn.pop_back();
        while (!mOper.empty() && grammar.oper[mOper.back()].postfix) {
            r[1] = node(tag_expression, mOper.back());
            mOper.pop_back();
            insert(r[1], r[0]);
            r[0] = r[1];
        }
        insert(s.op, r[0]);
    }
    return;
    }
}
bool Parser::compare(Frame &s) {
    if (mOper.size() == s.offset)
		return false;
    if (s.mode == -7)
		return true;
    if (grammar.oper[s.op].prec < grammar.oper[mOper.back()].prec) 
		return true;
    if (grammar.oper[s.op].prec == grammar.oper[mOper.back()].prec)
		return grammar.oper[s.op].left;
    return false;
}
uint Parser::expression() {
    uint r[1];
    Frame s(mOper.size(), -1);
    while (s.mode != -7) {
        shift(s);
        reduce(s);
        switch (s.mode) {
        case -2:
        case -3:
        case -5:
            mOper.push_back(s.op);
            break;
        case -4:
        case -6:
            mOprn.push_back(s.op);
            break;
        }
    }
    r[0] = mOprn.back();
    mOprn.pop_back();
    return r[0];
}

/*
suffix
: [suffix] (LBRACKET sequence? RBRACKET)*
;
*/
uint Parser::suffix() {
    uint r[3];
    r[0] = node(tag_suffix, -1);
    while (mLookahead == LBRACKET) {
        accept();
        if (mLookahead == RBRACKET) {
            accept();
            r[1] = node(tag_suffix, -2);
            insert(r[0], r[1]);
            continue;
        }
        r[1] = sequence();
        match(RBRACKET);
        r[2] = node(tag_suffix, -3);
        insert(r[2], r[1]);
        insert(r[0], r[2]);
    }
    return r[0];
}

/*
primary
: INTEGER
| FLOAT
| STRING
| LPAREN expression RPAREN
| brace
| backquote
| symbol
| blank
| POUND
;
*/
uint Parser::primary() {
    uint r;
    switch (mLookahead) {
    case INTEGER:
        r = node(tag_primary, instr_integer);
        note(r, mText);
        accept();
        return r;
    case FLOAT:
        r = node(tag_primary, instr_float);
        note(r, mText);
        accept();
        return r;
    case STRING:
        r = node(tag_primary, instr_string);
        note(r, mText);
        accept();
        return r;
	case LPAREN: {
        uint t;
        accept();
        t = expression();
        match(RPAREN);
        r = node(tag_primary, -1);
        insert(r, t);
    }
		return r;
    case LBRACE:
        return brace();
    case BACKQUOTE:
        return backquote();
    case SYMBOL:
        return symbol();
    case BLANK:
        return blank();
	case POUND:
		return pound();
	case DOLLAR:
		return dollar();
    }
    //SPACE -> null
    //解释要与null打印同步
    if (mLookback == COMMA || mLookback == SEMICOLON) {
        r = node(tag_primary, instr_null);
        return r;
    }
    mLookahead == EOI ? more() : error();
    return 0;
}

/*
brace
: [brace] LBRACE sequence? RBRACE
;
*/
uint Parser::brace() {
    uint r[2];
    accept();
    if (mLookahead == RBRACE) {
        accept();
        r[0] = node(tag_brace, -1);
    } else {
        r[0] = node(tag_brace, -2);
        r[1] = sequence();
        match(RBRACE);
        insert(r[0], r[1]);
    }
    return r[0];
}

/*
backquote
: [backquote] (BACKQUOTE SYMBOL)+
;
*/
uint Parser::backquote() {
    uint r[2];
    r[0] = node(tag_backquote, -1);
    while (mLookahead == BACKQUOTE) {
        mLookahead = scan();
        if (mLookahead == SYMBOL) {
            r[1] = node(tag_backquote, instr_symbol);
            note(r[1], mText);
            insert(r[0], r[1]);
            mLookahead = scan();
        } else
            error();
    }
    skip();
    return r[0];
}

/*
symbol
: [symbol] SYMBOL backquote?
;
*/
uint Parser::symbol() {
    uint r[3];
	r[0] = node(tag_symbol, instr_symbol);
	note(r[0], mText);
    accept();
    if (mLookahead == BACKQUOTE) {
        r[1] = backquote();
        r[2] = node(tag_symbol, -1);
        insert(r[2], r[0]);
        insert(r[2], r[1]);
        return r[2];
	}
    return r[0];
}

/*
blank
: [blank] BLANK (backquote | symbol)?
;
*/
uint Parser::blank() {
	uint r[2];
	mLookahead = scan();
	switch (mLookahead) {
	case BACKQUOTE:
		r[1] = backquote();
		r[0] = node(tag_blank, instr_symbol);
		insert(r[0], r[1]);
		break;
	case SYMBOL:
		r[1] = symbol();
		r[0] = node(tag_blank, instr_symbol);
		insert(r[0], r[1]);
		break;
	default:
		skip();
		r[0] = node(tag_blank, -1);
		break;
	}
	return r[0];
}

/*
pound
: [pound] POUND (INTEGER | STRING | SYMBOL)?
;
*/
uint Parser::pound() {
	uint r[2];
	mLookahead = scan();
	switch (mLookahead) {
	case INTEGER:
		r[0] = node(tag_pound, -1);
		note(r[0], mText);
		accept();
		break;
	case STRING:
	case SYMBOL:
		r[0] = node(tag_pound, -2);
		note(r[0], mText);
		accept();
		break;
	default:
		skip();
		r[0] = node(tag_pound, instr_key);
		break;
	}
	return r[0];
}

/*
dollar
: [dollar] DOLLAR (STRING | SYMBOL)?
;
*/
uint Parser::dollar() {
	uint r[2];
	mLookahead = scan();
	switch (mLookahead) {
	case STRING:
	case SYMBOL:
		r[0] = node(tag_dollar, -1);
		note(r[0], mText);
		accept();
		break;
	default:
		skip();
		r[0] = node(tag_dollar, instr_symbol);
		break;
	}
	return r[0];
}
}
