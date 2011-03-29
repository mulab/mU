#include "ParametricCurvePlot.h"

ParametricCurvePlot::ParametricCurvePlot(var cmd) {
    colorFunctionSet = false;
    var expr = At(cmd, 0);
    var trange = At(cmd, 1);
    tparam = At(trange, 0);
    tmin = N::D(Evalf(At(trange, 1)));
    tmax = N::D(Evalf(At(trange, 2)));
    int len = Size(cmd);
    if (ExQ(At(cmd, len - 1), TAG(Options))) {
        var opts = Body((At(cmd, len - 1)));
        readOptions(opts);
    }
    if (VecQ(At(expr, 0))) {
        curnum = Size(expr);
        curs = new Curve*[curnum];
        for (int i = 0; i < curnum; i++) {
            var funs = At(expr, i);
            F1P *fx = new F1P(At(funs, 0), tparam);
            F1P *fy = new F1P(At(funs, 1), tparam);
            if (!colorFunctionSet)
                curs[i] = (Curve*)(new ParametricCurve(fx, fy, tmin, tmax));
            else
                curs[i] = (Curve*)(new ParametricCurve(fx, fy, tmin, tmax, cf));
            delete fx;
            delete fy;
        }
    } else {
        curnum = 1;
        curs = new Curve*[1];
        var funs = expr;
        F1P *fx = new F1P(At(funs, 0), tparam);
        F1P *fy = new F1P(At(funs, 1), tparam);
        if (!colorFunctionSet)
            curs[0] = (Curve*)(new ParametricCurve(fx, fy, tmin, tmax));
        else
            curs[0] = (Curve*)(new ParametricCurve(fx, fy, tmin, tmax, cf));
        delete fx;
        delete fy;
    }
    if (colorFunctionSet)
        delete cf;
    xmin = curs[0]->xmin;
    xmax = curs[0]->xmax;
    ymin = curs[0]->ymin;
    ymax = curs[0]->ymax;
    for (int i = 1; i < curnum; i++) {
        if (curs[i]->ymin < ymin)
            ymin = curs[i]->ymin;
        if (curs[i]->ymax > ymax)
            ymax = curs[i]->ymax;
        if (curs[i]->xmin < xmin)
            xmin = curs[i]->xmin;
        if (curs[i]->xmax > xmax)
            xmax = curs[i]->xmax;
    }
}

ParametricCurvePlot::~ParametricCurvePlot() {
    for (int i = 0; i < curnum; i++) {
        delete curs[i];
    }
    delete curs;
}

var ParametricCurvePlot::exportGraphics() {
    var shapelist;
    if (!colorFunctionSet) {
        shapelist = Vec(curnum);
        for (int i = 0; i < curnum; i++) {
            Hue* hue = getHue(i);
            At(shapelist, i) = makeLine(curs[i], hue);
            delete hue;
        }
    } else {
        shapelist = makeColorfulLines(curs[0]); //to be added
        for (int i = 1; i < curnum; i++) {
            var lines = makeColorfulLines(curs[i]);
            shapelist = addVec(shapelist, lines);
        }
    }
    var shape = Ex(Sym(L"Shape"), shapelist);
    var opts = makeGlobalOptions();
    var graphics = makeGraphics(shape, opts);
    return graphics;
}

void ParametricCurvePlot::setRule(var title, var rule) {
    if (title == Sym(L"ColorFunction")) { //has color function
        colorFunctionSet = true;
        var fun = At(rule, 0);
        cf = new F1P(fun, tparam);
    }
    setCommonRule(title, rule);
}
