#include <mU/Numeric.h>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel.lib")
#endif

using namespace mU;

CAPI void Install(Kernel& k) {
	//wcout << "#Numeric# Install Successfully!" << endl;
}
CAPI void Uninstall(Kernel& k) {
	wcout << "#Numeric# Uninstall Successfully!" << endl;
}