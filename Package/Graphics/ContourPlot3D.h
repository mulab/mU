#pragma once

#include "GraphicsMaker3D.h"
#include "F3P.h"
#include "ImplicitSurface.h"

class ContourPlot3D : GraphicsMaker3D {
private:
    int surnum;
    ImplicitSurface *sur;

public:
    ContourPlot3D(var expr);
    ~ContourPlot3D();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};