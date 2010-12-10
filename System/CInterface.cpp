#include <mU/System.h>
#ifdef _MSC_VER
#pragma comment(lib,"Interface.lib")
#endif

using namespace mU;

CAPI void METHOD(CInstall, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    r = boolean(cinstall(k, cstr(x[1]).c_str()));
}
CAPI void VALUE(CInterface)(Kernel& k, var& r, Tuple& x) {
    if (x[1].head() != $.String)
        return;
    void* m = cnoload(cstr(x[1]).c_str());
    if (!m)
        return;
    for (uint i = 2; i < x.size; ++i) {
        if (!x[i].isTuple() || !cinterface(k, m, x[i].tuple())) {
        	// k.error << "cinterface:undefined interface [" << i - 1 << "]";
            // k.abort();
            return;
        }
    }
    r = null;
}
CAPI void METHOD(CUninstall, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    r = boolean(cuninstall(k, cstr(x[1]).c_str()));
}
