#include "Plot.h"
using namespace std;

Plot::Plot(var cmd) {
    filling = FILL_NULL;
    rangeSet = false;
    colorFunctionSet = false;
    var expr = At(cmd, 0);
    var xrange = At(cmd, 1);
    xparam = At(xrange, 0);
    xmin = N::D(Evalf(At(xrange, 1)));
    xmax = N::D(Evalf(At(xrange, 2)));
    int len = Size(cmd);
    if (ExQ(At(cmd, len - 1), TAG(Options))) {
        var opts = Body((At(cmd, len - 1)));
        readOptions(opts);
    }
    if (!rangeSet) {
        if (VecQ(expr)) {
            curnum = Size(expr);
            curs = new Curve*[curnum];
            for (int i = 0; i < curnum; i++) {
                F1P *f2d = new F1P(At(expr, i), xparam);
                if (!colorFunctionSet)
                    curs[i] = (Curve*)(new CommonCurve(f2d, xmin, xmax));
                else
                    curs[i] = (Curve*)(new CommonCurve(f2d, xmin, xmax, cf));
                delete f2d;
            }
        } else {
            curnum = 1;
            curs = new Curve*[1];
            F1P *f2d = new F1P(expr, xparam);
            if (!colorFunctionSet)
                curs[0] = (Curve*)(new CommonCurve(f2d, xmin, xmax));
            else
                curs[0] = (Curve*)(new CommonCurve(f2d, xmin, xmax, cf));
            delete f2d;
        }
        ymin = curs[0]->ymin;
        ymax = curs[0]->ymax;
        for (int i = 1; i < curnum; i++) {
            if (curs[i]->ymin < ymin)
                ymin = curs[i]->ymin;
            if (curs[i]->ymax > ymax)
                ymax = curs[i]->ymax;
        }
    } else if (rangeSet == 1) { //range set
        if (VecQ(expr)) {
            curnum = Size(expr);
            curs = new Curve*[curnum];
            for (int i = 0; i < curnum; i++) {
                F1P *f2d = new F1P(At(expr, i), xparam);
                if (!colorFunctionSet)
                    curs[i] = (Curve*)(new CommonCurveWithPlotRange(f2d, xmin, xmax, ymin, ymax));
                else
                    curs[i] = (Curve*)(new CommonCurveWithPlotRange(f2d, xmin, xmax, ymin, ymax, cf));
                delete f2d;
            }
        } else {
            curnum = 1;
            curs = new Curve*[1];
            F1P *f2d = new F1P(expr, xparam);
            if (!colorFunctionSet)
                curs[0] = (Curve*)(new CommonCurveWithPlotRange(f2d, xmin, xmax, ymin, ymax));
            else
                curs[0] = (Curve*)(new CommonCurveWithPlotRange(f2d, xmin, xmax, ymin, ymax, cf));
            delete f2d;
        }
    } else if (rangeSet == 2) { //automatic plot range
        if (VecQ(expr)) {
            curnum = Size(expr);
            curs = new Curve*[curnum];
            for (int i = 0; i < curnum; i++) {
                F1P *f2d = new F1P(At(expr, i), xparam);
                if (!colorFunctionSet)
                    curs[i] = (Curve*)(new CommonCurveWithPlotRange(f2d, xmin, xmax));
                else
                    curs[i] = (Curve*)(new CommonCurveWithPlotRange(f2d, xmin, xmax, cf));
                delete f2d;
            }
        } else {
            curnum = 1;
            curs = new Curve*[1];
            F1P *f2d = new F1P(expr, xparam);
            if (!colorFunctionSet)
                curs[0] = (Curve*)(new CommonCurveWithPlotRange(f2d, xmin, xmax));
            else
                curs[0] = (Curve*)(new CommonCurveWithPlotRange(f2d, xmin, xmax, cf));
            delete f2d;
        }
        if (colorFunctionSet)
            delete cf;
        ymin = curs[0]->ymin;
        ymax = curs[0]->ymax;
        for (int i = 1; i < curnum; i++) {
            if (curs[i]->ymin < ymin)
                ymin = curs[i]->ymin;
            if (curs[i]->ymax > ymax)
                ymax = curs[i]->ymax;
        }
    }
}

Plot::~Plot() {
    for (int i = 0; i < curnum; i++) {
        delete curs[i];
    }
    delete[] curs;
}

