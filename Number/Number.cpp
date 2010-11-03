#include <mU/Number.h>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel.lib")
#endif

using namespace mU;

CAPI void Install(Kernel& k) {
	//wcout << "#Number# Install Successfully!" << endl;
}
CAPI void Uninstall(mU::Kernel& k) {
	wcout << "#Number# Uninstall Successfully!" << endl;
}
