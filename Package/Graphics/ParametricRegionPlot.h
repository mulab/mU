#pragma once

#include "GraphicsMaker2D.h"
#include "ParametricRegion.h"

class ParametricRegionPlot : GraphicsMaker2D {

private:
    int polynum;
    IndexedQuadArray** polys;
    double umin, umax, vmin, vmax;

public:
    ParametricRegionPlot(var expr);
    ~ParametricRegionPlot();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};
