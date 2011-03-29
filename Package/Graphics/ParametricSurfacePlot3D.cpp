#include "ParametricSurfacePlot3D.h"

ParametricSurfacePlot3D::ParametricSurfacePlot3D(var cmd) {
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
        polys = new IndexedQuadArray3D*[polynum];
        for (int i = 0; i < polynum; i++) {
            var funs = At(expr, i);
            F2P *fx = new F2P(At(funs, 0), uparam, vparam);
            F2P *fy = new F2P(At(funs, 1), uparam, vparam);
            F2P *fz = new F2P(At(funs, 2), uparam, vparam);
            polys[i] = (IndexedQuadArray3D*)(new ParametricSurface(fx, fy, fz, umin, umax, vmin, vmax));
            delete fx;
            delete fy;
            delete fz;
        }
    } else {
        polynum = 1;
        polys = new IndexedQuadArray3D*[1];
        var funs = expr;
        F2P *fx = new F2P(At(funs, 0), uparam, vparam);
        F2P *fy = new F2P(At(funs, 1), uparam, vparam);
        F2P *fz = new F2P(At(funs, 2), uparam, vparam);
        polys[0] = (IndexedQuadArray3D*)(new ParametricSurface(fx, fy, fz, umin, umax, vmin, vmax));
        delete fx;
        delete fy;
        delete fz;
    }
    xmin = polys[0]->xmin;
    xmax = polys[0]->xmax;
    ymin = polys[0]->ymin;
    ymax = polys[0]->ymax;
    zmin = polys[0]->zmin;
    zmax = polys[0]->zmax;
    for (int i = 1; i < polynum; i++) {
        if (polys[i]->xmin < xmin)
            xmin = polys[i]->xmin;
        if (polys[i]->xmax > xmax)
            xmax = polys[i]->xmax;
        if (polys[i]->ymin < ymin)
            ymin = polys[i]->ymin;
        if (polys[i]->ymax > ymax)
            ymax = polys[i]->ymax;
        if (polys[i]->zmin < zmin)
            zmin = polys[i]->zmin;
        if (polys[i]->zmax > zmax)
            zmax = polys[i]->zmax;
    }
}

ParametricSurfacePlot3D::~ParametricSurfacePlot3D() {
    for (int i = 0; i < polynum; i++) {
        delete polys[i];
    }
    delete polys;
}

var ParametricSurfacePlot3D::exportGraphics() {
    var shapelist = Vec(polynum);
    for (int k = 0; k < polynum; k++) {
        IndexedQuadArray3D* poly = polys[k];
        var v = Vec(poly->verts.size());
        for (size_t i = 0; i < poly->verts.size(); ++i) {
            At(v, i) = Vec(Flt(poly->verts[i]->x), Flt(poly->verts[i]->y), Flt(poly->verts[i]->z));
        }
        //quad list
        var quads = Vec(poly->quads.size() * 4);
        for (size_t i = 0; i < poly->quads.size(); i++) {
            At(quads, 4*i) = Int((long)poly->quads[i]->p11);
            At(quads, 4*i + 1) = Int((long)poly->quads[i]->p12);
            At(quads, 4*i + 2) = Int((long)poly->quads[i]->p22);
            At(quads, 4*i + 3) = Int((long)poly->quads[i]->p21);
        }
        var surfflag = Ex(Sym(L"Surface"), Vec());
        var qopt = Ex(TAG(Options), Vec(surfflag));
        var equad = Ex(Sym(L"QuadArray"), Vec(quads, qopt));
        //mesh
        var lines = Vec(poly->meshs.size() + 1);
        for (size_t i = 0; i < poly->meshs.size(); i++) {
            var line = Vec(poly->meshs[i]->p.size());
            for (size_t j = 0; j < poly->meshs[i]->p.size(); j++)
                At(line, j) = Int((long)poly->meshs[i]->p[j]);
            At(lines, i) = line;
        }
        var color = Ex(Sym(L"Color3f"), Vec(Flt(0.0), Flt(0.0), Flt(0.0)));
        var meshflag = Ex(Sym(L"Mesh"), Vec());
        var lopts = Ex(TAG(Options), Vec(meshflag, color));
        At(lines, poly->meshs.size()) = lopts;
        var eline = Ex(Sym(L"Line"), lines);
        //graphics complex
        var directive = Ex(Sym(L"Directive"), Vec(equad, eline));
        var complex = Ex(Sym(L"GraphicsComplex"), Vec(v, directive));
        At(shapelist, k) = complex;
    }
    var shape = Ex(Sym(L"Shape"), shapelist);
    var opts = makeGlobalOptions();
    var graphics = makeGraphics(shape, opts);
    return graphics;
}

void ParametricSurfacePlot3D::setRule(var title, var rule) {
    setCommonRule(title, rule);
}
