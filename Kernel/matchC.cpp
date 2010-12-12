#include <mU/Match.h>

namespace mU {
namespace {
struct MatchC {
	MatchC(Kernel& k) : mKernel(k), mMatch(new CMatch()) {}
	Kernel& mKernel;
	CMatch* mMatch;
	std::stack<sym> mFlat;
	sym flat() {
		return mFlat.empty() ? 0 : mFlat.top();
	}
	void terminal(const var& x) {
		mMatch->codes.push_back(CMatch::TERMINAL);
		mMatch->args.push_back(x);
	}
	void blank_any() {
		mMatch->codes.push_back(CMatch::BLANK_ANY);
	}
	void blank(sym x) {
		mMatch->codes.push_back(CMatch::BLANK);
		mMatch->args.push_back(x);
	}
	void test(const var& x) {
		mMatch->codes.push_back(CMatch::TEST);
		mMatch->args.push_back(x);
	}
	void variable(const var& x) {
		mMatch->codes.push_back(CMatch::VARIABLE);
		mMatch->args.push_back(x);
	}
	void non_terminal(const var& x) {
		mMatch->codes.push_back(CMatch::NON_TERMINAL);
		mMatch->args.push_back(x);
	}
	void unorder(const Tuple& x) {
		mMatch->codes.push_back(CMatch::UNORDER);
		CMatch::Unorder* r = new CMatch::Unorder();
		mMatch->args.push_back(r);
		for (uint i = 1; i < x.size; ++i) {
			if (isCertain(x[i]))
				r->mset.insert(x[i]);
			else
				compile(x[i]);
		}
	}
	void sequence(const var& x) {
		mMatch->codes.push_back(CMatch::SEQUENCE);
		mMatch->args.push_back(x);
	}
	void rule(const var& x) {
		mMatch->codes.push_back(CMatch::RULE);
		mMatch->args.push_back(x);
	}
	void serial(const Tuple& x) {
		mMatch->codes.push_back(CMatch::SERIAL);
		Tuple* r = tuple(x.size - 1);
		mMatch->args.push_back(r);
		for (uint i = 1; i < x.size; ++i)
			r->tuple[i - 1] = cmatch(mKernel, x[i]);		
	}
	void match(const Match& x) {
		mMatch->codes.push_back(CMatch::MATCH);
		mMatch->args.push_back(&x);
	}
	void expression(const Tuple& x) {
		bool ordered = true;
		var h = x[0];
		if (h.isSymbol()) {
			if (x.size == 1) {
				if (h == $.Blank) {
					if (flat())
						sequence(flat());
					else
						blank_any();
					return;
				}
			} else {
				if (h == $.Alternatives) {
					serial(x);
					return;
				}
				if (h == $.Production) {
					non_terminal(x[1]);
					return;
				}
				if (x[1].isSymbol()) {
					if (h == $.Blank) {
						blank(x[1].symbol());
						return;
					}
				}
				if (x.size >= 3) {
					if (h == $.Pattern) {
						compile(x[2]);
						variable(x[1]);
						return;
					}
					if (h == $.Rule) {
						compile(x[1]);
						rule(x[2]);
						return;
					}
					if (h == $.RuleDelayed) {
						compile(x[1]);
						rule(new Tag($.Delayed, x[2]));
						return;
					}
					if (h == $.Condition) {
						compile(x[1]);
						test(x[2]);
						return;
					}
				}
			}
			mFlat.push(0);
			mMatch->codes.push_back(CMatch::PUSH);
			terminal(h);
			std::unordered_map<sym, Kernel::Attribute>::const_iterator
				iter = mKernel.attributes.find(h.symbol());
			if (iter != mKernel.attributes.end()) {
				if (iter->second.count($.Flat))
					mFlat.top() = h.symbol();
				if (iter->second.count($.Orderless)) {
					ordered = false;
					unorder(x);
				}
			}
		}	
		else {
			mFlat.push(0);
			mMatch->codes.push_back(CMatch::PUSH);
			compile(h);
		}
		if (ordered) {
			for (uint i = 1; i < x.size; ++i)
				compile(x[i]);
		}
		mFlat.pop();
		mMatch->codes.push_back(CMatch::POP);
	}
	void compile(const var& x) {
		switch (x.primary()) {
			case Primary::Object:
				if (x.object().type == $.Match) {
					match(x.cast<Match>());
					return;
				}
				break;
			case Primary::Tuple:
				if (!isCertain(x.tuple())) {
					expression(x.tuple());
					return;
				}
				break;
		}
		terminal(x);
	}
};
}
Match* cmatch(Kernel& k, const var& x) {
	MatchC m(k);
	m.compile(x);
	return m.mMatch;
}
Match* crule(Kernel& k, const var& x, const var& y) {
	MatchC m(k);
	m.compile(x);
	m.rule(y);
	return m.mMatch;
}
Match* ctest(Kernel& k, const var& x, const var& y) {
	MatchC m(k);
	m.compile(x);
	m.test(y);
	return m.mMatch;
}
Match* cserial(Kernel& k, const Tuple& x) {
	MatchC m(k);
	m.serial(x);
	return m.mMatch;
}
}
