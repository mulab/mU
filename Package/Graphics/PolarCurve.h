#pragma once

#include "Curve.h"
#include "F1P.h"

using namespace mU;

class PolarCurve : public Curve {

public:
    double tmin, tmax, spt;
    double dt, graphScale;
    ArrayList<double> arrayT; //存每个点的t值，与verts一一对应
    F1P *f;

    //about color functions
    F1P *cf;

public:
    PolarCurve(F1P *f, double start, double end);
    PolarCurve(F1P *ff, double start, double end, F1P *cff);
    ~PolarCurve();

protected:
    int getBaseNum();
    void makePoints();
    void useColorFunction();
    void addVerts(LineStrip* lineStrip);
    inline double getK(double t, double dt);
};
