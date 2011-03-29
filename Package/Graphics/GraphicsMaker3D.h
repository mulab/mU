#pragma once

#include "GraphicsMaker.h"

using namespace mU;

#define NORMAL_SPACE 0
#define FLAT_SPACE 1

class GraphicsMaker3D : public GraphicsMaker {

protected:
    double xmin, xmax, ymin, ymax, zmin, zmax;
    int spacetype;

public:
    GraphicsMaker3D();

protected:
    var makeGlobalOptions();
    var makeGraphics(var shape, var opts);
    var makeGraphics(var shape);
    void setCommonRule(var title, var rule);
};