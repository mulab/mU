#include <mU/Interface.h>

namespace mU {
class CMatch : public Match {
public:
	enum Code {
		TERMINAL, BLANK_ANY, BLANK, TEST, PUSH, POP, VARIABLE,
		RULE, NON_TERMINAL, UNORDER, SEQUENCE, LIST, MATCH
	};
	std::vector<Code> codes;
	std::vector<var> args;
	class Unorder : public Object {
	public:
		std::multiset<var> certains;
	};
	virtual bool operator()(Kernel&, var&, const var&);
};
}