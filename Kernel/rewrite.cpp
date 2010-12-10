#include <mU/Kernel.h>

namespace mU {
Tuple* Kernel::rewrite(Tuple* x) {
    Tuple *r = 0;
    x->tuple[0] = eval(x->tuple[0]);
    if (x->tuple[0].isSymbol()) {
        std::tr1::unordered_map<sym, Attribute>::const_iterator
			iter = attributes.find(x->tuple[0].symbol());
        if (iter != attributes.end()) {
			r = x;
            if (!iter->second.count($.HoldAll)) {
                if (r->size > 1 && !iter->second.count($.HoldFirst))
                    r->tuple[1] = eval(r->tuple[1]);
                if (r->size > 2 && !iter->second.count($.HoldRest)) {
                    for (uint i = 2; i < r->size; ++i)
                        r->tuple[i] = eval(r->tuple[i]);
                }
            }
//             if (!iter->second.count($.SequenceHold))
//                 if ((x = flatten($.Sequence, *r)) != r) {
//                     r->discard();
//                     r = x;
//                 }
            if (iter->second.count($.Flat))
                if ((x = flatten(r->tuple[0].symbol(), *r)) != r) {
                    r->discard();
                    r = x;
                }
            if (iter->second.count($.Orderless))
                std::sort(r->tuple + 1, r->tuple + r->size);
            if (iter->second.count($.Listable))
                if ((x = thread(*r)) != r) {
                    r->discard();
                    r = x;
                }
        }
    }
    if (!r) {
		r = x;
        for (uint i = 1; i < r->size; ++i)
            r->tuple[i] = eval(r->tuple[i]);
//         if ((x = flatten($.Sequence, *r)) != r) {
//             r->discard();
//             r = x;
//         }
    }
    return r;
}
Tuple* Kernel::thread(const Tuple& x) {
    uint m = 0;
    for (uint i = 1; i < x.size; ++i)
        if (x[i].isTuple() && x[i].tuple()[0] == $.List)
            m = std::max(m, x[i].tuple().size - 1);
    if (m == 0)
        return const_cast<Tuple*>(&x);
    Tuple* r = tuple(m + 1);
    r->tuple[0] = $.List;
    for (uint i = 1; i < m + 1; ++i)	{
        Tuple* t = tuple(x.size);
        r->tuple[i] = t;
        t->tuple[0] = x[0];
        for (uint j = 1; j < t->size; ++j) {
            if (x[j].isTuple() && x[j].tuple()[0] == $.List)
                t->tuple[j] = x[j].tuple()[std::min(i, x[j].tuple().size - 1)];
            else
                t->tuple[j] = x[j];
        }
        r->tuple[i] = rewrite(t);
    }
    return r;
}
Tuple* Kernel::flatten(sym h, const Tuple& x) const {
    std::vector<var> v;
    v.reserve(x.size - 1);
    for (uint i = 1; i < x.size; ++i) {
        if (x[i].isTuple() && x[i].tuple()[0] == h)
            v.insert(v.end(), x[i].tuple().tuple + 1, x[i].tuple().tuple + x[i].tuple().size);
        else
            v.push_back(x[i]);
    }
    if (v.size() == x.size - 1)
        return const_cast<Tuple*>(&x);
	return mU::list(v.size(), v.begin(), x[0]);
}
}
