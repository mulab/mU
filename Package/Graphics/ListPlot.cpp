#include "ListPlot.h"

using namespace std;

// Color BLUE(0,0,255);
// Color GREEN(0,255,0);
// Color RED(255,0,0);
// int colorListNum = 3;
// Color colorList[] = {BLUE,RED,GREEN};
//
// Hue hue0(0.67, 0.6, 0.6);
// Hue hue1(0.9060679774997897, 0.6, 0.6);
// Hue hue2(0.14213595499957954, 0.6, 0.6);
// Hue hue3(0.37820393249936934, 0.6, 0.6);
// int hueListNum = 4;
// Hue hueList[] = {hue0,hue1,hue2,hue3};

ListPlot::ListPlot(var cmd) {
    filling = FILL_NULL;
    varplist = At(cmd, 0);
    readVerts2d(varplist, &pnum, &plist);
    for (int i = 0; i < pnum; i++) {
    }
    int len = Size(cmd);
    if (ExQ(At(cmd, len - 1), TAG(Options))) {
        var opts = Body((At(cmd, len - 1)));
        readOptions(opts);
    }
    xmin = plist[0].x;
    xmax = plist[0].x;
    ymin = plist[0].y;
    ymax = plist[0].y;
    for (int i = 1; i < pnum; i++) {
        if (plist[i].x < xmin)
            xmin = plist[i].x;
        if (plist[i].x > xmax)
            xmax = plist[i].x;
        if (plist[i].y < ymin)
            ymin = plist[i].y;
        if (plist[i].y > ymax)
            ymax = plist[i].y;
    }
}

ListPlot::~ListPlot() {
    delete plist;
}

var ListPlot::exportGraphics() {
    var shapelist = Vec(1);
    //var opt = makeColorOption(&colorList[0]);
    Hue* hue = getHue(0);
    var opt = makeHueOption(hue);
    delete hue;
    At(shapelist, 0) = Ex(Sym(L"PointList"), Vec(varplist, opt));
    var shape = Ex(Sym(L"Shape"), shapelist);
    var opts = makeGlobalOptions();
    var graphics = makeGraphics(shape, opts);
    return graphics;
}

void ListPlot::setRule(var title, var rule) {
    setCommonRule(title, rule);
}
