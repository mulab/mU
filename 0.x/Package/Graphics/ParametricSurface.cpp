#include "common.h"
#include "ParametricSurface.h"

using namespace std;

//fix me
ParametricSurface::ParametricSurface(F2P *ffx, F2P *ffy, F2P *ffz, double umin, double umax, double vmin, double vmax) {
    fx = ffx;
    fy = ffy;
    fz = ffz;
    this->umin = umin;
    this->umax = umax;
    this->vmin = vmin;
    this->vmax = vmax;
    spu = umax - umin;
    spv = vmax - vmin;
    numesh = getUMeshNum();
    nvmesh = getVMeshNum();
    wumesh = getUMeshWidth();
    wvmesh = getVMeshWidth();
    nu = (numesh + 1) * wumesh + 1;
    nv = (nvmesh + 1) * wvmesh + 1;
    double *u = new double[nu*nv];
    double *v = new double[nu*nv];
    double *x = new double[nu*nv];
    double *y = new double[nu*nv];
    double *z = new double[nu*nv];
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
    fz->getArrayData(u, v, z, nu*nv);
    xmin = x[0];
    xmax = x[0];
    ymin = y[0];
    ymax = y[0];
    zmin = y[0];
    zmax = y[0];
    for (int i = 0; i < nu*nv; i++) {
        if (x[i] > xmax) xmax = x[i];
        if (x[i] < xmin) xmin = x[i];
        if (y[i] > ymax) ymax = y[i];
        if (y[i] < ymin) ymin = y[i];
        if (z[i] > zmax) zmax = z[i];
        if (z[i] < zmin) zmin = z[i];
    }
    for (int i = 0; i < nu*nv; i++) {
        verts.add(new Point3d(x[i], y[i], z[i]));
    }
    for (int i = 0; i < nv - 1; i++)
        for (int j = 0; j < nu - 1; j++) {
            quads.add(new Quad(i*nu + j, i*nu + j + 1, (i + 1)*nu + j, (i + 1)*nu + j + 1));
        }
    for (int j = 0; j < numesh; j++) {
        Mesh *mesh = new Mesh;
        mesh->p.clear();
        for (int i = 0; i < nv; i++)
            mesh->p.add(i*nu + j*wumesh);
        meshs.add(mesh);
    }
    for (int i = 0; i < nvmesh; i++) {
        Mesh *mesh = new Mesh;
        mesh->p.clear();
        for (int j = 0; j < nu; j++)
            mesh->p.add(i*wvmesh*nu + j);
        meshs.add(mesh);
    }
    delete[] u;
    delete[] v;
    delete[] x;
    delete[] y;
    delete[] z;
}

ParametricSurface::~ParametricSurface() {
}

int ParametricSurface::getUMeshNum() {
    return 10;
}

int ParametricSurface::getVMeshNum() {
    return 10;
}

int ParametricSurface::getUMeshWidth() {
    return 3;
}

int ParametricSurface::getVMeshWidth() {
    return 3;
}
