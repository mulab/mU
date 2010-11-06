#pragma once

#include "IndexedQuadArray.h"
#include "F2P.h"

using namespace mU;

class ParametricRegion : IndexedQuadArray {

public:
    double umin, umax, spu, vmin, vmax, spv;
    int nu, nv;
    F2P *fx, *fy;

public:
    ParametricRegion(F2P *fx, F2P *fy, double umin, double umax, double vmin, double vmax);
    ~ParametricRegion();

private:
    int getBaseUNum();
    int getBaseVNum();
};
