#pragma once

#include "common.h"

#define ADD_POINT_ANGLE 0.05
#define MIN_DELTA_DIS 0.001

class Curve {

public:
    struct LineStrip {
        ArrayList<Point2d*> vert;
        ArrayList<Color*> color;
    };

public:
    double spx, spy, xmin, xmax, ymin, ymax;
    int dataNum;

    //about color functions
    bool isColorfulCurve;

    ArrayList<LineStrip*> lineStrips;

public:
    Curve();
    Curve(ArrayList<Point2d*> v);
    ~Curve();

protected:
    Color* getRainbowColor(double code);
};