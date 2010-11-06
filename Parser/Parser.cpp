#include <mU/Kernel.h>
#include <mU/Parser.h>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel.lib")
#endif

namespace mU {
Parser::Parser(Kernel& k) : kernel(k), mIn(&wcin) {
	Grammar::Init();
}
}

using namespace mU;

CAPI void Install(Kernel& k) {
	//wcout << "#Parser# Install Successfully!" << endl;
}
CAPI void Uninstall(mU::Kernel& k) {
	wcout << "#Parser# Uninstall Successfully!" << endl;
}
