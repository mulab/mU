#pragma once

#include "Curve.h"
#include "F1P.h"

using namespace mU;

class ParametricCurve : public Curve {

public:
    double tmin, tmax, spt;
    double dt, graphScale;
    ArrayList<double> arrayT; //存每个点的t值，与verts一一对应
    F1P *fx, *fy;

    //about color functions
    F1P *cf;

public:
    ParametricCurve(F1P *fx, F1P *fy, double start, double end);
    ParametricCurve(F1P *fx, F1P *fy, double start, double end, F1P *cf);
    ~ParametricCurve();

protected:
    int getBaseNum();
    void makePoints();
    void useColorFunction();
    void addVerts(LineStrip* lineStrip);
    inline double getK(double t, double dt);
};
