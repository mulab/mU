#include "F1P.h"

F1P::F1P(Var ep, Var pa) {
    param = pa;
    expre = ep;
}

void F1P::getData(double* x, double* y, int num) {
    map_t m;
    for (int i = 0; i < num; ++i) {
        m[param] = Flt(x[i]);
        var r = Subs(m, expre);
        r = Evalf(r);
        y[i] = N::D(r);
    }
}

void F1P::getArrayData(double *x, double *y, int num) {
    getData(x, y, num);
}

double F1P::getSingleData(double input) {
    double x = input;
    double y;
    getData(&x, &y, 1);
    return y;
}

double F1P::getDerivative(double x, double dx) {
    double datay[2];
    double datax[2];
    datax[0] = x;
    datax[1] = x + dx;
    getData(datax, datay, 2);
    return (datay[1] - datay[0]) / dx;
}

double F1P::getSecondDerivative(double x, double dx) {
    double datay[3];
    double datax[3];
    datax[0] = x;
    datax[1] = x + dx;
    datax[2] = x + 2 * dx;
    getData(datax, datay, 3);
    return (datay[2] - 2*datay[1] + datay[0]) / (dx*dx);
}
