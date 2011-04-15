#include "ParametricRegionPlot.h"

ParametricRegionPlot::ParametricRegionPlot(var cmd) {
    var expr = At(cmd, 0);
    var urange = At(cmd, 1);
    var vrange = At(cmd, 2);
    int len = Size(cmd);
    if (ExQ(At(cmd, len - 1), TAG(Options))) {
        var opts = Body((At(cmd, len - 1)));
        readOptions(opts);
    }
    var uparam = At(urange, 0);
    umin = N::D(Evalf(At(urange, 1)));
    umax = N::D(Evalf(At(urange, 2)));
    var vparam = At(vrange, 0);
    vmin = N::D(Evalf(At(vrange, 1)));
    vmax = N::D(Evalf(At(vrange, 2)));
    if (VecQ(At(expr, 0))) {
        polynum = Size(expr);
        polys = new IndexedQuadArray*[polynum];
        for (int i = 0; i < polynum; i++) {
            var funs = At(expr, i);
            F2P *fx = new F2P(At(funs, 0), uparam, vparam);
            F2P *fy = new F2P(At(funs, 1), uparam, vparam);
            polys[i] = (IndexedQuadArray*)(new ParametricRegion(fx, fy, umin, umax, vmin, vmax));
            delete fx;
            delete fy;
        }
    } else {
        polynum = 1;
        polys = new IndexedQuadArray*[1];
        var funs = expr;
        F2P *fx = new F2P(At(funs, 0), uparam, vparam);
        F2P *fy = new F2P(At(funs, 1), uparam, vparam);
        polys[0] = (IndexedQuadArray*)(new ParametricRegion(fx, fy, umin, umax, vmin, vmax));
        delete fx;
        delete fy;
    }
    xmin = polys[0]->xmin;
    xmax = polys[0]->xmax;
    ymin = polys[0]->ymin;
    ymax = polys[0]->ymax;
    for (int i = 1; i < polynum; i++) {
        if (polys[i]->ymin < ymin)
            ymin = polys[i]->ymin;
        if (polys[i]->ymax > ymax)
            ymax = polys[i]->ymax;
        if (polys[i]->xmin < xmin)
            xmin = polys[i]->xmin;
        if (polys[i]->xmax > xmax)
            xmax = polys[i]->xmax;
    }
}

ParametricRegionPlot::~ParametricRegionPlot() {
    for (int i = 0; i < polynum; i++) {
        delete polys[i];
    }
    delete polys;
}

var ParametricRegionPlot::exportGraphics() {
    var shapelist = Vec(polynum);
    for (int i = 0; i < polynum; i++) {
        At(shapelist, i) = makeIndexedPolygon(polys[i], i);
    }
    var shape = Ex(Sym(L"Shape"), shapelist);
    var opts = makeGlobalOptions();
    var graphics = makeGraphics(shape, opts);
    return graphics;
}

void ParametricRegionPlot::setRule(var title, var rule) {
    setCommonRule(title, rule);
}
