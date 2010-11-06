#include <mU/System.h>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel.lib")
#endif

using namespace mU;

CAPI void Install(Kernel& k) {
#define M(x,y) k.owns[SYS(x)] = y;
    M(Null, null)
#undef M
#define M(x,y) k.attributes[SYS(x)].insert($.y);
    /*M(CInterface, HoldRest)*/M(Set, HoldFirst)
#undef M
    cvalue(k, cnoload("system"), SYS(CInterface));
    var r;
    k.beginContext(sys);
    Input(k, r, _W("../System/System.u"));
    k.endContext();
}
CAPI void Uninstall(Kernel& k) {
	wcout << "#System# Uninstall Successfully!" << endl;
}