var Plot::exportGraphics() {
    if (filling == FILL_NULL) {
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
    } else if (filling == FILL_TO_CURVE) {
        var verts;
        var* line_indices = new var[curnum];
        makeDirectiveOfLines(curs, curnum, &verts, line_indices);
        var shapes = Vec(curnum + 1);
        for (int i = 0; i < curnum; i++) {
            var l = Vec(Size(line_indices[i]) + 1);
            for (size_t j = 0; j < Size(line_indices[i]); j++)
                At(l, j) = At(line_indices[i], j);
            var opt = makeHueOption(getHue(i));
            At(l, Size(line_indices[i])) = opt;
            At(shapes, i) = Ex(Sym(L"Line"), l);
        }
        var poly = addVecReversed(At(line_indices[fillcur1], 0), At(line_indices[fillcur2], 0)); //fix it
        var opt = makeHueOption(getLightHue(0));
        At(shapes, curnum) = Ex(Sym(L"Polygon"), Vec(poly, opt));
        delete[] line_indices;
        var directive = Ex(Sym(L"Directive"), shapes);
        var complex = Ex(Sym(L"GraphicsComplex"), Vec(verts, directive));
        var shape = Ex(Sym(L"Shape"), Vec(complex));
        var opts = makeGlobalOptions();
        var graphics = makeGraphics(shape, opts);
        return graphics;
    } else if (filling == FILL_TO_AXIS) {
        var verts;
        var* line_indices = new var[curnum+1];
        ArrayList<Point2d*> vaxis;
        vaxis.clear();
        vaxis.add(new Point2d(xmin, 0));
        vaxis.add(new Point2d(xmax, 0));
        Curve* axis = new Curve(vaxis);
        Curve** newcurves = new Curve*[curnum+1];
        for (int i = 0; i < curnum; i++)
            newcurves[i] = curs[i];
        newcurves[curnum] = axis;
        makeDirectiveOfLines(newcurves, curnum + 1, &verts, line_indices);
        var shapes = Vec(curnum + 1);
        for (int i = 0; i < curnum; i++) {
            var l = Vec(Size(line_indices[i]) + 1);
            for (size_t j = 0; j < Size(line_indices[i]); j++)
                At(l, j) = At(line_indices[i], j);
            var opt = makeHueOption(getHue(i));
            At(l, Size(line_indices[i])) = opt;
            At(shapes, i) = Ex(Sym(L"Line"), l);
        }
        var poly = addVecReversed(At(line_indices[fillcur1], 0), At(line_indices[curnum], 0)); //fix it
        var opt = makeHueOption(getLightHue(0));
        At(shapes, curnum) = Ex(Sym(L"Polygon"), Vec(poly, opt));
        delete[] line_indices;
        delete vaxis[0];
        delete vaxis[1];
        delete axis;
        delete newcurves;
        var directive = Ex(Sym(L"Directive"), shapes);
        var complex = Ex(Sym(L"GraphicsComplex"), Vec(verts, directive));
        var shape = Ex(Sym(L"Shape"), Vec(complex));
        var opts = makeGlobalOptions();
        var graphics = makeGraphics(shape, opts);
        return graphics;
    }
    return NULL;
}

void Plot::setRule(var title, var rule) {
    if (title == Sym(L"FillToCurve")) {
        filling = FILL_TO_CURVE;
        fillcur1 = Z::SI(At(rule, 0));
        fillcur2 = Z::SI(At(rule, 1));
    } else if (title == Sym(L"FillToAxis")) {
        filling = FILL_TO_AXIS;
        fillcur1 = Z::SI(At(rule, 0));
    } else if (title == Sym(L"PlotRange")) { //plot range set or auto plot range
        if (Size(rule) == 2) {
            rangeSet = 1;
            ymin = N::D(At(rule, 0));
            ymax = N::D(Evalf(At(rule, 1)));
        } else {
            rangeSet = 2; //auto range
        }
    } else if (title == Sym(L"ColorFunction")) { //has color function
        colorFunctionSet = true;
        var fun = At(rule, 0);
        cf = new F1P(fun, xparam);
    }
    //to do color function in hue(f1,f2,f3) or color(f1,f2,f3)
    setCommonRule(title, rule);
}
