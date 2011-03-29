#pragma once

#include "GraphicsMaker2D.h"
#include "F2P.h"
#include "ImplicitCurve.h"

class ContourPlot : GraphicsMaker2D {
private:
    int curnum;
    ImplicitCurve *cur;

public:
    ContourPlot(var expr);
    ~ContourPlot();
    var exportGraphics();

protected:
    void setRule(var title, var rule);
};