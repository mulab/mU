#include "CommonCurve.h"

using namespace std;

CommonCurve::CommonCurve() {
}

CommonCurve::CommonCurve(F1P *f1, double start, double end) {
    LineStrip* lineStrip = new LineStrip;
    lineStrips.add(lineStrip);
    f = f1;
    xmin = start;
    xmax = end;
    spx = xmax - xmin;
    makePoints();
}

CommonCurve::CommonCurve(F1P *f1, double start, double end, F1P *cf1) {
    LineStrip* lineStrip = new LineStrip;
    lineStrips.add(lineStrip);
    f = f1;
    xmin = start;
    xmax = end;
    spx = xmax - xmin;
    cf = cf1;
    isColorfulCurve = true;
    makePoints();
    useColorFunction();
}

CommonCurve::~CommonCurve() {
}

void CommonCurve::makePoints() {
    dataNum = getBaseNum();
    double *x = new double[dataNum];
    double *y = new double[dataNum];
    for (int i = 0; i < dataNum; i++) {
        //split the domain into dataNum-1 parts
        x[i] = xmin + (xmax - xmin) * i / (dataNum - 1);
    }
    f->getArrayData(x, y, dataNum);
    for (int i = 0; i < dataNum; i++) {
        lineStrips[0]->vert.add(new Point2d(x[i], y[i]));
    }
    delete x;
    delete y;
    ymin = lineStrips[0]->vert[0]->y;
    ymax = lineStrips[0]->vert[0]->y;
    for (int i = 0; i < dataNum; i++) {
        if (lineStrips[0]->vert[i]->y > ymax) ymax = lineStrips[0]->vert[i]->y;
        if (lineStrips[0]->vert[i]->y < ymin) ymin = lineStrips[0]->vert[i]->y;
    }
    spx = xmax - xmin;
    spy = ymax - ymin;
    graphScale = spy / spx;
    dx = spx / LARGE;
    addVerts(lineStrips[0]);
}

void CommonCurve::addVerts(LineStrip* lineStrip) {
    //add points where the angle between two lines is too large
    ArrayList<double> kcaled; //save the k calculated
    kcaled.add(f->getDerivative(lineStrip->vert[0]->x, dx) / graphScale); //get k0
    unsigned int i = 0;
    while (i < lineStrip->vert.size() - 1) {
        double dis = sqrt(((lineStrip->vert[i+1]->x - lineStrip->vert[i]->x) / spx) * ((lineStrip->vert[i+1]->x - lineStrip->vert[i]->x) / spx) + ((lineStrip->vert[i+1]->y - lineStrip->vert[i]->y) / spy) * ((lineStrip->vert[i+1]->y - lineStrip->vert[i]->y) / spy));
        if (dis < MIN_DELTA_DIS) {
            i++;
            kcaled.add(f->getDerivative(lineStrip->vert[i]->x, dx) / graphScale);
            continue;  //line is too short
        }
        double netk1 = kcaled[i];
        double netk2 = f->getDerivative(lineStrip->vert[i+1]->x, -dx) / graphScale; //always divided by graph scale
        double dangle = acos((1 + netk1 * netk2) / sqrt(1 + netk1 * netk1) / sqrt(1 + netk2 * netk2));
        if (dangle > ADD_POINT_ANGLE) {	//need to add points
            double x = (lineStrip->vert[i]->x + lineStrip->vert[i+1]->x) / 2;
            double y = f->getSingleData(x);
            lineStrip->vert.add(i + 1, new Point2d(x, y));
            if (y > ymax) ymax = y;
            if (y < ymin) ymin = y;
        } else { //try adding points
            double x = (lineStrip->vert[i]->x + lineStrip->vert[i+1]->x) / 2;
            double netk3 = f->getDerivative(x, dx) / graphScale;
            if ((netk3 < netk1 && netk3 < netk2) || (netk3 > netk1 && netk3 > netk2)) {
                double y = f->getSingleData(x);
                lineStrip->vert.add(i + 1, new Point2d(x, y));
                if (y > ymax) ymax = y;
                if (y < ymin) ymin = y;
            } else {
                i++;
                kcaled.add(f->getDerivative(lineStrip->vert[i]->x, dx) / graphScale);
            }
        }
    }
    spx = xmax - xmin;
    spy = ymax - ymin;
    graphScale = spy / spx;
}

int CommonCurve::getBaseNum() {
    //should not be to large
    return 20;
}

void CommonCurve::useColorFunction() {
    double cfmin, cfmax;
    cfmin = cf->getSingleData(lineStrips[0]->vert[0]->x);
    cfmax = cfmin;
    for (unsigned int i = 0; i < lineStrips.size(); i++) {
        LineStrip* lineStrip = lineStrips[i];
        for (unsigned int j = 0; j < lineStrip->vert.size(); j++) {
            double tmp = cf->getSingleData(lineStrip->vert[j]->x);
            if (tmp > cfmax)
                cfmax = tmp;
            if (tmp < cfmin)
                cfmin = tmp;
        }
    }
    for (unsigned int i = 0; i < lineStrips.size(); i++) {
        LineStrip* lineStrip = lineStrips[i];
        for (unsigned int j = 0; j < lineStrip->vert.size(); j++) {
            double tmp = cf->getSingleData(lineStrip->vert[j]->x);
            lineStrip->color.add(j, getRainbowColor((tmp - cfmin) / (cfmax - cfmin)));
        }
    }
}