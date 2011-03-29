#include <mU/Kernel.h>
#include <mU/Parser.h>

namespace mU {
//////////////////////////////////////
void parser::emit(Var x)
{
	if(code_stack.empty())
	{
		m_code.push_back(x);
		return;
	}
	std::pair<Var,size_t> &c = code_stack.top();
	if(VecQ(c.first))
	{
		At(c.first,c.second) = x;
		if(++c.second == Size(c.first))
			code_stack.pop();
	}
	else
	{
		if(c.second == 0)
			Head(c.first) = x;
		else
			Part(c.first,c.second - 1) = x;
		if(c.second == Len(c.first))
			code_stack.pop();
		else
			++c.second;
	}
}
#define sym(x) emit(Sym2(x))
void parser::head(size_t n)
{
	var r = Ex(var(),Vec(n - 1));
	emit(r);
	code_stack.push(std::make_pair(r,0));
}
parser::iter_t parser::lower(size_t i)
{
	return m_child.lower_bound(i);
}
parser::iter_t parser::upper(size_t i)
{
	return m_child.upper_bound(i);
}
size_t parser::count(size_t i)
{
	return m_child.count(i);
}
void parser::gen(size_t m)
{
	node_t& n = m_node[m];
	parser::iter_t iter = lower(m);

	switch(n.tag)
	{
	case tag_sequence:
		{
			parser::iter_t end = upper(m);
			while(iter != end)
			{
				gen(iter->second);
				++iter;
			}
		}
		break;
	case tag_expression:
		if(s_oper[n.value].postfix || s_oper[n.value].prefix)
		{
			head(2);
			sym(s_oper[n.value].symbol);
			gen(iter->second);
			break;
		}
		head(3);
		sym(s_oper[n.value].symbol);
		gen(iter->second);
		gen((++iter)->second);
		break;
	case tag_suffix:
		switch(n.value)
		{
		case -1:
			{
				parser::iter_t begin = lower(m), end = upper(m);
				iter = --end;
				while(iter != begin)
				{
					--iter;
					gen(iter->second);
				}
				gen(end->second);
				while(iter != end)
				{
					gen(iter->second);
					++iter;
				}
			}
			break;
		case -2:
			head(2);
			sym(L"Part");
			n.value = -6;
			break;
		case -3:
			head(count(iter->second) + 2);
			sym(L"Part");
			n.value = -7;
			break;
		case -4:
			head(1);
			n.value = -8;
			break;
		case -5:
			head(count(iter->second) + 1);
			n.value = -9;
			break;
		case -6: break;
		case -7: gen(iter->second); break;
		case -8: break;
		case -9: gen(iter->second);	break;
		}
		break;
	case tag_primary:
		switch(n.value)
		{
		case instr_symbol:
			sym(m_note.find(m)->second);
			break;
		case instr_integer:
			{
				const wchar *s = m_note.find(m)->second;
				emit(Int(string(s,s + wcslen(s)).c_str(),10));
			}
			break;
		case instr_float:
			{
				const wchar *s = m_note.find(m)->second;
				emit(Flt(string(s,s + wcslen(s)).c_str(),10));
			}
			break;
		case instr_string:
			emit(Str(m_note.find(m)->second));
			break;
		case -1:
			emit(Vec());
			break;
		case -2:
			{
				var r = Vec(count(iter->second));
				emit(r);
				code_stack.push(std::make_pair(r,0));
				gen(iter->second);
			}
			break;
		case -3:
			gen(iter->second);
			break;
		case -4:
			head(2);
			sym(L"Blank");
			gen(iter->second);
			break;
		case -5:
			head(2);
			sym(L"Optional");
			head(1);
			sym(L"Blank");
			break;
		case -6:
			head(1);
			sym(L"Blank");
			break;
		case -7:
			head(2);
			sym(L"BlankSequence");
			gen(iter->second);
			break;
		case -8:
			head(2);
			sym(L"Optional");
			head(1);
			sym(L"BlankSequence");
			break;
		case -9:
			head(1);
			sym(L"BlankSequence");
			break;
		case -10:
			head(2);
			sym(L"Slot");
			gen(iter->second);
			break;
		case -11:
			head(2);
			sym(L"Slot");
			emit(Int(1L));
			break;
		case -12:
			head(2);
			sym(L"SlotSequence");
			gen(iter->second);
			break;
		case -13:
			head(2);
			sym(L"SlotSequence");
			emit(Int(1L));
			break;
		case -14:
			head(3);
			sym(L"Pattern");
			gen(iter->second);
			head(2);
			sym(L"Blank");
			gen((++iter)->second);
			break;
		case -15:
			head(2);
			sym(L"Optional");
			head(3);
			sym(L"Pattern");
			gen(iter->second);
			head(1);
			sym(L"Blank");
			break;
		case -16:
			head(3);
			sym(L"Pattern");
			gen(iter->second);
			head(1);
			sym(L"Blank");
			break;
		case -17:
			head(3);
			sym(L"Pattern");
			gen(iter->second);
			head(2);
			sym(L"BlankSequence");
			gen((++iter)->second);
			break;
		case -18:
			head(2);
			sym(L"Optional");
			head(3);
			sym(L"Pattern");
			gen(iter->second);
			head(1);
			sym(L"BlankSequence");
			break;
		case -19:
			head(3);
			sym(L"Pattern");
			gen(iter->second);
			head(1);
			sym(L"BlankSequence");
			break;
		case -20:
			head(3);
			sym(L"Pattern");
			gen(iter->second);
			gen((++iter)->second);
			break;
		case -21:
			head(3);
			sym(L"Property");
			gen(iter->second);
			gen((++iter)->second);
			break;
		}
		break;
	}
}
//////////////////////////////////////
}
