#include "common.h"
#include "ParametricCurve3D.h"

using namespace std;

ParametricCurve3D::ParametricCurve3D(F1P *ffx, F1P *ffy, F1P *ffz, double start, double end) {
    fx = ffx;
    fy = ffy;
    fz = ffz;
    tmin = start;
    tmax = end;
    spt = tmax - tmin;
    dataNum = getBaseNum();
    double *t = new double[dataNum];
    double *x = new double[dataNum];
    double *y = new double[dataNum];
    double *z = new double[dataNum];
    for (int i = 0; i < dataNum; i++) {
        //把区间等分为dataNum-1份
        t[i] = tmin + (tmax - tmin) * i / (dataNum - 1);
    }
    fx->getArrayData(t, x, dataNum);
    fy->getArrayData(t, y, dataNum);
    fz->getArrayData(t, z, dataNum);
    for (int i = 0; i < dataNum; i++) {
        vert.add(new Point3d(x[i], y[i], z[i]));
        arrayT.add(t[i]);
    }
    delete t;
    delete x;
    delete y;
    delete z;
    xmin = vert[0]->x;
    xmax = vert[0]->x;
    ymin = vert[0]->y;
    ymax = vert[0]->y;
    zmin = vert[0]->z;
    zmax = vert[0]->z;
    for (int i = 0; i < dataNum; i++) {
        if (vert[i]->z > zmax) zmax = vert[i]->z;
        if (vert[i]->z < zmin) zmin = vert[i]->z;
        if (vert[i]->y > ymax) ymax = vert[i]->y;
        if (vert[i]->y < ymin) ymin = vert[i]->y;
        if (vert[i]->x > xmax) xmax = vert[i]->x;
        if (vert[i]->x < xmin) xmin = vert[i]->x;
    }
    spx = xmax - xmin;
    spy = ymax - ymin;
    spz = zmax - zmin;
    dt = spt / LARGE;
    addVerts();
}

void ParametricCurve3D::creatCurve() {
}

ParametricCurve3D::~ParametricCurve3D() {
    for (unsigned int i = 0; i < vert.size(); i++) {
        delete vert[i];
    }
    vert.clear();
}

int ParametricCurve3D::getBaseNum() {
    return 20;
}

void ParametricCurve3D::addVerts() {
    ArrayList<Vector3D*> vcaled; //save the vector calculated
    vcaled.add(getVector(arrayT[0], dt));
    unsigned int i = 0;
    while (i < vert.size() - 1) {
        double dis = sqrt(((vert[i+1]->x - vert[i]->x) / spx) * ((vert[i+1]->x - vert[i]->x) / spx) + ((vert[i+1]->y - vert[i]->y) / spy) * ((vert[i+1]->y - vert[i]->y) / spy) + ((vert[i+1]->z - vert[i]->z) / spz) * ((vert[i+1]->z - vert[i]->z) / spz));
        if (dis < MIN_DELTA_DIS_3D) {
            i++;
            vcaled.add(getVector(arrayT[i], dt));
            continue;
        }
        Vector3D v1 = *(vcaled[i]);
        Vector3D v2 = *(getVector(arrayT[i+1], -dt));
        double dangle = acos((v1 * v2) / sqrt(v1 * v1) / sqrt(v2 * v2));
        if (dangle > ADD_POINT_ANGLE_3D) {
            double t = (arrayT[i] + arrayT[i+1]) / 2;
            double x = fx->getSingleData(t);
            double y = fy->getSingleData(t);
            double z = fz->getSingleData(t);
            vert.add(i + 1, new Point3d(x, y, z));
            arrayT.add(i + 1, t);
            if (x > xmax) xmax = x;
            if (x < xmin) xmin = x;
            if (y > ymax) ymax = y;
            if (y < ymin) ymin = y;
            if (z > zmax) zmax = z;
            if (z < zmin) zmin = z;
        } else { //尝试添加
            double t = (arrayT[i] + arrayT[i+1]) / 2;
            Vector3D v3 = *(getVector(t, -dt));
            if ((v1*v3)*(v2*v3) < 0) {
                double t = (arrayT[i] + arrayT[i+1]) / 2;
                double x = fx->getSingleData(t);
                double y = fy->getSingleData(t);
                double z = fz->getSingleData(t);
                vert.add(i + 1, new Point3d(x, y, z));
                arrayT.add(i + 1, t);
                if (x > xmax) xmax = x;
                if (x < xmin) xmin = x;
                if (y > ymax) ymax = y;
                if (y < ymin) ymin = y;
                if (z > zmax) zmax = z;
                if (z < zmin) zmin = z;
            } else {
                i++;
                vcaled.add(getVector(arrayT[i], dt));
            }
        }
    }
    spx = xmax - xmin;
    spy = ymax - ymin;
    for (unsigned int i = 0; i < vcaled.size(); i++) {
        delete vcaled[i];
    }
    vcaled.clear();
}

//获得切向量
Vector3D* ParametricCurve3D::getVector(double t, double dt) {
    return new Vector3D(fx->getDerivative(t, dt) / spx, fy->getDerivative(t, dt) / spy, fz->getDerivative(t, dt) / spz);
}
