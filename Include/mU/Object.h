#pragma once
#include "var.h"

namespace mU {
class Object : public Var {
public:
    API static void ruin(Var*);
    sym type;
    explicit Object(sym $type = 0) : Var(Primary::Object), type($type) {}
    virtual ~Object() {}
    virtual Object* clone() const {
        return new Object(*this);
    }
	virtual int compare(const Object& $other) const {
		return reinterpret_cast<long>(this) - reinterpret_cast<long>(&$other);
	}
	virtual bool equal(const Object& $other) const {
		return compare($other) == 0;
	}
	virtual size_t hash() const {
		return reinterpret_cast<size_t>(this);
	}
	virtual void print(wostream& o = wcout) const {
		type->print(o);
		o << _W('`') << static_cast<const void*>(this);
	}
};
inline wostream& operator<<(wostream& o, const Object& x) {
	x.print(o);
	return o;
}
inline bool var::isObject(sym x) const {
	return isObject() && object().type == x;
}
inline Object& var::object() const {
	return cast<Object>();
}
}