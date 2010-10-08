#pragma once
#include <vector>

typedef unsigned int uint;
class sieve {
    std::vector<uint> v;
    void init(uint);
public:
    sieve(uint n) {
        init(n);
    }
    const uint* begin() const {
        return v.begin().operator ->();
    }
    const uint* end() const {
        return v.end().operator ->();
    }
    uint lower(uint c) const {
        return std::max<int>(std::upper_bound(v.begin(), v.end(), c) - v.begin() - 1, 0);
    }
    uint upper(uint c) const {
        return std::min<int>(std::lower_bound(v.begin(), v.end(), c) - v.begin(), v.size() - 1);
    }
    uint floor(uint c) const {
        return v[lower(c)];
    }
    uint ceiling(uint c) const {
        return v[upper(c)];
    }
    uint find(uint c) const {
        uint r = lower(c);
        return v[r] == c ? r : 0;
    }
    const uint& operator [](uint i) const {
        return v[i];
    }
    uint count(uint low, uint high) const {
        if (low > high) std::swap(low, high);
        uint r = upper(low), s = lower(high);
        return s + 1 - r + (v[r] == low) - (v[s] == high);
    }
    uint size() const {
        return v.size();
    }
    void reset(uint n) {
        init(n);
    }
};
