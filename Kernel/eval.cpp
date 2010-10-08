#include <mU/Common.h>
#include <mU/Kernel.h>

namespace mU {
void Kernel::start() {
    mStack.clear();
    mSelf.clear();
    mLocal.clear();
	depth = 0;
    error.str("");
	beginSelf(null);
	mStack.push_back(null);
    mLocal.push_back(context());
}
void Kernel::abort() {
    throw std::logic_error(error.str().c_str());
}
void Kernel::debug() {
    // TODO: 处理trace,debug,exception,semaphore,kill,yield,resume
    // 作为函数对象,允许外部插入(Hook)处理函数?
	if (depth >= recursion) {
		for (uint i = 0; i < mStack.size(); ++i)
			println(mStack[i]);
		error << "depth:Recursion depth of " << recursion << " exceeded";
		abort();
	}
}
var Kernel::eval(const var& x) {
	var r;
	++depth;
	debug();
	if (x.isTuple()) {
        r = rewrite(x.tuple().clone());
        r = value(r.tuple());
    } else {
        r = value(x);
    }
	--depth;
    return r;
}
var Kernel::lazy(const var& x) {
	if (x.isObject()) {
		if (x.object().type == $.Delayed)
			return eval(cast<Delayed>(x).data);
		if (x.object().type == $.Method)
			return cast<Method>(x)(*this);
	}
	return x;
}
}