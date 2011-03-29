#include "GraphicsMaker3D.h"

GraphicsMaker3D::GraphicsMaker3D() {
    spacetype = NORMAL_SPACE;
}

var GraphicsMaker3D::makeGlobalOptions() {
    ArrayList<var> optlist;
    optlist.clear();
    var range = GraphicsMaker::makeRange(xmin, xmax, ymin, ymax, zmin, zmax);
    optlist.add(range);
    var spaceflag = NULL;
    if (spacetype == FLAT_SPACE) {
        spaceflag = Ex(Sym(L"FlatSpace"), Vec());
    } else if (spacetype == NORMAL_SPACE) {
        spaceflag = Ex(Sym(L"NormalSpace"), Vec());
    }
    if (spaceflag != NULL)
        optlist.add(spaceflag);
    var opts = Vec(optlist.size());
    for (unsigned int i = 0; i < optlist.size(); i++) {
        At(opts, i) = optlist[i];
    }
    var options = Ex(TAG(Options), opts);
    return options;
}

var GraphicsMaker3D::makeGraphics(var shape, var opts) {
    var graphics = Ex(TAG(Graphics3D), Vec(shape, opts));
    return graphics;
}

var GraphicsMaker3D::makeGraphics(var shape) {
    var opts = makeGlobalOptions();
    return makeGraphics(shape, opts);
}

void GraphicsMaker3D::setCommonRule(var title, var rule) {
}
