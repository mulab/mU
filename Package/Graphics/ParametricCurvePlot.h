#pragma once

#include "GraphicsMaker2D.h"
#include "ParametricCurve.h"

class ParametricCurvePlot : GraphicsMaker2D {

private:
    int curnum;
    Curve **curs;
    double tmin, tmax;

    //change by options
    bool colorFunctionSet;
    F1P* cf;
    var tparam;

public:
    ParametricCurvePlot(var expr);
    ~ParametricCurvePlot();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};
