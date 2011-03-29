#pragma once

#include "GraphicsMaker3D.h"
#include "Surface.h"

class Plot3D : GraphicsMaker3D {
private:
    int surnum;
    Surface **sur;

public:
    Plot3D(var expr);
    ~Plot3D();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};