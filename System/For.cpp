#include <mU/System.h>

using namespace mU;

CAPI void VALUE(And)(Kernel& k, var& r, Tuple& x) {
	bool flag = true;
	for (uint i = 1; i < x.size; ++i) {
		x[i] = k.eval(x[i]);
		if (x[i] == $.True)
			continue;
		if (x[i] == $.False) {
			r = $.False;
			return;
		}
		flag = false;
	}
	if (flag)
		r = $.True;
}
CAPI void VALUE(For)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 5) {
		for (k.eval(x[1]);
			k.eval(x[2]) == $.True;
			k.eval(x[3])) {
			r = k.eval(x[4]);
			if (r.isTuple() && r.tuple()[0].isSymbol()) {
				sym h = r.tuple()[0].symbol();
				if (h == $.Return) 
					return;
				if (h == $.Break)
					break;
				if (h == $.Continue)
					continue;
			}
		}
	}
}
CAPI void VALUE(If)(Kernel& k, var& r, Tuple& x) {
	if (x.size >= 3) {
		if (x[1] == $.True) {
			r = k.eval(x[2]);
			return;
		}
		if (x[1] == $.False && x.size >= 4) {
			r = k.eval(x[3]);
			return;
		} 
	}
}
CAPI void VALUE(Not)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		if (x[1] == $.True) {
			r = $.False;
			return;
		}
		if (x[1] == $.False) {
			r = $.True;
			return;
		}
	}	
}
CAPI void VALUE(Or)(Kernel& k, var& r, Tuple& x) {
	bool flag = true;
	for (uint i = 1; i < x.size; ++i) {
		x[i] = k.eval(x[i]);
		if (x[i] == $.False)
			continue;
		if (x[i] == $.True) {
			r = $.True;
			return;
		}
		flag = false;
	}
	if (flag)
		r = $.False;
}
CAPI void VALUE(Serial)(Kernel& k, var& r, Tuple& x) {
	for (uint i = 1; i < x.size; ++i) {
		r = k.eval(x[i]);
		if (r.isTuple() && r.tuple()[0].isSymbol()) {
			sym h = r.tuple()[0].symbol();
			if (h == $.Return) {
				r = r.tuple().size == 1 ? null : r.tuple()[1];
				return;
			}
			if (h == $.Break || h == $.Continue)
				break;
		}
	}
}
CAPI void VALUE(While)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3) {
		while (k.eval(x[1]) == $.True) {
			r = k.eval(x[2]);
			if (r.isTuple() && r.tuple()[0].isSymbol()) {
				sym h = r.tuple()[0].symbol();
				if (h == $.Return) 
					return;
				if (h == $.Break)
					break;
				if (h == $.Continue)
					continue;
			}
		}
	}
}