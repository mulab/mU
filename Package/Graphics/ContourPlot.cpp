#include "ContourPlot.h"

using namespace std;

ContourPlot::ContourPlot(var cmd) {
    var expr = At(cmd, 0);
    var xrange = At(cmd, 1);
    var xparam = At(xrange, 0);
    xmin = N::D(Evalf(At(xrange, 1)));
    xmax = N::D(Evalf(At(xrange, 2)));
    var yrange = At(cmd, 2);
    var yparam = At(yrange, 0);
    ymin = N::D(Evalf(At(yrange, 1)));
    ymax = N::D(Evalf(At(yrange, 2)));
    int len = Size(cmd);
    if (ExQ(At(cmd, len - 1), TAG(Options))) {
        var opts = Body((At(cmd, len - 1)));
        readOptions(opts);
    }
    F2P *f = new F2P(expr, xparam, yparam);
    cur = new ImplicitCurve(f, xmin, xmax, ymin, ymax);
    delete f;
}

ContourPlot::~ContourPlot() {
    delete cur;
}

var ContourPlot::exportGraphics() {
    //vert list
    var v = Vec(cur->m_vertices.size());
    for (size_t i = 0; i < cur->m_vertices.size(); ++i) {
        At(v, i) = Vec(Flt(cur->m_vertices[i]->position.x), Flt(cur->m_vertices[i]->position.y));
    }
    var lines = Vec(cur->m_line.size() / 2 + 1);
    for (size_t i = 0; i < cur->m_line.size() / 2; i++) {
        At(lines, i) = Vec(Int((long)cur->m_line[2*i]), Int((long)cur->m_line[2*i+1]));
    }
    Hue* hue = getHue(0);
    var opt = makeHueOption(hue);
    At(lines, cur->m_line.size() / 2) = opt;
    var eline = Ex(Sym(L"Line"), lines);
    //graphics complex
    var directive = Ex(Sym(L"Directive"), Vec(eline));
    var complex = Ex(Sym(L"GraphicsComplex"), Vec(v, directive));
    var shape = Ex(Sym(L"Shape"), Vec(complex));
    var opts = makeGlobalOptions();
    var graphics = makeGraphics(shape, opts);
    return graphics;
}

void ContourPlot::setRule(var title, var rule) {
    setCommonRule(title, rule);
}
