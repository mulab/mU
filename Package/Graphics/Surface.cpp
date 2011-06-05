#include "common.h"
#include "Surface.h"

using namespace std;

Surface::Surface(F2P* f3d, double xstart, double xend, double ystart, double yend) {
    this->f = f3d;
    xmin = xstart;
    xmax = xend;
    ymin = ystart;
    ymax = yend;
    spx = xmax - xmin;
    spy = ymax - ymin;
    nx = getNx();
    ny = getNy();
    double *x = new double[nx*ny];
    double *y = new double[nx*ny];
    double *z = new double[nx*ny];
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            x[i*nx+j] = xmin + (xmax - xmin) * (double)j / (double)(nx - 1);
        }
    }
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            y[i*nx+j] = ymin + (ymax - ymin) * (double)i / (double)(ny - 1);
        }
    }
    f->getArrayData(x, y, z, nx*ny);
    for (int i = 0; i < nx*ny; i++) {
        vert.add(new Point3d(x[i], y[i], z[i]));
    }
    xline = new IndexStripLineX*[ny];
    yline = new IndexStripLineY*[nx];
    for (int i = 0; i < ny; i++) {
        xline[i] = new IndexStripLineX(&vert);
    }
    for (int i = 0; i < nx; i++) {
        yline[i] = new IndexStripLineY(&vert);
    }
    sur.resize(0);
    int count = 0;
    for (int y = 0; y < ny - 1; y++) {
        for (int x = 0; x < nx - 1; x++) {
            int v = nx * y + x;
            int a = v;
            int b = v + 1;
            v += nx;
            int c = v;
            int d = v + 1;
            sur.add(new IndexPolygonForSurface(a, b, c, d, y + 1, x + 1, y == 0, x == 0));
            xline[y+1]->add(d);
            yline[x+1]->add(d);
            if (y == 0) {
                xline[0]->add(b);
                yline[x+1]->add(b);
            }
            if (x == 0) {
                yline[0]->add(c);
                xline[y+1]->add(c);
            }
            if (x == 0 && y == 0) {
                xline[0]->add(a);
                yline[0]->add(a);
            }
        }
    }
    delete x;
    delete y;
    delete z;
    zmin = vert[0]->z;
    zmax = vert[0]->z;
    for (int i = 0; i < nx*ny; i++) {
        if (vert[i]->z > zmax) zmax = vert[i]->z;
        if (vert[i]->z < zmin) zmin = vert[i]->z;
    }
    spz = zmax - zmin;
    graphScalex = spz / spx;
    graphScaley = spz / spy;
    dx = spx / LARGE;
    dy = spy / LARGE;
    unsigned int surnum = sur.size();
    for (unsigned int i = 0; i < surnum; i++) {
        trySplitPolygonX(i, 1);
    }
    surnum = sur.size();
    for (unsigned int i = 0; i < surnum; i++) {
        trySplitPolygonY(i, 1);
    }
}

Surface::~Surface() {
    for (int i = 0; i < ny; i++) {
        delete xline[i];
    }
    for (int i = 0; i < nx; i++) {
        delete yline[i];
    }
    delete xline;
    delete yline;
}

int Surface::getNx() {
    return 20;
    //return 3;
}

int Surface::getNy() {
    return 20;
    //return 3;
}

