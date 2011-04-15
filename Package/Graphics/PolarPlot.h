#pragma once

#include "GraphicsMaker2D.h"
#include "PolarCurve.h"

class PolarPlot : GraphicsMaker2D {

private:
    int curnum;
    Curve **curs;
    double tmin, tmax;

    //change by options
    bool colorFunctionSet;
    F1P* cf;
    var tparam;

public:
    PolarPlot(var expr);
    ~PolarPlot();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};
