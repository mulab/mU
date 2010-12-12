#include <mU/Kernel.h>

namespace mU {
sym Kernel::symbol(wcs x) {
    Context& ctx = contexts[context()];
    std::pair<Context::iterator, bool>
    r = ctx.insert(std::make_pair(reinterpret_cast<uint>(x), null));
    if (!r.second)
        return r.first->second.isSymbol() ? r.first->second.symbol() : 0;
    for (std::list<sym>::const_iterator
            iter = contextPath().begin();
            iter != contextPath().end(); ++iter) {
        if (*iter == context())
            continue;
        Context& m = contexts[*iter];
        Context::const_iterator iterm =
            m.find(reinterpret_cast<uint>(x));
        if (iterm != m.end()) {
            ctx.erase(r.first);
            return iterm->second.isSymbol() ? iterm->second.symbol() : 0;
        }
    }
    sym t = context()->clone(x);
    r.first->second = t;
    return t;
}
bool Kernel::beginPackage(sym x) {
    if (!x->name())
        return false;
    mContext.push_back(x);
    mContextPath.push_back(std::list<sym>());
    contextPath().push_back(sys);
    contextPath().push_back(root);
    contextPath().push_back(x);
    return true;
}
bool Kernel::endPackage() {
    if (mContextPath.size() == 1 || mContext.size() == 1)
        return false;
    mContextPath.pop_back();
    std::list<sym>::iterator iter =
        std::find(
            contextPath().begin(),
            contextPath().end(), context());
    if (iter != contextPath().end())
        contextPath().erase(iter);
    contextPath().push_front(context());
    mContext.pop_back();
    return true;
}
void Kernel::destroy(sym x) {
    // TODO: Recycle实例symbol垃圾回收
    //x=$F;x=3
    //F.ref=1,F`#1.ref=2,why?
    if (!x || x->ref() > 1)
        return;
    clear(x);
    const_cast<Symbol*>(x)->discard();
}
void Kernel::clear(sym x) {
    // TODO: ClearAll清除context下所有symbol
    owns.erase(x);
    values.erase(x);
    assigns.erase(x);
    attributes.erase(x);
    certains.erase(x);
    matches.erase(x);
}
}
