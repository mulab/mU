#pragma once

#include "Curve3D.h"
#include "F1P.h"

using namespace mU;

class ParametricCurve3D : public Curve3D {

public:
    double tmin, tmax, spt;
    double dt;
    ArrayList<double> arrayT; //存每个点的t值，与verts一一对应
    F1P *fx, *fy, *fz;

public:
    ParametricCurve3D(F1P *fx, F1P *fy, F1P *fz, double start, double end);
    ~ParametricCurve3D();
    void creatCurve();

private:
    int getBaseNum();
    void addVerts();
    Vector3D* getVector(double t, double dt);
};




