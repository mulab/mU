#include "common.h"
#include "ParametricRegion.h"

using namespace std;

//fix me
ParametricRegion::ParametricRegion(F2P *ffx, F2P *ffy, double umin, double umax, double vmin, double vmax) {
    fx = ffx;
    fy = ffy;
    this->umin = umin;
    this->umax = umax;
    this->vmin = vmin;
    this->vmax = vmax;
    spu = umax - umin;
    spv = vmax - vmin;
    nu = getBaseUNum();
    nv = getBaseVNum();
    double *u = new double[nu*nv];
    double *v = new double[nu*nv];
    double *x = new double[nu*nv];
    double *y = new double[nu*nv];
    for (int i = 0; i < nv; i++) {
        for (int j = 0; j < nu; j++) {
            u[i*nu+j] = umin + (umax - umin) * (double)j / (double)(nu - 1);
        }
    }
    for (int i = 0; i < nv; i++) {
        for (int j = 0; j < nu; j++) {
            v[i*nu+j] = vmin + (vmax - vmin) * (double)i / (double)(nv - 1);
        }
    }
    fx->getArrayData(u, v, x, nu*nv);
    fy->getArrayData(u, v, y, nu*nv);
    xmin = x[0];
    xmax = x[0];
    ymin = y[0];
    ymax = y[0];
    for (int i = 0; i < nu*nv; i++) {
        if (x[i] > xmax) xmax = x[i];
        if (x[i] < xmin) xmin = x[i];
        if (y[i] > ymax) ymax = y[i];
        if (y[i] < ymin) ymin = y[i];
    }
    for (int i = 0; i < nu*nv; i++) {
        verts.add(new Point2d(x[i], y[i]));
    }
    for (int i = 0; i < nv - 1; i++)
        for (int j = 0; j < nu - 1; j++) {
            quads.add(new Quad(i*nu + j, i*nu + j + 1, (i + 1)*nu + j, (i + 1)*nu + j + 1));
        }
    for (int j = 0; j < nu; j++)
        edge.add(0*nu + j);
    for (int i = 0; i < nv; i++)
        edge.add(i*nu + nv - 1);
    for (int j = nu - 1; j >= 0; j--)
        edge.add((nu - 1)*nv + j);
    for (int i = nv - 1; i >= 0; i--)
        edge.add(i*nu + 0);
    delete[] u;
    delete[] v;
    delete[] x;
    delete[] y;
}

ParametricRegion::~ParametricRegion() {
}

int ParametricRegion::getBaseUNum() {
    return 30;
}

int ParametricRegion::getBaseVNum() {
    return 30;
}
