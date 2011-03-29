#pragma once

#include "GraphicsMaker3D.h"
#include "ParametricCurve3D.h"

class ParametricCurvePlot3D : GraphicsMaker3D {

private:
    ParametricCurve3D *curve;
    double tmin, tmax;

public:
    ParametricCurvePlot3D(var expr);
    ~ParametricCurvePlot3D();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};
