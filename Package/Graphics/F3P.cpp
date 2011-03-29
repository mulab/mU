#include "F3P.h"

F3P::F3P(Var ep, Var xpa, Var ypa, Var zpa) {
    xparam = xpa;
    yparam = ypa;
    zparam = zpa;
    expre = ep;
}

void F3P::getData(double* x, double* y, double *z, double *ans, int num) {
    map_t m;
    for (int i = 0; i < num; ++i) {
        m[xparam] = Flt(x[i]);
        m[yparam] = Flt(y[i]);
        m[zparam] = Flt(z[i]);
        var r = Subs(m, expre);
        r = Evalf(r);
        ans[i] = N::D(r);
    }
}

void F3P::getArrayData(double *x, double *y, double *z, double *ans, int num) {
    getData(x, y, z, ans, num);
}

double F3P::getSingleData(double inputx, double inputy, double inputz) {
    double x = inputx;
    double y = inputy;
    double z = inputz;
    double ans;
    getData(&x, &y, &z, &ans, 1);
    return ans;
}

double F3P::getDerivativeX(double x, double y, double z, double dx) {
    double datax[2];
    double datay[2];
    double dataz[2];
    double dataw[2];
    datax[0] = x;
    datax[1] = x + dx;
    datay[0] = y;
    datay[1] = y;
    dataz[0] = z;
    dataz[1] = z;
    getData(datax, datay, dataz, dataw, 2);
    return (dataw[1] - dataw[0]) / dx;
}

double F3P::getDerivativeY(double x, double y, double z, double dy) {
    double datax[2];
    double datay[2];
    double dataz[2];
    double dataw[2];
    datax[0] = x;
    datax[1] = x;
    datay[0] = y;
    datay[1] = y + dy;
    dataz[0] = z;
    dataz[1] = z;
    getData(datax, datay, dataz, dataw, 2);
    return (dataw[1] - dataw[0]) / dy;
}

double F3P::getDerivativeZ(double x, double y, double z, double dz) {
    double datax[2];
    double datay[2];
    double dataz[2];
    double dataw[2];
    datax[0] = x;
    datax[1] = x;
    datay[0] = y;
    datay[1] = y;
    dataz[0] = z;
    dataz[1] = z + dz;
    getData(datax, datay, dataz, dataw, 2);
    return (dataw[1] - dataw[0]) / dz;
}