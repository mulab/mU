#include "ParametricCurvePlot3D.h"

ParametricCurvePlot3D::ParametricCurvePlot3D(var cmd) {
    spacetype = NORMAL_SPACE;
    var expr = At(cmd, 0);
    var trange = At(cmd, 1);
    var tparam = At(trange, 0);
    tmin = N::D(Evalf(At(trange, 1)));
    tmax = N::D(Evalf(At(trange, 2)));
    int len = Size(cmd);
    if (ExQ(At(cmd, len - 1), TAG(Options))) {
        var opts = Body((At(cmd, len - 1)));
        readOptions(opts);
    }
    F1P *fx = new F1P(At(expr, 0), tparam);
    F1P *fy = new F1P(At(expr, 1), tparam);
    F1P *fz = new F1P(At(expr, 2), tparam);
    curve = new ParametricCurve3D(fx, fy, fz, tmin, tmax);
    delete fx;
    delete fy;
    delete fz;
    xmin = curve->xmin;
    xmax = curve->xmax;
    ymin = curve->ymin;
    ymax = curve->ymax;
    zmin = curve->zmin;
    zmax = curve->zmax;
}

ParametricCurvePlot3D::~ParametricCurvePlot3D() {
    delete curve;
}

var ParametricCurvePlot3D::exportGraphics() {
    var shapelist = Vec(1);
    Hue* hue = getHue(0);
    At(shapelist, 0) = makeLine(curve, hue);
    delete hue;
    var shapeexpr = Ex(Sym(L"Shape"), shapelist);
    var options = makeGlobalOptions();
    var graphics = Ex(TAG(Graphics3D), Vec(shapeexpr, options));
    return graphics;
}

void ParametricCurvePlot3D::setRule(var title, var rule) {
    setCommonRule(title, rule);
}


