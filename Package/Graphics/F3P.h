#pragma once

#include "common.h"

#define LARGE 1000000

using namespace mU;

class F3P {

    var expre, xparam, yparam, zparam;

public:

    F3P(Var ep, Var xpa, Var ypa, Var zpa);

    void getData(double* x, double* y, double *z, double *ans, int num);

    void getArrayData(double *x, double *y, double *z, double *ans, int num);

    double getSingleData(double inputx, double inputy, double inputz);

    double getDerivativeX(double x, double y, double z, double dx);

    double getDerivativeY(double x, double y, double z, double dy);

    double getDerivativeZ(double x, double y, double z, double dz);
};