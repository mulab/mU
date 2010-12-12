#include <mU/utils.h>
#include <mU/Grammar.h>
#include <mU/System.h>

namespace mU {
bool Output(Kernel& k, const var& x, wcs s) {
	wofstream f(wcs2mbs(s).c_str());
	if (f) {
		println(k, x, f);
		f.close();
		return true;
	}
	return false;
}
bool OutputAppend(Kernel& k, const var& x, wcs s) {
	wofstream f(wcs2mbs(s).c_str(), ios::app);
	if (f) {
		println(k, x, f);
		f.close();
		return true;
	}
	return false;
}
}

using namespace mU;

CAPI void METHOD(Output, 2)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = boolean(Output(k, x[1], x[2].cast<String>().toS()));
}
CAPI void METHOD(OutputAppend, 2)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = boolean(OutputAppend(k, x[1], x[2].cast<String>().toS()));
}
