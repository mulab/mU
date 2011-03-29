#pragma once

#include "GraphicsMaker.h"

using namespace mU;

class GraphicsMaker2D : public GraphicsMaker {

protected:
    double xmin, xmax, ymin, ymax;
    int axeX, axeY;

public:
    GraphicsMaker2D();

protected:
    var makeGlobalOptions();
    var makeGraphics(var shape, var opts);
    var makeGraphics(var shape);
    void setCommonRule(var title, var rule);
};