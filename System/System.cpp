#include <mU/System.h>
#include <mU/utils.h>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel.lib")
#endif

using namespace mU;

CAPI void Install(Kernel& k) {
#define M(x,y) k.attributes[SYS(x)].insert($.y);
	M(CInterface, HoldRest)
#undef M
    cvalue(k, cnoload("system"), SYS(CInterface));
    var r;
    k.beginContext(sys);
    Input(k, r, path() + _W("../System/System.u"));
    k.endContext();
}
CAPI void Uninstall(Kernel& k) {
	wcout << "#System# Uninstall Successfully!" << endl;
}
