#pragma once

#include "CommonCurve.h"

using namespace mU;

#define MAX_GET_POINT_ON_RANGE_DEPTH 20
#define MAX_DERIVATIVE 20

class CommonCurveWithPlotRange : public CommonCurve {

public:
    CommonCurveWithPlotRange(F1P *f, double xmin, double xmax); //auto plot range
    CommonCurveWithPlotRange(F1P *f, double xmin, double xmax, double ymin, double ymax);
    CommonCurveWithPlotRange(F1P *f, double xmin, double xmax, F1P *cf);
    CommonCurveWithPlotRange(F1P *f, double xmin, double xmax, double ymin, double ymax, F1P *cf);
    ~CommonCurveWithPlotRange();

protected:
    int getBaseNum(); //Override
    void makePlotRange();
    void makePoints();
    double getPointOnRangeFromLeft(double x1, double x2, int depth);
    double getPointOnRangeFromRight(double x1, double x2, int depth);
};
