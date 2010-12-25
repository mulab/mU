#include "../JavaObject.h"
#include <mU/utils.h>
#include <algorithm>

using namespace mU;

#undef ASSIGN
#define ASSIGN(f) CASSIGN(Java,f)
#undef VALUE
#define VALUE(f) CVALUE(Java,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(Java,f,sig)

CAPI bool ASSIGN(Object)(Kernel& k, const Tuple& x, const var& y) {
	if (x[0].isObject()) {
		if (x.size == 2) {
			Key* h = toKey(x[1]);
			if (h->kind() == Key::String)
				return set(JavaObject::Env, x[0].cast<JavaObject>().obj, h->toS(), y);
		}
	}
	return false;
}
CAPI void VALUE(Cast)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2) {
		if (x[1].isObject(JavaObject::$JavaObject)) {
			r = cast(JavaObject::Env, x[1].cast<JavaObject>().obj);
			return;
		}
	}
	if (x.size == 3) {
		jclass cls = toClass(JavaObject::Env, x[2]);
		if (cls) {
			r = new JavaObject(cast(JavaObject::Env, x[1], cls));
			return;
		}
	}
}
CAPI void VALUE(Class)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1) {
		r = new JavaObject(JavaObject::Class);
		return;
	}
	if (x.size == 2) {
		jclass cls = toClass(JavaObject::Env, x[1]);
		if (cls)
			r = new JavaObject(cls);
	}
}
CAPI void VALUE(Java)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[0].isSymbol()) {
		r = JavaObject::to(x[1]);
		return;
	}
}
CAPI void VALUE(Init)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1) {
		char* argv = "";
		JavaObject::Init(0, &argv);
		r = null;
		return;
	}
	std::vector<string> buf;
	std::vector<char*> argv;
	for (uint i = 1; i < x.size; ++i) {
		if (x[i].isObject($.String)) {
			buf.push_back(wcs2mbs(x[i].cast<String>().str));
			argv.push_back(const_cast<char*>(buf.back().c_str()));
		}
	}
	JavaObject::Init(argv.size(), argv.data());
	r = null;
	return;
}
CAPI void VALUE(New)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 3 && x[2].isTuple()) {
		jclass cls = toClass(JavaObject::Env, x[1]);
		if (cls)
			r = newInstance(JavaObject::Env, cls, x[2].tuple());
	}
}
CAPI void VALUE(Object)(Kernel& k, var& r, Tuple& x) {
	if (x[0].isObject()) {
		if (x.size == 2) {
			if (x[1].isTuple()) {
				Key* h = toKey(x[1].tuple()[0]);
				if (h->kind() == Key::String) {
					r = invoke(JavaObject::Env, x[0].cast<JavaObject>().obj, h->toS(), x[1].tuple());
					return;
				}
			}
			Key* h = toKey(x[1]);
			if (h->kind() == Key::String) {
				r = get(JavaObject::Env, x[0].cast<JavaObject>().obj, h->toS());
				return;
			}
		}
	}
}
CAPI void METHOD(mU, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = x[1].cast<JavaObject>().from();
}

#undef ASSIGN
#define ASSIGN(f) CASSIGN(Java_Object,f)
#undef VALUE
#define VALUE(f) CVALUE(Java_Object,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(Java_Object,f,sig)

CAPI void VALUE(cast)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 1) {
		if (k.self().isObject(JavaObject::$JavaObject)) {
			r = cast(JavaObject::Env, k.self().cast<JavaObject>().obj);
			return;
		}
	}
	if (x.size == 2) {
		jclass cls = toClass(JavaObject::Env, x[1]);
		if (cls) {
			r = new JavaObject(cast(JavaObject::Env, k.self(), cls));
			return;
		}
	}
}
CAPI void VALUE(new)(Kernel& k, var& r, Tuple& x) {
	jclass cls = toClass(JavaObject::Env, k.self());
	if (cls)
		r = newInstance(JavaObject::Env, cls, x);
}
// getter/setter? prop cls?
CAPI void METHOD(class, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = new JavaObject(self.cast<JavaObject>().cls);
}
CAPI void METHOD(exec, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	if (x[1].isTuple()) {
		Key* h = toKey(x[1].tuple()[0]);
		if (h->kind() == Key::String) {
			r = invoke(JavaObject::Env, self.cast<JavaObject>().obj, h->toS(), x[1].tuple());
			return;
		}
	}
}
CAPI void METHOD(get, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	Key* h = toKey(x[1]);
	if (h->kind() == Key::String) {
		r = get(JavaObject::Env, self.cast<JavaObject>().obj, h->toS());
		return;
	}
}
CAPI void METHOD(set, 2)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	Key* h = toKey(x[1]);
	if (h->kind() == Key::String) {
		r = boolean(set(JavaObject::Env, self.cast<JavaObject>().obj, h->toS(), x[2]));
		return;
	}
}
CAPI void METHOD(mU, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
	r = self.cast<JavaObject>().from();
}