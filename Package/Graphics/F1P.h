#pragma once

#include "common.h"

#define LARGE 1000000

using namespace mU;

class F1P {

    var expre, param;

public:

    F1P(Var ep, Var pa);

    void getData(double* x, double* y, int num);

    void getArrayData(double *x, double *y, int num);

    double getSingleData(double input);

    double getDerivative(double x, double dx);

    double getSecondDerivative(double x, double dx);
};