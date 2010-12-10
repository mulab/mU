#include <mU/Parser.h>
#include <mU/System.h>
#ifdef _MSC_VER
#pragma comment(lib,"Parser.lib")
#endif

namespace mU {
bool Input(Kernel& k, var& r, wcs s) {
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
}

using namespace mU;

CAPI void VALUE(Input)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1) {
		wstring buf;
		std::getline(wcin, buf);
		wistringstream iss(buf);
		Parser p(k);
		p.start(iss);
		r = p.eval();
		return;
	}
	if (x.size == 2 && x[1].isObject($.String)) {
		if (!Input(k, r, x[1].cast<String>().toS()))
			r = $.Fail;
	}
}
