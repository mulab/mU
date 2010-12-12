#include "Kernel.h"

namespace mU {
class CMatch : public Match {
public:
	enum Code {
		TERMINAL, BLANK_ANY, BLANK, TEST, PUSH, POP, VARIABLE,
		RULE, NON_TERMINAL, UNORDER, SEQUENCE, SERIAL, MATCH
	};
	std::vector<Code> codes;
	std::vector<var> args;
	class Unorder : public Object {
	public:
		typedef std::unordered_multiset<var> UMSet;
		UMSet mset;
	};
	API virtual bool operator()(Kernel&, var&, const var&);
};
API Match* cmatch(Kernel&, const var&);
API Match* crule(Kernel&, const var&, const var&);
API Match* ctest(Kernel&, const var&, const var&);
API Match* cserial(Kernel&, const Tuple&);
}