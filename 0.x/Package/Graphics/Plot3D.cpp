#include "Plot3D.h"

using namespace std;

Plot3D::Plot3D(var cmd) {
    spacetype = FLAT_SPACE;
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
    sur = new Surface*[1];
    surnum = 1;
    F2P *f3d = new F2P(expr, xparam, yparam);
    sur[0] = new Surface(f3d, xmin, xmax, ymin, ymax);
    delete f3d;
    zmin = sur[0]->zmin;
    zmax = sur[0]->zmax;
    for (int i = 1; i < surnum; i++) {
        if (sur[i]->zmin < zmin)
            zmin = sur[i]->zmin;
        if (sur[i]->zmax > zmax)
            zmax = sur[i]->zmax;
    }
}

Plot3D::~Plot3D() {
    for (int i = 0; i < surnum; i++) {
        delete sur[i];
    }
    delete[] sur;
}

//make a meshed surface
var Plot3D::exportGraphics() {
    //vert list
    var v = Vec(sur[0]->vert.size());
    for (size_t i = 0; i < sur[0]->vert.size(); ++i) {
        At(v, i) = Vec(Flt(sur[0]->vert[i]->x), Flt(sur[0]->vert[i]->y), Flt(sur[0]->vert[i]->z));
    }
    //polygon list
    var quad = Vec(sur[0]->sur.size() * 4);
    for (size_t i = 0; i < sur[0]->sur.size(); i++) {
        At(quad, 4*i) = Int((long)sur[0]->sur[i]->p11);
        At(quad, 4*i + 1) = Int((long)sur[0]->sur[i]->p12);
        At(quad, 4*i + 2) = Int((long)sur[0]->sur[i]->p22);
        At(quad, 4*i + 3) = Int((long)sur[0]->sur[i]->p21);
    }
    var surfflag = Ex(Sym(L"DoubleSurface"), Vec());
    var qoptions = Ex(TAG(Options), Vec(surfflag));
    var equad = Ex(Sym(L"QuadArray"), Vec(quad, qoptions));
    //mesh line list & options
    var linepak = Vec(sur[0]->ny + sur[0]->nx + 1);
    for (int i = 0; i < sur[0]->ny; i++) {
        var lv = Vec(sur[0]->xline[i]->p.size());
        for (size_t j = 0; j < sur[0]->xline[i]->p.size(); j++) {
            At(lv, j) = Int((long)sur[0]->xline[i]->p[j]);
        }
        At(linepak, i) = lv;
    }
    for (int i = 0; i < sur[0]->nx; i++) {
        var lv = Vec(sur[0]->yline[i]->p.size());
        for (size_t j = 0; j < sur[0]->yline[i]->p.size(); j++) {
            At(lv, j) = Int((long)sur[0]->yline[i]->p[j]);
        }
        At(linepak, sur[0]->ny + i) = lv;
    }
    var color = Ex(Sym(L"Color3f"), Vec(Flt(0.0), Flt(0.0), Flt(0.0)));
    var meshflag = Ex(Sym(L"Mesh"), Vec());
    var loptions = Ex(TAG(Options), Vec(meshflag, color));
    At(linepak, sur[0]->ny + sur[0]->nx) = loptions;
    var eline = Ex(Sym(L"Line"), linepak);
    //graphics complex
    var directive = Ex(Sym(L"Directive"), Vec(equad, eline));
    var complex = Ex(Sym(L"GraphicsComplex"), Vec(v, directive));
    var shape = Ex(Sym(L"Shape"), Vec(complex));
    var options = makeGlobalOptions();
    var graphics = Ex(TAG(Graphics3D), Vec(shape, options));
    return graphics;
}

void Plot3D::setRule(var title, var rule) {
    setCommonRule(title, rule);
}
