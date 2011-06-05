#pragma once

#include "F2P.h"
#include "GraphicsMaker.h"

//#define ADD_POINT_ANGLE 0.3
#define ADD_POINT_ANGLE_SURFACE 0.1
#define MAX_SPLIT_DEPTH 0
#define LARGE 1000000

using namespace mU;

struct IndexPolygonForSurface {
    int p11, p12, p21, p22;
    int xlineid;
    int ylineid;
    bool x0line;
    bool y0line;
    IndexPolygonForSurface(int a, int b, int c, int d, int xid, int yid, bool isx0line, bool isy0line)
            : p11(a), p12(b), p21(c), p22(d), xlineid(xid), ylineid(yid), x0line(isx0line), y0line(isy0line) {}
};

class IndexStripLine {
public:
    ArrayList<int> p;
    ArrayList<Point3d*>* vert;
public:
    IndexStripLine(ArrayList<Point3d*>* vert) {
        this->vert = vert;
    }
};

class IndexStripLineX : public IndexStripLine {
public:
    IndexStripLineX(ArrayList<Point3d*>* vert) : IndexStripLine(vert) {}
    void add(int index) {
        unsigned int i;
        for (i = 0; i < p.size() && ((*vert)[index]->x > (*vert)[p[i]]->x); i++);
        p.add(i, index);
    }
};

class IndexStripLineY : public IndexStripLine {
public:
    IndexStripLineY(ArrayList<Point3d*>* vert) : IndexStripLine(vert) {}
    void add(int index) {
        unsigned int i;
        for (i = 0; i < p.size() && ((*vert)[index]->y > (*vert)[p[i]]->y); i++);
        p.add(i, index);
    }
};

class Surface {

public:
    F2P* f;
    int nx, ny;
    double spx, spy, xmin, xmax, ymin, ymax, spz, zmin, zmax;
    double graphScalex, graphScaley;
    double dx, dy;
    ArrayList<Point3d*> vert;
    ArrayList<IndexPolygonForSurface*> sur;
    IndexStripLineX **xline;
    IndexStripLineY **yline;

public:
    Surface(F2P* f3d, double xstart, double xend, double ystart, double yend);
    ~Surface();

private:
    int getNx();
    int getNy();
    void trySplitPolygonX(unsigned int id, int depth);
    void trySplitPolygonY(unsigned int id, int depth);
};
