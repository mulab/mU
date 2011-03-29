#pragma once

#include "common.h"

class IndexedQuadArray {

public:
    struct Quad {
        int p11, p12, p21, p22;
        Quad(int a, int b, int c, int d) : p11(a), p12(b), p21(c), p22(d) {}
    };

public:
    double xmin, xmax, ymin, ymax;
    ArrayList<Point2d*> verts;
    ArrayList<Quad*> quads;
    ArrayList<int> edge;

public:
    IndexedQuadArray();
    ~IndexedQuadArray();
};
