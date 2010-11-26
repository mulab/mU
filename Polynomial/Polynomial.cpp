#include <mU/Polynomial.h>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel.lib")
#endif

namespace mU {
namespace Polynomial {
sym poly;
}
}

using namespace mU;

CAPI void Install(Kernel& k) {
	Polynomial::poly = SYS(Polynomial);
	//wcout << "#Polynomial# Install Successfully!" << endl;
}
CAPI void Uninstall(mU::Kernel& k) {
	wcout << "#Polynomial# Uninstall Successfully!" << endl;
}
