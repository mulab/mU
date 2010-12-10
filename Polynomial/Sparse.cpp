#include "Sparse.h"

namespace mU {
namespace Polynomial {
Exponent& exponent(const Tuple& x) {
	if (isSparse(x))
		return x[x.size - 1].cast<Exponent>();
	Exponent* e = new Exponent(x.size - 2);
	for (uint i = 0; i < x.size - 2; ++i)
		e->exponent[i] = i;
	return *e;
} 
Tuple* Poly(const sparse_t& x) {
	if (x.size() < ((x.back().first + 1) >> 1)) {
		Exponent* re = new Exponent(x.size());
		for (uint i = 0; i < x.size(); ++i)
			re->exponent[i] = x[i].first;
		Tuple* r = tuple(x.size() + 2);
		r->tuple[0] = poly;
		r->tuple[x.size() + 1] = re;
		for (uint i = 0; i < x.size(); ++i)
			r->tuple[i + 1] = x[i].second;
		return r;
	}
	Tuple* r = tuple(x.back().first + 3);
	r->tuple[0] = poly;
	for (uint i = 0; i < x.size(); ++i)
		r->tuple[x[i].first + 1] = x[i].second;
	return r;
}
}
}