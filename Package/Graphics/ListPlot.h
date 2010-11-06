#pragma once

#include "GraphicsMaker2D.h"

#define FILL_NULL 0
#define FILL_AXIS 1

class ListPlot : GraphicsMaker2D {

private:
    int pnum;
    int filling;
    Point2d* plist;
    var varplist;
    Color* color;

public:
    ListPlot(var expr);
    ~ListPlot();
    var exportGraphics();

private:
    var makePointList(Point2d* vert, int num, Color* color);
    var makeColorOption(Color* color);

protected:
    void setRule(var title, var rule);
};