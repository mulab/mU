#include <mU/Common.h>
#include <mU/Kernel.h>

namespace mU {
Kernel::Kernel() : recursion(128), precision(20) {
	Enum::Init();
    mContextPath.push_back(std::list<sym>());
    contextPath().push_back(sys);
    contextPath().push_back(root);
    beginContext(SYM(root, Global));
    start();
}
}

using namespace mU;

CAPI void Install(Kernel& k) {
    wcout << "#Kernel# Install Successfully!" << endl;
}
CAPI void Uninstall(Kernel& k) {
    wcout << "#Kernel# Uninstall Successfully!" << endl;
}