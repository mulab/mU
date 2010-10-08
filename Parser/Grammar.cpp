#include <mU/Parser.h>

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
#define M(...) oper.push_back(Parser::Oper(__VA_ARGS__));
	M(Parser::SEMICOLON, _W("Serial"), _W(";"))
	++prec;
	M(Parser::DOLLAR, _W("New"), _W("$"), false, true)
    M(Parser::QUOTE, _W("Unevaluated"), _W("'"), false, true)
	M(Parser::AND_OP, _W("Delayed"), _W("&"), false, true)
    ++prec;
    M(Parser::LEFT_OP, _W("In"), _W("<<"), false, true)
    M(Parser::RIGHT_OP, _W("Out"), _W(">>"))
    M(Parser::RIGHT_GT, _W("OutAppend"), _W(">>>"))
    ++prec;
    M(Parser::ASSIGN, _W("Set"), _W("="))
    M(Parser::COLON_ASSIGN, _W("SetDelayed"), _W(":="))
    M(Parser::POW_ASSIGN, _W("UpSet"), _W("^="))
    M(Parser::POW_COLON_ASSIGN, _W("UpSetDelayed"), _W("^:="))
    M(Parser::ASSIGN_PERIOD, _W("Unset"), _W("=."), true)
    ++prec;
    M(Parser::SLASH_SLASH, _W("Postfix"), _W("//"))
    ++prec;
    M(Parser::AND_OP, _W("Function"), _W("&"), true)
    ++prec;
    M(Parser::ADD_ASSIGN, _W("AddTo"), _W("+="))
    M(Parser::SUB_ASSIGN, _W("SubtractFrom"), _W("-="))
    M(Parser::MUL_ASSIGN, _W("TimesBy"), _W("*="))
    M(Parser::DIV_ASSIGN, _W("DivideBy"), _W("/="))
    M(Parser::MOD_ASSIGN, _W("ModBy"), _W("%="))
    ++prec;
    M(Parser::ASSIGN_GT, _W("Function"), _W("=>"))
    ++prec;
    M(Parser::SLASH_PERIOD, _W("ReplaceAll"), _W("/."))
    M(Parser::SLASH_SLASH_PERIOD, _W("ReplaceRepeated"), _W("//."))
    ++prec;
    M(Parser::PTR_OP, _W("Rule"), _W("->"))
    M(Parser::COLON_GT, _W("RuleDelayed"), _W(":>"))
    ++prec;
    M(Parser::SLASH_SEMICOLON, _W("Condition"), _W("/;"))
    ++prec;
    M(Parser::TILDE_TILDE, _W("StringExpression"), _W("~~"))
    ++prec;
    M(Parser::OR_OP, _W("Alternatives"), _W("|"))
    ++prec;
    M(Parser::BOOL_OR_OP, _W("Or"), _W("||"))
    ++prec;
    M(Parser::BOOL_AND_OP, _W("And"), _W("&&"))
    ++prec;
    M(Parser::BANG, _W("Not"), _W("!"), false, true)
    ++prec;
    M(Parser::ASSIGN_EQ, _W("SameQ"), _W("==="))
    M(Parser::ASSIGN_NE, _W("UnsameQ"), _W("=!="))
    ++prec;
    M(Parser::EQ_OP, _W("Equal"), _W("=="))
    M(Parser::LE_OP, _W("LessEqual"), _W("<="))
    M(Parser::GE_OP, _W("GreaterEqual"), _W(">="))
    M(Parser::NE_OP, _W("Unequal"), _W("!="))
    M(Parser::LT_OP, _W("Less"), _W("<"))
    M(Parser::GT_OP, _W("Greater"), _W(">"))
    ++prec;
    M(Parser::PLUS, _W("Plus"), _W("+"))
    ++prec;
    M(Parser::STAR, _W("Times"), _W("*"))
    ++prec;
    M(Parser::SLASH, _W("Divide"), _W("/"), false, false, true)
    ++prec;
    M(Parser::PERCENT, _W("Mod"), _W("%"))
    ++prec;
    M(Parser::PLUS, _W("Plus"), _W("+"), false, true)
    M(Parser::MINUS, _W("Minus"), _W("-"), false, true)
    ++prec;
    M(Parser::CIRCUMFLEX, _W("Power"), _W("^"))
    ++prec;
    M(Parser::LT_GT, _W("StringJoin"), _W("<>"))
    ++prec;
    M(Parser::TILDE, _W("Join"), _W("~"))
    ++prec;
    M(Parser::QUOTE, _W("Differential"), _W("'"), true)
    ++prec;
    M(Parser::BANG, _W("Factorial"), _W("!"), true)
    ++prec;
    M(Parser::SLASH_AT, _W("Map"), _W("/@"))
    M(Parser::SLASH_SLASH_AT, _W("MapAll"), _W("//@"))
    M(Parser::AT_AT, _W("Apply"), _W("@@"))
    ++prec;
    M(Parser::AT, _W("Prefix"), _W("@"))
    ++prec;
    M(Parser::INC_OP, _W("PreIncrement"), _W("++"), false, true)
    M(Parser::DEC_OP, _W("PreDecrement"), _W("--"), false, true)
    ++prec;
    M(Parser::INC_OP, _W("Increment"), _W("++"), true)
    M(Parser::DEC_OP, _W("Decrement"), _W("--"), true)
    ++prec;
    M(Parser::QUESTIONMARK, _W("PatternTest"), _W("?"))
    M(Parser::COLON, _W("Pattern"), _W(":"))
	++prec;
    M(Parser::PERIOD, _W("Slot"), _W("."), false, false, true)
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
    end.insert(Parser::EOI);
    end.insert(Parser::COMMA);
    end.insert(Parser::RBRACKET);
    end.insert(Parser::RPAREN);
    end.insert(Parser::RBRACE);
	//end.insert(Parser::SEMICOLON);
#include "character.txt"
	for (uint i = 0; i < character.size(); ++i) {
        unicode[character[i].unicode] = i;
        named[character[i].named] = i;
    }
}
Grammar grammar;
}