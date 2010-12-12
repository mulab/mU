#include <mU/Numeric.h>

namespace mU {
var N(Kernel& k, const var& x, uint n) {
	if (x.isObject()) {
		Real* r = new Real(n);
		if (x.object().type == $.Integer) {
			mpf_set_z(r->mpf, x.cast<Integer>().mpz);
			return r;
		}
		if (x.object().type == $.Rational) {
			mpf_set_q(r->mpf, x.cast<Rational>().mpq);
			return r;
		}
		return x;
	}
	if (x.isSymbol()) {
		return x;
	}
	if (x.isTuple()) {
		var r;
		Tuple* t = x.tuple().clone();
		t->tuple[0] = N(k, t->tuple[0], n);
		if (t->tuple[0].isSymbol()) {
			std::unordered_map<sym, Kernel::Attribute>::const_iterator
				iter = k.attributes.find(t->tuple[0].symbol());
			if (iter != k.attributes.end()) {
				if (!iter->second.count($.NHoldAll)) {
					if (t->size > 1 && !iter->second.count($.NHoldFirst))
						t->tuple[1] = N(k, t->tuple[1], n);
					if (t->size > 2 && !iter->second.count($.NHoldRest)) {
						for (uint i = 2; i < t->size; ++i)
							t->tuple[i] = N(k, t->tuple[i], n);
					}
				}
				r = t;
			}
		}
		if (!r) {
			for (uint i = 1; i < t->size; ++i)
				t->tuple[i] = N(k, t->tuple[i], n);
			r = t;
		}
		return k.eval(r);
	}
	return x;
}
}

using namespace mU;

CAPI void CVALUE(System, N)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2)
		r = N(k, x[1]);
	if (x.size == 3)
		if (isNumber(x[2])) {
			uint n = static_cast<uint>(toD(x[2].object()));
			r = N(k, x[1], n);
			/*
			wcout << L"Prec:" << mpf_get_prec(r->mpf) 
				<< L':'	<< LOG_10_2 * mpf_get_prec(r->mpf)
				<< endl;
				*/
		}
}