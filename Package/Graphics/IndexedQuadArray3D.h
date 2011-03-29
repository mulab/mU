#pragma once

#include "common.h"

class IndexedQuadArray3D {

public:
    struct Quad {
        int p11, p12, p21, p22;
        Quad(int a, int b, int c, int d) : p11(a), p12(b), p21(c), p22(d) {}
    };

    struct Mesh {
        ArrayList<int> p;
    };

public:
    double xmin, xmax, ymin, ymax, zmin, zmax;
    ArrayList<Point3d*> verts;
    ArrayList<Quad*> quads;
    ArrayList<Mesh*> meshs;

public:
    IndexedQuadArray3D();
    ~IndexedQuadArray3D();
};
