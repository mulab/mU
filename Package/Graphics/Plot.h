#pragma once

#include "GraphicsMaker2D.h"
#include "CommonCurveWithPlotRange.h"

#define FILL_NULL 0
#define FILL_TO_AXIS 1
#define FILL_TO_CURVE 2

class Plot : GraphicsMaker2D {

private:
    int curnum;
    Curve **curs;

    //change by options
    int filling;
    int fillcur1, fillcur2;
    int mode;
    int rangeSet;
    bool colorFunctionSet;
    F1P* cf;
    var xparam;

public:
    Plot(var expr);
    ~Plot();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};