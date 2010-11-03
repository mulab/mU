#include <mU/Parser.h>
#include <mU/System.h>
#ifdef _MSC_VER
#pragma comment(lib,"Parser.lib")
#endif

namespace mU {
bool In(Kernel& k, var& r, wcs s) {
    wifstream f(cstr(s).c_str());
    if (f) {
        Parser p(k);
		p.start(f);
        r = p.eval();
        f.close();
        return true;
    }
    return false;
}
bool Out(Kernel& k, const var& x, wcs s) {
	wofstream f(cstr(s).c_str());
	if (f) {
		println(k, x, f);
		f.close();
		return true;
	}
	return false;
}
bool OutAppend(Kernel& k, const var& x, wcs s) {
	wofstream f(cstr(s).c_str(),ios::app);
	if (f) {
		println(k, x, f);
		f.close();
		return true;
	}
	return false;
}
}

using namespace mU;

CAPI void VALUE(In)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject($.String))
		if (!In(k, r, cast<String>(x[1]).toS()))
			r = $.Fail;
}
CAPI void METHOD(Out, 2)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = boolean(Out(k, x[1], cast<String>(x[2]).toS()));
}
CAPI void METHOD(OutAppend, 2)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = boolean(OutAppend(k, x[1], cast<String>(x[2]).toS()));
}
CAPI void VALUE(Print)(Kernel& k, var& r, Tuple& x) {
    for (uint i = 1; i < x.size; ++i)
        grammar.print(k, wcout, x[i]);
    wcout << endl;
    r = null;
}
CAPI void VALUE(Full)(Kernel& k, var& r, Tuple& x) {
    wostringstream out;
	for (uint i = 1; i < x.size; ++i)
		k.print(out, x[i]);
    r = new String(out.str());
}
