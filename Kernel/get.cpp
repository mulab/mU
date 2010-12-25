#include <mU/Kernel.h>

namespace mU {
var Kernel::get(sym x, const Key& y) {
    if (y.kind() == Key::String) {
        if (!x || x->name())
            return value(x->symbol(y.toS()));
        else {
            var r = x->get(y.toS());
            if (r)
                return lazy(r);
            if (x->context->name()) {
                r = x->context->get(y.toS());
                if (r)
                    return value(r);
            }
			// else x->context, x->context->context, ...?
        }
    }
    return $.Fail;
}
var Kernel::get(const Object& x, const Key& y) const {
	if (y.kind() == Key::String) {
		return x.type->get(y.toS());
	}
	return $.Fail;
}
var Kernel::get(const Tuple& x, const Key& y) {
    if (y.kind() == Key::Integer) {
        uint i = y.toUI();
        if (i < x.size)
            return lazy(x[i]);
    } else {
        for (uint i = 0; i < x.size; ++i) {
            var r = get(x[i], y);
            if (r != $.Fail)
                return r;
        }
    }
    return $.Fail;
}
var Kernel::get(const var& x, const Key& y) {
    switch (x.primary()) {
    case Primary::Symbol:
        return get(x.symbol(), y);
    case Primary::Object:
        return get(x.object(), y);
    case Primary::Tuple:
        return get(x.tuple(), y);
    }
    return $.Fail;
}
var Kernel::get(const var& x, const Tuple& y) {
    if (y[0] != $.List) {
        if (y[0].isKey()) {
            Tuple* t = y.clone();
            t->tuple[0] = get(x, y[0].key());
            var r = rewrite(t);
            return value(r.tuple());
        }
        return $.Fail;
    }
    var r = x;
    for (uint i = 1; i < y.size && r != $.Fail; ++i) {
        var c = eval(y[i]);
        Key* k;
        if (c.isObject()) {
			if (c.object().type == $.Integer)
				k = key(c.cast<Integer>().toUI());
			else if (c.object().type == $.String)
				k = key(wstr(c.cast<String>().toS()));
			else
				return $.Fail;
		} else
            return $.Fail;
        if (i == 1)
            r = get(x, *k);
        else
            r = slot(r, *k);
    }
    return r;
}
}