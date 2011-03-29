#include "F2P.h"

F2P::F2P(Var ep, Var xpa, Var ypa) {
    xparam = xpa;
    yparam = ypa;
    expre = ep;
}

void F2P::getData(double* x, double* y, double *z, int num) {
    map_t m;
    for (int i = 0; i < num; ++i) {
        m[xparam] = Flt(x[i]);
        m[yparam] = Flt(y[i]);
        var r = Subs(m, expre);
        r = Evalf(r);
        z[i] = N::D(r);
    }
}

void F2P::getArrayData(double *x, double *y, double *z, int num) {
    getData(x, y, z, num);
}

double F2P::getSingleData(double inputx, double inputy) {
    double x = inputx;
    double y = inputy;
    double z;
    getData(&x, &y, &z, 1);
    return z;
}

double F2P::getDerivativeX(double x, double y, double dx) {
    double datax[2];
    double datay[2];
    double dataz[2];
    datax[0] = x;
    datax[1] = x + dx;
    datay[0] = y;
    datay[1] = y;
    getData(datax, datay, dataz, 2);
    return (dataz[1] - dataz[0]) / dx;
}

double F2P::getDerivativeY(double x, double y, double dy) {
    double datax[2];
    double datay[2];
    double dataz[2];
    datax[0] = x;
    datax[1] = x;
    datay[0] = y;
    datay[1] = y + dy;
    getData(datax, datay, dataz, 2);
    return (dataz[1] - dataz[0]) / dy;
}