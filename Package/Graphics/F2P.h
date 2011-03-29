#pragma once

#include "common.h"

#define LARGE 1000000

using namespace mU;

class F2P {

    var expre, xparam, yparam;

public:

    F2P(Var ep, Var xpa, Var ypa);

    void getData(double* x, double* y, double *z, int num);

    void getArrayData(double *x, double *y, double *z, int num);

    double getSingleData(double inputx, double inputy);

    double getDerivativeX(double x, double y, double dx);

    double getDerivativeY(double x, double y, double dy);
};