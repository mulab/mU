#include "GraphicsMaker2D.h"

GraphicsMaker2D::GraphicsMaker2D() {
    axeX = true;
    axeY = true;
}

var GraphicsMaker2D::makeGlobalOptions() {
    var range = GraphicsMaker::makeRange(xmin, xmax, ymin, ymax);
    optlist.add(range);
    var opts = Vec(optlist.size());
    for (unsigned int i = 0; i < optlist.size(); i++) {
        At(opts, i) = optlist[i];
    }
    var options = Ex(TAG(Options), opts);
    return options;
}

var GraphicsMaker2D::makeGraphics(var shape, var opts) {
    var graphics = Ex(TAG(Graphics2D), Vec(shape, opts));
    return graphics;
}

var GraphicsMaker2D::makeGraphics(var shape) {
    var opts = makeGlobalOptions();
    return makeGraphics(shape, opts);
}

void GraphicsMaker2D::setCommonRule(var title, var rule) {
    if (title == Sym(L"Axes")) {
        var op = Ex(title, rule);
        optlist.add(op);
    }
    if (title == Sym(L"AxesLabel")) {
        var op = Ex(title, rule);
        optlist.add(op);
    }
    if (title == Sym(L"GraphLabel")) {
        var op = Ex(title, rule);
        optlist.add(op);
    }
}


