#include <mU/Grammar.h>
#include <mU/System.h>

using namespace mU;

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
