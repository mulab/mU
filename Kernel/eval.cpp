#include <mU/Kernel.h>
#include <mU/utils.h>
#include <stdexcept>

namespace mU {
void Kernel::start() {
    logging(__FUNCTIONW__) << _W("Kernel start.") << endl;
	mStack.clear();
    mSelf.clear();
    mLocal.clear();
	depth = 0;
	beginSelf(null);
	mStack.push_back(null);
    mLocal.push_back(context());
}
void Kernel::abort() {
	logging(__FUNCTIONW__) << _W("Kernel abort.") << endl;
	if (mStack.size() > 1) {
		wcerr << _W("Stack dump: ") << endl;
		for (uint i = 1; i < mStack.size(); ++i)
			(*this) << mStack[i] << endl;
	}
    throw std::logic_error("abort");
}
void Kernel::debug() {
    // TODO: 处理trace,debug,exception,semaphore,kill,yield,resume
    // 作为函数对象,允许外部插入(Hook)处理函数?
	if (depth >= recursion) {
		logging(__FUNCTIONW__) << _W("Recursion depth of ") << recursion << _W(" exceeded!") << endl;
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
			return eval(x.cast<Tag>().data);
		if (x.object().type == $.Method)
			return x.cast<Method>()(*this);
	}
	return x;
}
}
