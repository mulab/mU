#pragma once
#include <mU/Polynomial.h>

namespace mU {
namespace Polynomial {
inline bool isSparse(const Tuple& x) {
	return x[x.size - 1];
}
class Exponent : public Object {
public:
	uint* exponent;
	virtual ~Exponent() {
		delete exponent;
	}
	Exponent(uint n) {
		exponent = new uint[n];
	}
	const uint operator[](uint i) const {
		return exponent[i - 1];
	}
	uint& operator[](uint i) {
		return exponent[i - 1];
	}
};
Exponent& exponent(const Tuple&);
typedef std::vector<std::pair<uint, var> > sparse_t;
Tuple* Poly(const sparse_t&);
}
}
