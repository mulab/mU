#include "ContourPlot3D.h"

using namespace std;

ContourPlot3D::ContourPlot3D(var cmd) {
    spacetype = NORMAL_SPACE;
    var expr = At(cmd, 0);
    var xrange = At(cmd, 1);
    var xparam = At(xrange, 0);
    xmin = N::D(Evalf(At(xrange, 1)));
    xmax = N::D(Evalf(At(xrange, 2)));
    var yrange = At(cmd, 2);
    var yparam = At(yrange, 0);
    ymin = N::D(Evalf(At(yrange, 1)));
    ymax = N::D(Evalf(At(yrange, 2)));
    var zrange = At(cmd, 3);
    var zparam = At(zrange, 0);
    zmin = N::D(Evalf(At(zrange, 1)));
    zmax = N::D(Evalf(At(zrange, 2)));
    int len = Size(cmd);
    if (ExQ(At(cmd, len - 1), TAG(Options))) {
        var opts = Body((At(cmd, len - 1)));
        readOptions(opts);
    }
    F3P *f = new F3P(expr, xparam, yparam, zparam);
    sur = new ImplicitSurface(f, xmin, xmax, ymin, ymax, zmin, zmax);
    delete f;
}

ContourPlot3D::~ContourPlot3D() {
    delete sur;
}

//make a meshed surface
var ContourPlot3D::exportGraphics() {
    //vert list
    var v = Vec(sur->m_vertices.size());
    for (size_t i = 0; i < sur->m_vertices.size(); ++i) {
        At(v, i) = Vec(Flt(sur->m_vertices[i]->position.x), Flt(sur->m_vertices[i]->position.y), Flt(sur->m_vertices[i]->position.z));
    }
    sur->m_triangleindices;
    //polygon list
    var tri = Vec(sur->m_triangleindices.size());
    for (size_t i = 0; i < sur->m_triangleindices.size(); i++) {
        At(tri, i) = Int((long)sur->m_triangleindices[i]);
    }
    var surfflag = Ex(Sym(L"LightedSurface"), Vec());
    //var surfflag = Ex(Sym(L"Surface"),Vec());
    var suroptions = Ex(TAG(Options), Vec(surfflag));
    var etriangle = Ex(Sym(L"TriangleArray"), Vec(tri, suroptions));
    //graphics complex
    var directive = Ex(Sym(L"Directive"), Vec(etriangle));
    var complex = Ex(Sym(L"GraphicsComplex"), Vec(v, directive));
    var shape = Ex(Sym(L"Shape"), Vec(complex));
    var options = makeGlobalOptions();
    var graphics = Ex(TAG(Graphics3D), Vec(shape, options));
    return graphics;
}

void ContourPlot3D::setRule(var title, var rule) {
    setCommonRule(title, rule);
}
