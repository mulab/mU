#include <mU/Polynomial.h>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel.lib")
#endif

using namespace mU;

CAPI void Install(Kernel& k) {
	//wcout << "#Polynomial# Install Successfully!" << endl;
}
CAPI void Uninstall(mU::Kernel& k) {
	wcout << "#Polynomial# Uninstall Successfully!" << endl;
}
