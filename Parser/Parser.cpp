#include <mU/Parser.h>

namespace mU {
Parser::Parser(Kernel& k) : kernel(k), mIn(&wcin) {
	Grammar::Init();
}
}
