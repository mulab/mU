#pragma once

#include "Curve.h"
#include "F1P.h"

using namespace mU;

class CommonCurve : public Curve {

public:
    double dx, graphScale;
    F1P *f;

    //about color functions
    F1P *cf;

public:
    CommonCurve();
    CommonCurve(F1P *f, double start, double end);
    CommonCurve(F1P *f, double start, double end, F1P *cf); //with color function
    ~CommonCurve();

protected:
    int getBaseNum();
    void makePoints();
    void addVerts(LineStrip* lineStrip);
    void useColorFunction();
};
