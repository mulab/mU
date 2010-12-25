#include "../mU0Object.h"

using namespace mU;

CAPI void Install(Kernel& k) {
	mU0Object::Init();
	// wcout << "#mU0Embed# Install Successfully!" << endl;
}
CAPI void Uninstall(mU::Kernel& k) {
	mU0Object::Close();
	wcout << "#mU0Embed# Uninstall Successfully!" << endl;
}

#undef ASSIGN
#define ASSIGN(f) CASSIGN(mU0,f)
#undef VALUE
#define VALUE(f) CVALUE(mU0,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(mU0,f,sig)

CAPI void VALUE(mU0)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[0].isSymbol()) {
		r = mU0Object::New(x[1]);
		return;
	}
}
CAPI void VALUE(Eval)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject()) {
		if (x[1].object().type == $.String) {
			r = new mU0Object(mU0::ParseString(x[1].cast<String>().str));
			return;
		}
		if (x[1].object().type == mU0Object::$mU0Object) {
			r = new mU0Object(mU0::Eval(x[1].cast<mU0Object>().obj));
			return;
		}
	}
}
CAPI void VALUE(Main)(Kernel& k, var& r, Tuple& x) {
	int argc = x.size - 1;
	if (x.size == 1) {
		char* argv[] = {""};
		mU0Object::Main(argc, argv);
	}
}
CAPI void VALUE(Object)(Kernel& k, var& r, Tuple& x) {
	if (x[0].isObject()) {
		r = new mU0Object(mU0::Eval(to_mu0(x)));
	}
}
CAPI void METHOD(mU, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = x[1].cast<mU0Object>().normal();
}
CAPI void METHOD(Read, 2)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	if (x[2].isSymbol()) {
		wistringstream iss(x[1].cast<String>().str);
		r = new mU0Object(mU0::Read(iss, to_mu0(x[2].symbol())));
	}
}

#undef ASSIGN
#define ASSIGN(f) CASSIGN(mU0_Object,f)
#undef VALUE
#define VALUE(f) CVALUE(mU0_Object,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(mU0_Object,f,sig)

CAPI void METHOD(eval, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = new mU0Object(mU0::Eval(self.cast<mU0Object>().obj));
}
CAPI void METHOD(mU, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = self.cast<mU0Object>().normal();
}