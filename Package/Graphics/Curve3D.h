#pragma once

#include "common.h"

#define ADD_POINT_ANGLE_3D 0.05
#define MIN_DELTA_DIS_3D 0.001

class Curve3D {

public:
    double spx, spy, spz, xmin, xmax, ymin, ymax, zmin, zmax;
    int dataNum;
    ArrayList<Point3d*> vert;

public:
    virtual void creatCurve() = 0;
    ~Curve3D();
};