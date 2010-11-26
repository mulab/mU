#include "Sparse.h"

namespace mU {
namespace Polynomial {
var Add(Kernel& k, const Tuple& x, const Tuple& y) {
	sparse_t r;
	Exponent &xe = exponent(x), &ye = exponent(y);
	uint xi = 1, yi = 1;
	while (xi < x.size - 1 && yi < y.size - 1) {
		if (xe[xi] < ye[yi]) {
			r.push_back(std::make_pair(xe[xi], x[xi]));
			++xi;
		}
		else if (xe[xi] > ye[yi]) {
			r.push_back(std::make_pair(ye[yi], y[yi]));
			++yi;
		}
		else {
			var t = Plus(k, x[xi], y[yi]);
			if (!t.isObject() || cmpD(t.object(), 0.0)) {
				r.push_back(std::make_pair(xe[xi], t));
			}
			++xi;
			++yi;
		}
	} 
	while (xi < x.size - 1) {
		r.push_back(std::make_pair(xe[xi], x[xi]));
		++xi;
	}
	while (yi < y.size - 1) {
		r.push_back(std::make_pair(ye[yi], y[yi]));
		++yi;
	}
	if (!isSparse(x))
		delete &xe;
	if (!isSparse(y))
		delete &ye;
	return Poly(r);
}
}
}

using namespace mU;

CAPI void VALUE(Add)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1 || !x[1].isTuple(Polynomial::poly))
		return;
	var c = x[1];
	for (uint i = 2; i < x.size; ++i)
		if (x[i].isTuple(Polynomial::poly))
			c = Polynomial::Add(k, c.tuple(), x[i].tuple());
	r = c;
}