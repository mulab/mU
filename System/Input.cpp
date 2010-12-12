#include <mU/System.h>
#include <mU/Parser.h>
#ifdef _MSC_VER
#pragma comment(lib,"Parser.lib")
#endif
#include <mU/utils.h>

namespace mU {
bool Input(Kernel& k, var& r, wcs s) {
    wifstream f(wcs2mbs(s).c_str());
    if (!f) {
		k.logging(__FUNCTIONW__) << _W("File ") << s << _W(" not found.") << endl;
		return false;
	}
	Parser p(k);
	try {
		p.start(f);
		r = p.eval();
	} catch (std::exception& e) {
		k.logging(__FUNCTIONW__) << _W("Error occurred while eval ") << s << _W(", ") 
			<< e.what() << _W("...") << endl;
	}
	f.close();
    return true;
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
