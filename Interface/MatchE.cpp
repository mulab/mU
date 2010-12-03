#include <mU/Kernel.h>
#include "Match.h"

namespace mU {
namespace {
struct MatchE {
	MatchE(Kernel& k, const CMatch& x, MatchE* y = 0) : mKernel(k), mMatch(x), mNext(y), mCode(0), mArg(0) {}
	Kernel& mKernel;
	const CMatch& mMatch;
	var result;
	uint mCode;
	CMatch::Code code() {
		return mMatch.codes[mCode];
	}
	uint mArg;
	var arg(uint n) {
		return mMatch.args[mArg + n];
	}
	MatchE* mNext;
	bool match(Pos& x) {
		if (mCode == mMatch.codes.size()) {
			if (mNext) {
				mNext->result = result;
				return mNext->match(x);
			}
			return x.empty() && !x.prev;
		}
		switch (code()) {
		case CMatch::TERMINAL: {
				if (x.empty() || *x != arg(0))
					return false;
				bool flag;
				result = *x;
				++x;
				flag = next(x, 1);
				--x;
				return flag;
			}
			break;
		case CMatch::BLANK_ANY: {
				if (x.empty())
					return false;
				bool flag;
				result = *x;
				++x;
				flag = next(x, 0);
				--x;
				return flag;
			}
			break;
		case CMatch::BLANK: {
				if (x.empty() || x->head() != arg(0))
					return false;
				bool flag;
				result = *x;
				++x;
				flag = next(x, 1);
				--x;
				return flag;
			}
			break;
		case CMatch::PUSH: {
				if (x.empty() || !x->isTuple())
					return false;
				Pos t(x->tuple(), &x);
				return next(t, 0);
			}
			break;
		case CMatch::POP: {
				if (!x.empty() || !x.prev)
					return false;
				bool flag;
				result = *(*x.prev);
				++(*x.prev);
				flag = next(*x.prev, 0);
				--(*x.prev);
				return flag;
			}
			break;
		case CMatch::TEST: {
				return mKernel.eval(arg(0)) == $.True && next(x, 1);
			}
			break;
		case CMatch::VARIABLE: {
				if (mKernel.bound(arg(0))) {
					if (result == mKernel.value(arg(0)))
						return next(x, 1);
				} else if (mKernel.bind(arg(0), result)) {
					if (next(x, 1))
						return true;
					mKernel.unbind(arg(0));
				}
				return false;
			}
			break;
		case CMatch::RULE: {
				result = mKernel.lazy(arg(0));
				if (result.isTuple() 
					&& result.tuple()[0] == $.Condition
					&& result.tuple().size == 3) {
					if (mKernel.eval(result.tuple()[2]) != $.True)
						return false;
					result = mKernel.eval(result.tuple()[1]);
				}
				return next(x, 1);
			}
			break;
		case CMatch::NON_TERMINAL: {
				if (x.empty())
					return false;
				var c = mKernel.value(arg(0));
				if (c.isObject() && c.object().type == $.Match) {
					if (cast<Match>(c)(mKernel, result, *x)) {
						++x;
						if (next(x, 1))
							return true;
						--x;
					}
				}
				return false;
			}
			break;
		case CMatch::UNORDER: {
				CMatch::Unorder& c = cast<CMatch::Unorder>(arg(0));
				std::multiset<var> certains;
				Pos x0 = x;
				while (!x.empty()) {
					certains.insert(*x);
					++x;
				}
				std::multiset<var>::const_iterator
					iter = c.certains.begin();
				while (iter != c.certains.end()) {
					if (certains.erase(*iter) == 0)
						return false;
					++iter;
				}
				if (certains.empty()) {
					if (next(x, 1))
						return true;
				} else {
					Tuple* t = mU::tuple(certains.size(), certains.begin());
					var c = t;
					x.ptr = t->tuple;
					x.end = t->tuple + t->size;
					do {
						if (next(x, 1))
							return true;
					} while (std::next_permutation(t->tuple, t->tuple + t->size));
				} 
				x = x0;
				return false;
			}
			break;
		case CMatch::SEQUENCE: {
				std::vector<var> t; 
				uint n = x.size();
				const var* ptr = x.ptr;
				for (uint i = 0; i < n; ++i) {
					t.push_back(ptr[i]);
					x.ptr = ptr + i + 1;
					if (i == 0)
						result = t[0];
					else
						result = mU::list(t.size(), t.begin(), arg(0));
					if (next(x, 1))
						return true;
				}
				x.ptr = ptr;
				return false;
			}
			break;
		case CMatch::LIST: {
				var c = arg(0);
				++mCode;
				mArg += 1;
				for (uint i = 0; i < c.tuple().size; ++i) {
					MatchE m(mKernel, cast<CMatch>(c.tuple()[i]), this);
					if (m.match(x))
						return true;
				}
				--mCode;
				mArg -= 1;
				return false;
			}
			break;
		case CMatch::MATCH: {
				if (x.empty())
					return false;
				if (cast<Match>(arg(0))(mKernel, result, *x)) {
					++x;
					if (next(x, 1))
						return true;
					--x;
				}
				return false;
			}
			break;
		}
		return false;
	}
	bool next(Pos& x, uint n) {
		++mCode;
		mArg += n;
		bool flag = match(x);
		--mCode;
		mArg -= n;
		return flag;
	}
};
}
bool CMatch::operator()(Kernel& k, var& r, const var& x) {
	MatchE m(k, *this);
	Pos p(x, 0);
	if (m.match(p)) {
		r = m.result;
		return true;
	}
	return false;
}
}
