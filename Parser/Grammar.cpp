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
#define M(...) oper.push_back(Oper(__VA_ARGS__))
	M(SEMICOLON, _W("Serial"), _W(";"));
	++prec;
	M(DOLLAR, _W("New"), _W("$"), false, true);
	M(QUOTE, _W("Unevaluated"), _W("'"), false, true);
	M(AND_OP, _W("Delayed"), _W("&"), false, true);
	++prec;
	M(LEFT_OP, _W("Input"), _W("<<"), false, true);
    M(RIGHT_OP, _W("Output"), _W(">>"));
    M(RIGHT_GT, _W("OutputAppend"), _W(">>>"));
    ++prec;
    M(ASSIGN, _W("Set"), _W("="));
    M(COLON_ASSIGN, _W("SetDelayed"), _W(":="));
    M(POW_ASSIGN, _W("UpSet"), _W("^="));
    M(POW_COLON_ASSIGN, _W("UpSetDelayed"), _W("^:="));
    M(ASSIGN_PERIOD, _W("Unset"), _W("=."), true);
    ++prec;
    M(SLASH_SLASH, _W("Postfix"), _W("//"));
    ++prec;
    M(AND_OP, _W("Function"), _W("&"), true);
    ++prec;
    M(ADD_ASSIGN, _W("AddTo"), _W("+="));
    M(SUB_ASSIGN, _W("SubtractFrom"), _W("-="));
    M(MUL_ASSIGN, _W("TimesBy"), _W("*="));
    M(DIV_ASSIGN, _W("DivideBy"), _W("/="));
    M(MOD_ASSIGN, _W("ModBy"), _W("%="));
    ++prec;
    M(ASSIGN_GT, _W("Function"), _W("=>"));
    ++prec;
    M(SLASH_PERIOD, _W("ReplaceAll"), _W("/."));
    M(SLASH_SLASH_PERIOD, _W("ReplaceRepeated"), _W("//."));
    ++prec;
    M(PTR_OP, _W("Rule"), _W("->"));
    M(COLON_GT, _W("RuleDelayed"), _W(":>"));
    ++prec;
    M(SLASH_SEMICOLON, _W("Condition"), _W("/;"));
    ++prec;
    M(TILDE_TILDE, _W("StringExpression"), _W("~~"));
    ++prec;
    M(OR_OP, _W("Alternatives"), _W("|"));
    ++prec;
    M(BOOL_OR_OP, _W("Or"), _W("||"));
    ++prec;
    M(BOOL_AND_OP, _W("And"), _W("&&"));
    ++prec;
    M(BANG, _W("Not"), _W("!"), false, true);
    ++prec;
    M(ASSIGN_EQ, _W("SameQ"), _W("==="));
    M(ASSIGN_NE, _W("UnsameQ"), _W("=!="));
    ++prec;
    M(EQ_OP, _W("Equal"), _W("=="));
    M(LE_OP, _W("LessEqual"), _W("<="));
    M(GE_OP, _W("GreaterEqual"), _W(">="));
    M(NE_OP, _W("Unequal"), _W("!="));
    M(LT_OP, _W("Less"), _W("<"));
    M(GT_OP, _W("Greater"), _W(">"));
    ++prec;
    M(PLUS, _W("Plus"), _W("+"));
    ++prec;
    M(STAR, _W("Times"), _W("*"));
    ++prec;
    M(SLASH, _W("Divide"), _W("/"), false, false, true);
    ++prec;
    M(PERCENT, _W("Mod"), _W("%"));
    ++prec;
    M(PLUS, _W("Plus"), _W("+"), false, true);
    M(MINUS, _W("Minus"), _W("-"), false, true);
    ++prec;
    M(CIRCUMFLEX, _W("Power"), _W("^"));
    ++prec;
    M(LT_GT, _W("StringJoin"), _W("<>"));
    ++prec;
    M(TILDE, _W("Join"), _W("~"));
    ++prec;
    M(QUOTE, _W("Differential"), _W("'"), true);
    ++prec;
    M(BANG, _W("Factorial"), _W("!"), true);
    ++prec;
    M(SLASH_AT, _W("Map"), _W("/@"));
    M(SLASH_SLASH_AT, _W("MapAll"), _W("//@"));
    M(AT_AT, _W("Apply"), _W("@@"));
    ++prec;
    M(AT, _W("Prefix"), _W("@"));
    ++prec;
    M(INC_OP, _W("PreIncrement"), _W("++"), false, true);
    M(DEC_OP, _W("PreDecrement"), _W("--"), false, true);
    ++prec;
    M(INC_OP, _W("Increment"), _W("++"), true);
    M(DEC_OP, _W("Decrement"), _W("--"), true);
    ++prec;
    M(QUESTIONMARK, _W("PatternTest"), _W("?"));
    M(COLON, _W("Pattern"), _W(":"));
	++prec;
    M(PERIOD, _W("Slot"), _W("."), false, false, true);
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
#include "character.txt"
	for (uint i = 0; i < character.size(); ++i) {
        unicode[character[i].unicode] = i;
        named[character[i].named] = i;
    }
}
Grammar grammar;
Grammar::Oper::Oper(Token $token, wcs $name, wcs $show, bool $postfix, bool $prefix, bool $rassoc) :
token($token), name($name), show($show), prec(grammar.prec), postfix($postfix), prefix($prefix), rassoc($rassoc) {}
}
