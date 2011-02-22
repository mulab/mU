#pragma once
#include "Common.h"
#include <list>
#include <map>
#include <vector>

namespace mU {
class Kernel;
struct Assign : public Object {
	Assign() : Object($.Assign) {}
	virtual bool operator()(Kernel& k, const Tuple& x, const var& y) {
		return false;
	}
};
struct Value : public Object {
	Value() : Object($.Value) {}
	virtual var operator()(Kernel& k, Tuple& x) {
		return &x;
	}
};

struct Method : public Object {
	Method() : Object($.Method) {}
	virtual var operator()(Kernel& k) {
		return null;
	}
};
class Match : public Object {
public:
	Match() : Object($.Match) {}
	virtual bool operator()(Kernel& k, var& r, const var& x) {
		return false;
	}
};
// Rewriting Engine + Rule Database + Pattern Matcher
class Kernel {
public:
    std::vector<sym> mContext;
    std::vector<std::list<sym> > mContextPath;
	std::vector<sym> mLocal;
    std::vector<var> mSelf;
    std::vector<var> mStack;

    API Kernel();

	// symbol & context
    API sym symbol(wcs);
	inline sym symbol(const wstring& x) {
		return symbol(x.c_str());
	}
    sym& context() {
        return mContext.back();
    }
	sym context() const {
		return mContext.back();
	}
	void beginContext(sym x) {
		mContext.push_back(x);
	}
	void endContext() {
		mContext.pop_back();
	}
    std::list<sym>& contextPath() {
        return mContextPath.back();
    }
	const std::list<sym>& contextPath() const {
		return mContextPath.back();
	}
    API bool beginPackage(sym);
    API bool endPackage();
	API void destroy(sym);
	API void clear(sym);
    sym local() const {
        return mLocal.back();
    }
    void beginLocal(sym x) {
        mLocal.push_back(x);
    }
    void beginLocal() {
        beginLocal(root->clone());
    }
    void endLocal() {
        mLocal.pop_back();
    }
    var& self() {
        return mSelf.back();
    }
	var self() const {
		return mSelf.back();
	}
	void beginSelf(const var& x) {
		mSelf.push_back(x);
	}
	void endSelf() {
		mSelf.pop_back();
	}

	// print & log
	API void print(sym, wostream& = wcout) const;
	API void print(const Object&, wostream& = wcout) const;
	API void print(const Tuple&, wostream& = wcout) const;
	API void print(const var&, wostream& = wcout) const;
	wostream* log;
	API Kernel& logging(wcs);
	inline Kernel& logging(const wstring& x) {
		return logging(x.c_str());
	}
	Kernel& operator<<(sym x) {
		print(x, *log);
		return *this;
	}
	Kernel& operator<<(const Object& x) {
		print(x, *log);
		return *this;
	}
	Kernel& operator<<(const Tuple& x) {
		print(x, *log);
		return *this;
	}
	Kernel& operator<<(const var& x) {
		print(x, *log);
		return *this;
	}
	template <class T>
	Kernel& operator<<(const T& x) {
		*log << x;
		return *this;
	}
	Kernel& operator<<(Kernel& (*pf)(Kernel&)) {
		return pf(*this);
	}

    // Rules Database
    typedef std::unordered_set<sym> Attribute;
    std::unordered_map<sym, Attribute> attributes;
    API Tuple* rewrite(Tuple*);
    API Tuple* spread(const Tuple&);
    API Tuple* flatten(sym, const Tuple&) const;

	API bool match(var&, const var&, Match&);
	API var replace(const var&, Match&);
	// std::set<var> mBind;
	std::unordered_set<var> mBind;
	bool bound(const var& x) const {
		return mBind.count(x) != 0;
	}
	bool bind(const var& x, const var& y) {
		mBind.insert(x);
		return assign(x, y);
	}
	bool unbind(const var& x) {
		mBind.erase(x);
		return assign(x, null);
	}

    // Rule
	std::unordered_map<sym, var> owns;
	typedef std::unordered_map<var, var> UMap;
	std::unordered_map<sym, UMap> certains;
	typedef std::map<var, var> Map;
	std::unordered_map<sym, Map> matches;
	
	// Interface
	std::unordered_map<sym, var> assigns;
    std::unordered_map<sym, var> values;

    // assign
    API bool assign(sym, const var&);
    API bool assign(const Key&, const var&);
    API bool assign(const Tuple&, const var&);
    API bool assign(const var&, const var&);
	API bool rule(const Tuple&, const Match*);

    // value
    API var value(sym);
    API var value(const Key&);
    API var value(Tuple&);
    API var value(const var&);
	API bool certain(var&, sym, const Tuple&);
	API bool match(var&, sym, const Tuple&);

    // get
    API var get(sym, const Key&);
    var slot(sym x, const Key& y) {
        beginSelf(x);
        var r = get(x, y);
        endSelf();
        return r;
    }
	API var get(const Object&, const Key&) const;
	var slot(const Object& x, const Key& y) {
		beginSelf(&x);
		var r = get(x, y);
		endSelf();
		return r;
	}
    API var get(const Tuple&, const Key&);
    var slot(const Tuple& x, const Key& y) {
        beginSelf(&x);
        var r = get(x, y);
        endSelf();
        return r;
    }
    API var get(const var&, const Key&);
    var slot(const var& x, const Key& y) {
        beginSelf(x);
        var r = get(x, y);
        endSelf();
        return r;
    }
    API var get(const var&, const Tuple&);
    var slot(const var& x, const Tuple& y) {
        beginSelf(x);
        var r = get(x, y);
        endSelf();
        return r;
	}

    // set
    API bool set(sym, const Key&, const var&);
    API bool set(Tuple&, const Key&, const var&);
    API bool set(const var&, const Key&, const var&);
    API bool set(const var&, const Tuple&, const var&);

    // eval
	var& top() {
		return mStack.back();
	}
	void push(const var& x) {
		mStack.push_back(x);
	}
	void pop() {
		mStack.pop_back();
	}
    API void start();
    API void abort();
    API void debug();
    API var eval(const var&);
	API var lazy(const var&);

	uint recursion;
	uint depth;
};
inline Kernel& endl(Kernel& k) {
	*k.log << std::endl;
	return k;
}
}
