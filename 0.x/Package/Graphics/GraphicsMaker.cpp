#include "GraphicsMaker.h"
#include <iostream>

using namespace std;

GraphicsMaker::GraphicsMaker() {
    optlist.clear();
}

void GraphicsMaker::readOptions(var opts) {
    int opnum = Size(opts);
    for (int i = 0; i < opnum; i++) {
        var op = At(opts, i);
        if (ExQ(op, TAG(Rule))) {
            var rulebody = Body(op);
            setRule(At(rulebody, 0), At(rulebody, 1));
        } else {
            setRule(Head(op), Body(op));
        }
    }
}

var GraphicsMaker::makeRange(double xmin, double xmax, double ymin, double ymax) {
    var xrange = Vec(Flt(xmin), Flt(xmax));
    var yrange = Vec(Flt(ymin), Flt(ymax));
    var range = Vec(xrange, yrange);
    return Ex(Sym(L"PlotRange"), range);
}

var GraphicsMaker::makeRange(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) {
    var xrange = Vec(Flt(xmin), Flt(xmax));
    var yrange = Vec(Flt(ymin), Flt(ymax));
    var zrange = Vec(Flt(zmin), Flt(zmax));
    var range = Vec(xrange, yrange, zrange);
    return Ex(Sym(L"PlotRange"), range);
}

void GraphicsMaker::readVerts2d(var expr, int* num, Point2d** vert) {
    *num = Size(expr);
    *vert = new Point2d[*num];
    for (int i = 0; i < *num; i++) {
        readPoint2d(At(expr, i), *vert + i);
    }
}

void GraphicsMaker::readPoint2d(var expr, Point2d* vert) {
    vert->x = N::D(At(expr, 0));
    vert->y = N::D(Evalf(At(expr, 1)));
}

var GraphicsMaker::makeColor255(Color* color) {
    var cvar = Ex(Sym(L"Color255"), Vec(Int((long)color->r), Int((long)color->g), Int((long)color->b)));
    return cvar;
}

var GraphicsMaker::makeColor255Option(Color* color) {
    var opt = Ex(Sym(L"Options"), Vec(makeColor255(color)));
    return opt;
}

var GraphicsMaker::makeHueOption(Hue* hue) {
    //
    var cvar = Ex(Sym(L"Hue"), Vec(Flt(hue->h), Flt(hue->s), Flt(hue->b)));
    var opt = Ex(TAG(Options), Vec(cvar));
    return opt;
}

Hue* GraphicsMaker::getLightHue(int i) {
    Hue* hue = new Hue(double((HUE_HUEBASIC + HUE_HUEADD*i) % 100) / 100, HUE_SLIGHT, HUE_BLIGHT);
    return hue;
}

Hue* GraphicsMaker::getHue(int i) {
    Hue* hue = new Hue(double((HUE_HUEBASIC + HUE_HUEADD*i) % 100) / 100, HUE_SBASIC, HUE_BBASIC);
    return hue;
}

var GraphicsMaker::makeLine(Curve* cur, Color* color) {
    var l = Vec(cur->lineStrips.size() + 1);
    for (unsigned int i = 0; i < cur->lineStrips.size(); i++) {
        var v = Vec(cur->lineStrips[i]->vert.size());
        for (unsigned int j = 0; j < cur->lineStrips[i]->vert.size(); j++) {
            At(v, j) = Vec(Flt(cur->lineStrips[i]->vert[j]->x), Flt(cur->lineStrips[i]->vert[j]->y));
        }
        At(l, i) = v;
    }
    var opt = makeColor255Option(color);
    At(l, cur->lineStrips.size()) = opt;
    var line = Ex(Sym(L"Line"), l);
    return line;
}

var GraphicsMaker::makeLine(Curve3D* cur, Color* color) {
    var v = Vec(cur->vert.size());
    for (size_t i = 0; i < cur->vert.size(); ++i) {
        At(v, i) = Vec(Flt(cur->vert[i]->x), Flt(cur->vert[i]->y), Flt(cur->vert[i]->z));
    }
    var opt = makeColor255Option(color);
    var line = Ex(Sym(L"Line"), Vec(v, opt));
    return line;
}

var GraphicsMaker::makeLine(Curve* cur, Hue* hue) {
    var l = Vec(cur->lineStrips.size() + 1);
    for (unsigned int i = 0; i < cur->lineStrips.size(); i++) {
        var v = Vec(cur->lineStrips[i]->vert.size());
        for (unsigned int j = 0; j < cur->lineStrips[i]->vert.size(); j++) {
            At(v, j) = Vec(Flt(cur->lineStrips[i]->vert[j]->x), Flt(cur->lineStrips[i]->vert[j]->y));
        }
        At(l, i) = v;
    }
    var opt = makeHueOption(hue);
    At(l, cur->lineStrips.size()) = opt;
    var line = Ex(Sym(L"Line"), l);
    return line;
}

