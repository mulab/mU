#include "../EclObject.h"
#include <mU/utils.h>

using namespace mU;

#undef ASSIGN
#define ASSIGN(f) CASSIGN(Ecl,f)
#undef VALUE
#define VALUE(f) CVALUE(Ecl,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(Ecl,f,sig)

CAPI void VALUE(Ecl)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[0].isSymbol()) {
		r = new EclObject(to_ecl(x[1]));
		return;
	}
	if (x[0].isTuple(EclObject::$Ecl)) {
	}
}
CAPI void VALUE(Init)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1) {
		char* argv = "";
		EclObject::Init(0, &argv);
		r = null;
		return;
	}
}
CAPI void VALUE(Eval)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject()) {
		if (x[1].object().type == $.String) {
			r = new EclObject(ecl_safe_eval_cstring(wcs2mbs(x[1].cast<String>().str).c_str()));
			return;
		}
		if (x[1].object().type == EclObject::$EclObject) {
			r = new EclObject(ecl_safe_eval(x[1].cast<EclObject>().obj));
			return;
		}
	}
}
CAPI void METHOD(mU, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = from_ecl(x[1].cast<EclObject>().obj);
}
CAPI void METHOD(Read, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = new EclObject(ecl_safe_read_cstring(wcs2mbs(x[1].cast<String>().str).c_str()));
}

#undef ASSIGN
#define ASSIGN(f) CASSIGN(Ecl_Object,f)
#undef VALUE
#define VALUE(f) CVALUE(Ecl_Object,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(Ecl_Object,f,sig)

CAPI void METHOD(eval, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = new EclObject(ecl_safe_eval(self.cast<EclObject>().obj));
}
CAPI void METHOD(mU, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = from_ecl(self.cast<EclObject>().obj);
}