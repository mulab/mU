#pragma once

#include "IndexedQuadArray3D.h"
#include "F2P.h"

using namespace mU;

class ParametricSurface : IndexedQuadArray3D {

public:
    double umin, umax, spu, vmin, vmax, spv;
    int nu, nv, numesh, nvmesh, wumesh, wvmesh;
    F2P *fx, *fy, *fz;

public:
    ParametricSurface(F2P *fx, F2P *fy, F2P *fz, double umin, double umax, double vmin, double vmax);
    ~ParametricSurface();

private:
    int getUMeshNum();
    int getVMeshNum();
    int getUMeshWidth();
    int getVMeshWidth();
};
