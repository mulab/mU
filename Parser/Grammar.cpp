#include <mU/Grammar.h>

namespace mU {
bool Grammar::Inited = false;
void Grammar::Init() {
    if (Inited)
        return;
    grammar.operSymbol.reserve(grammar.oper.size());
    for (uint i = 0; i < grammar.oper.size(); ++i) {
        sym t = sys->symbol(wstr(grammar.oper[i].name));
        if (grammar.oper[i].postfix)
            grammar.postfixSymbol[t] = i;
        else if (grammar.oper[i].prefix)
            grammar.prefixSymbol[t] = i;
        else
            grammar.infixSymbol[t] = i;
        grammar.operSymbol.push_back(t);
    }
    Inited = true;
}
Grammar::Grammar() {
#define M(token,name,show,...) oper.push_back(Oper(token,_W(#name),_W(show),##__VA_ARGS__))
	M(SEMICOLON, Serial, ";");
	++prec;
	M(QUOTE, Unevaluated, "'", false, true);
	M(AND_OP, Delayed, "&", false, true);
	++prec;
	M(LEFT_OP, Input, "<<", false, true);
    M(RIGHT_OP, Output, ">>");
    M(RIGHT_GT, OutputAppend, ">>>");
    ++prec;
    M(ASSIGN, Set, "=");
    M(COLON_ASSIGN, SetDelayed, ":=");
    M(POW_ASSIGN, UpSet, "^=");
    M(POW_COLON_ASSIGN, UpSetDelayed, "^:=");
    M(ASSIGN_PERIOD, Unset, "=.", true);
    ++prec;
    M(SLASH_SLASH, Postfix, "//");
    ++prec;
    M(AND_OP, Function, "&", true);
    ++prec;
    M(ADD_ASSIGN, AddTo, "+=");
    M(SUB_ASSIGN, SubtractFrom, "-=");
    M(MUL_ASSIGN, TimesBy, "*=");
    M(DIV_ASSIGN, DivideBy, "/=");
    M(MOD_ASSIGN, ModBy, "%=");
    ++prec;
    M(ASSIGN_GT, Function, "=>");
    ++prec;
    M(SLASH_PERIOD, ReplaceAll, "/.");
    M(SLASH_SLASH_PERIOD, ReplaceRepeated, "//.");
    ++prec;
    M(PTR_OP, Rule, "->");
    M(COLON_GT, RuleDelayed, ":>");
    ++prec;
    M(SLASH_SEMICOLON, Condition, "/;");
    ++prec;
    M(TILDE_TILDE, StringExpression, "~~");
    ++prec;
    M(OR_OP, Alternatives, "|");
    ++prec;
    M(BOOL_OR_OP, Or, "||");
    ++prec;
    M(BOOL_AND_OP, And, "&&");
    ++prec;
    M(BANG, Not, "!", false, true);
    ++prec;
    M(ASSIGN_EQ, SameQ, "===");
    M(ASSIGN_NE, UnsameQ, "=!=");
    ++prec;
    M(EQ_OP, Equal, "==");
    M(LE_OP, LessEqual, "<=");
    M(GE_OP, GreaterEqual, ">=");
    M(NE_OP, Unequal, "!=");
    M(LT_OP, Less, "<");
    M(GT_OP, Greater, ">");
    ++prec;
    M(PLUS, Plus, "+");
    ++prec;
    M(STAR, Times, "*");
    ++prec;
    M(SLASH, Divide, "/", false, false, true);
    ++prec;
    M(PERCENT, Mod, "%");
    ++prec;
    M(PLUS, Plus, "+", false, true);
    M(MINUS, Minus, "-", false, true);
	M(STAR, New, "*", false, true);
    ++prec;
    M(CIRCUMFLEX, Power, "^");
    ++prec;
    M(LT_GT, StringJoin, "<>");
    ++prec;
    M(TILDE, Join, "~");
    ++prec;
    M(QUOTE, Differential, "'", true);
    ++prec;
    M(BANG, Factorial, "!", true);
    ++prec;
    M(SLASH_AT, Map, "/@");
    M(SLASH_SLASH_AT, MapAll, "//@");
    M(AT_AT, Apply, "@@");
    ++prec;
    M(AT, Prefix, "@");
    ++prec;
    M(INC_OP, PreIncrement, "++", false, true);
    M(DEC_OP, PreDecrement, "--", false, true);
    ++prec;
    M(INC_OP, Increment, "++", true);
    M(DEC_OP, Decrement, "--", true);
    ++prec;
    M(QUESTIONMARK, PatternTest, "?");
    M(COLON, Pattern, ":");
	++prec;
    M(PERIOD, Slot, ".", false, false, true);
    ++prec;
#undef M
    for (uint i = 0; i < oper.size(); ++i) {
        if (oper[i].postfix)
            postfixToken[oper[i].token] = i;
        else if (oper[i].prefix)
            prefixToken[oper[i].token] = i;
        else
            infixToken[oper[i].token] = i;
    }
	operCOLON = infixToken[COLON];
	operPERIOD = infixToken[PERIOD];
	operPLUS = infixToken[PLUS];
	operSTAR = infixToken[STAR];
    end.insert(EOI);
    end.insert(COMMA);
    end.insert(RBRACKET);
    end.insert(RPAREN);
    end.insert(RBRACE);
/*#include "character.txt"
	for (uint i = 0; i < character.size(); ++i) {
        unicode[character[i].unicode] = i;
        named[character[i].named] = i;
    }
*/
}
Grammar grammar;
Grammar::Oper::Oper(Token $token, wcs $name, wcs $show, bool $postfix, bool $prefix, bool $left) :
token($token), name($name), show($show), prec(grammar.prec), postfix($postfix), prefix($prefix), left($left) {}
}
