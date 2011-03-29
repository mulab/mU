#pragma once

#include "GraphicsMaker3D.h"
#include "ParametricSurface.h"

class ParametricSurfacePlot3D : GraphicsMaker3D {

private:
    int polynum;
    IndexedQuadArray3D** polys;
    double umin, umax, vmin, vmax, wmin, wmax;

public:
    ParametricSurfacePlot3D(var expr);
    ~ParametricSurfacePlot3D();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};