var GraphicsMaker::makeLine(Curve3D* cur, Hue* hue) {
    var v = Vec(cur->vert.size());
    for (size_t i = 0; i < cur->vert.size(); ++i) {
        At(v, i) = Vec(Flt(cur->vert[i]->x), Flt(cur->vert[i]->y), Flt(cur->vert[i]->z));
    }
    var opt = makeHueOption(hue);
    var line = Ex(Sym(L"Line"), Vec(v, opt));
    return line;
}

var GraphicsMaker::makeColorfulLines(Curve* cur) {
    var lines = Vec(cur->lineStrips.size());
    for (unsigned int i = 0; i < cur->lineStrips.size(); i++) {
        var v = Vec(cur->lineStrips[i]->vert.size());
        var c = Vec(cur->lineStrips[i]->vert.size());
        for (unsigned int j = 0; j < cur->lineStrips[i]->vert.size(); j++) {
            At(v, j) = Vec(Flt(cur->lineStrips[i]->vert[j]->x), Flt(cur->lineStrips[i]->vert[j]->y));
            At(c, j) = makeColor255(cur->lineStrips[i]->color[j]);
        }
        At(lines, i) = Ex(Sym(L"ColorfulLine"), Vec(v, c));
    }
    return lines;
}

void GraphicsMaker::makeDirectiveOfLines(Curve** curs, int curnum, var* out_verts, var* out_lines) {
    int* index_add = new int[curnum];
    int* curvertnum = new int[curnum];
    int vertnum = 0;
    for (int i = 0; i < curnum; i++) {
        curvertnum[i] = 0;
        for (unsigned int j = 0; j < curs[i]->lineStrips.size(); j++)
            curvertnum[i] += curs[i]->lineStrips[j]->vert.size();
    }
    index_add[0] = 0;
    vertnum = curvertnum[0];
    for (int i = 1; i < curnum; i++) {
        index_add[i] = index_add[i-1] + curvertnum[i-1];
        vertnum += curvertnum[i];
    }
    *out_verts = Vec(vertnum);
    for (int i = 0; i < curnum; i++) {
        int add = 0;
        out_lines[i] = Vec(curs[i]->lineStrips.size());
        for (unsigned int j = 0; j < curs[i]->lineStrips.size(); j++) {
            for (unsigned int k = 0; k < curs[i]->lineStrips[j]->vert.size(); k++) {
                At(*out_verts, index_add[i] + add + k) = Vec(Flt(curs[i]->lineStrips[j]->vert[k]->x), Flt(curs[i]->lineStrips[j]->vert[k]->y));
            }
            var v = Vec(curs[i]->lineStrips[j]->vert.size());
            for (unsigned int k = 0; k < curs[i]->lineStrips[j]->vert.size(); k++) {
                At(v, k) = Int((long)index_add[i] + add + k);
            }
            At(out_lines[i], j) = v;
            add += curs[i]->lineStrips[j]->vert.size();
        }
    }
}

var GraphicsMaker::makeIndexedPolygon(IndexedQuadArray* poly, int color_id) {
    var v = Vec(poly->verts.size());
    for (size_t i = 0; i < poly->verts.size(); ++i) {
        At(v, i) = Vec(Flt(poly->verts[i]->x), Flt(poly->verts[i]->y));
    }
    //quad list
    var quads = Vec(poly->quads.size());
    for (size_t i = 0; i < poly->quads.size(); i++) {
        var indice = Vec(4);
        At(indice, 0) = Int((long)poly->quads[i]->p11);
        At(indice, 1) = Int((long)poly->quads[i]->p12);
        At(indice, 2) = Int((long)poly->quads[i]->p22);
        At(indice, 3) = Int((long)poly->quads[i]->p21);
        At(quads, i) = indice;
    }
    Hue* hue1 = getLightHue(color_id);
    var qopt = makeHueOption(hue1);
    var equad = Ex(Sym(L"QuadArray"), Vec(quads, qopt));
    //edge
    var line = Vec(poly->edge.size());
    for (size_t i = 0; i < poly->edge.size(); i++) {
        At(line, i) = Int((long)poly->edge[i]);
    }
    Hue* hue2 = getHue(color_id);
    var lopt = makeHueOption(hue2);
    var eline = Ex(Sym(L"Line"), Vec(line, lopt));
    //graphics complex
    var directive = Ex(Sym(L"Directive"), Vec(equad, eline));
    var complex = Ex(Sym(L"GraphicsComplex"), Vec(v, directive));
    return complex;
}


var GraphicsMaker::addVec(var vec1, var vec2) {
    var v = Vec(Size(vec1) + Size(vec2));
    for (size_t i = 0; i < Size(vec1); i++)
        At(v, i) = At(vec1, i);
    for (size_t i = 0; i < Size(vec2); i++)
        At(v, Size(vec1) + i) = At(vec2, i);
    return v;
}

var GraphicsMaker::addVecReversed(var vec1, var vec2) {
    var v = Vec(Size(vec1) + Size(vec2));
    for (size_t i = 0; i < Size(vec1); i++)
        At(v, i) = At(vec1, i);
    for (size_t i = 0; i < Size(vec2); i++)
        At(v, Size(vec1) + i) = At(vec2, Size(vec2) - 1 - i);
    return v;
}