void Surface::trySplitPolygonX(unsigned int id, int depth) {
    if (depth > MAX_SPLIT_DEPTH)
        return;
    Point3d* p1 = vert[sur[id]->p11];
    Point3d* p2 = vert[sur[id]->p12];
    double netk1 = f->getDerivativeX(p1->x, p1->y, dx) / graphScalex;
    double netk2 = f->getDerivativeX(p2->x, p2->y, -dx) / graphScalex;
    double dangle = acos((1 + netk1 * netk2) / sqrt(1 + netk1 * netk1) / sqrt(1 + netk2 * netk2));
    if (dangle > ADD_POINT_ANGLE_SURFACE) {	//need to add data
        double x = (p1->x + p2->x) / 2;
        double y = vert[sur[id]->p11]->y;
        double z = f->getSingleData(x, y);
        vert.add(new Point3d(x, y, z));
        y = vert[sur[id]->p21]->y; //creat another point
        z = f->getSingleData(x, y);
        vert.add(new Point3d(x, y, z));
        sur.add(new IndexPolygonForSurface(vert.size() - 2, sur[id]->p12, vert.size() - 1, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0));
        sur[id]->p12 = vert.size() - 2;
        sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
        if (sur[id]->xlineid != -1) {
            xline[sur[id]->xlineid]->add(vert.size() - 1);
        }
        if (sur[id]->x0line) {
            xline[0]->add(vert.size() - 2);
        }
        sur[id]->ylineid = -1;
        if (z > zmax) zmax = z;
        if (z < zmin) zmin = z;
        trySplitPolygonX(sur.size() - 1, depth + 1); //continue split the two polygon
        trySplitPolygonX(id, depth + 1);
    } else { //try add data
        double x = (p1->x + p2->x) / 2;
        double y = p1->y;
        double netk3 = f->getDerivativeX(x, y, dx) / graphScalex;
        if ((netk3 < netk1 && netk3 < netk2) || (netk3 > netk1 && netk3 > netk2)) { //有拐点
            double z = f->getSingleData(x, y);
            vert.add(new Point3d(x, y, z));
            y = vert[sur[id]->p21]->y; //creat another point
            z = f->getSingleData(x, y);
            vert.add(new Point3d(x, y, z));
            sur.add(new IndexPolygonForSurface(vert.size() - 2, sur[id]->p12, vert.size() - 1, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0));
            sur[id]->p12 = vert.size() - 2;
            sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
            if (sur[id]->xlineid != -1) {
                xline[sur[id]->xlineid]->add(vert.size() - 1);
            }
            if (sur[id]->x0line) {
                xline[0]->add(vert.size() - 2);
            }
            sur[id]->ylineid = -1;
            if (z > zmax) zmax = z;
            if (z < zmin) zmin = z;
            trySplitPolygonX(sur.size() - 1, depth + 1); //continue split the two polygon
            trySplitPolygonX(id, depth + 1);
        }
    }
}

void Surface::trySplitPolygonY(unsigned int id, int depth) {
    if (depth > MAX_SPLIT_DEPTH)
        return;
    Point3d* p1 = vert[sur[id]->p11];
    Point3d* p2 = vert[sur[id]->p21];
    double netk1 = f->getDerivativeY(p1->x, p1->y, dy) / graphScaley;
    double netk2 = f->getDerivativeY(p2->x, p2->y, -dy) / graphScaley;
    double dangle = acos((1 + netk1 * netk2) / sqrt(1 + netk1 * netk1) / sqrt(1 + netk2 * netk2));
    if (dangle > ADD_POINT_ANGLE_SURFACE) {	//need to add data
        double x = p1->x;
        double y = (p1->y + p2->y) / 2;
        double z = f->getSingleData(x, y);
        vert.add(new Point3d(x, y, z));
        x = vert[sur[id]->p12]->x; //creat another point
        z = f->getSingleData(x, y);
        vert.add(new Point3d(x, y, z));
        sur.add(new IndexPolygonForSurface(vert.size() - 2, vert.size() - 1, sur[id]->p21, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0));
        sur[id]->p21 = vert.size() - 2;
        sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
        if (sur[id]->ylineid != -1) {
            yline[sur[id]->ylineid]->add(vert.size() - 1);
        }
        if (sur[id]->y0line) {
            yline[0]->add(vert.size() - 2);
        }
        sur[id]->xlineid = -1;
        if (z > zmax) zmax = z;
        if (z < zmin) zmin = z;
        trySplitPolygonY(sur.size() - 1, depth + 1); //continue split the two polygon
        trySplitPolygonY(id, depth + 1);
    } else { //try add data
        double x = p1->x;
        double y = (p1->y + p2->y) / 2;
        double netk3 = f->getDerivativeY(x, y, dy) / graphScaley;
        if ((netk3 < netk1 && netk3 < netk2) || (netk3 > netk1 && netk3 > netk2)) { //有拐点
            double z = f->getSingleData(x, y);
            vert.add(new Point3d(x, y, z));
            x = vert[sur[id]->p12]->x; //creat another point
            z = f->getSingleData(x, y);
            vert.add(new Point3d(x, y, z));
            sur.add(new IndexPolygonForSurface(vert.size() - 2, vert.size() - 1, sur[id]->p21, sur[id]->p22, sur[id]->xlineid, sur[id]->ylineid, 0, 0));
            sur[id]->p21 = vert.size() - 2;
            sur[id]->p22 = vert.size() - 1; //change the old polygon and creat a new one
            if (sur[id]->ylineid != -1) {
                yline[sur[id]->ylineid]->add(vert.size() - 1);
            }
            if (sur[id]->y0line) {
                yline[0]->add(vert.size() - 2);
            }
            sur[id]->xlineid = -1;
            if (z > zmax) zmax = z;
            if (z < zmin) zmin = z;
            trySplitPolygonY(sur.size() - 1, depth + 1); //continue split the two polygon
            trySplitPolygonY(id, depth + 1);
        }
    }
}
