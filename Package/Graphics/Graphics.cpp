#include <mU/Kernel.h>
#include "Plot.h"
#include "Plot3D.h"
#include "ListPlot.h"
#include "ParametricCurvePlot.h"
#include "ParametricRegionPlot.h"
#include "PolarPlot.h"
#include "ParametricCurvePlot3D.h"
#include "ParametricSurfacePlot3D.h"
#include "ContourPlot3D.h"
#include "ContourPlot.h"

using namespace std;

namespace mU {
namespace Graphics {

Wrap(Plot) {
    Plot* plot = new Plot(x);
    var r = plot->exportGraphics();
    delete plot;
    return r;
}

Wrap(ListPlot) {
    ListPlot* plot = new ListPlot(x);
    plot->exportGraphics();
    var r = plot->exportGraphics();
    
    delete plot;
    return r;
}

Wrap(Plot3D) {
    Plot3D* plot = new Plot3D(x);
    var r = plot->exportGraphics();
    
    delete plot;
    return r;
}

Wrap(ParametricCurvePlot) {
    ParametricCurvePlot* plot = new ParametricCurvePlot(x);
    var r = plot->exportGraphics();
    
    delete plot;
    return r;
}

Wrap(ParametricRegionPlot) {
    ParametricRegionPlot* plot = new ParametricRegionPlot(x);
    var r = plot->exportGraphics();
    
    delete plot;
    return r;
}

Wrap(PolarPlot) {
    PolarPlot* plot = new PolarPlot(x);
    var r = plot->exportGraphics();
    
    delete plot;
    return r;
}

Wrap(ParametricCurvePlot3D) {
    ParametricCurvePlot3D* plot = new ParametricCurvePlot3D(x);
    var r = plot->exportGraphics();
    
    delete plot;
    return r;
}

Wrap(ParametricSurfacePlot3D) {
    ParametricSurfacePlot3D* plot = new ParametricSurfacePlot3D(x);
    var r = plot->exportGraphics();
    
    delete plot;
    return r;
}

Wrap(ContourPlot3D) {
    ContourPlot3D* plot = new ContourPlot3D(x);
    var r = plot->exportGraphics();
    
    delete plot;
    return r;
}

Wrap(ContourPlot) {
    ContourPlot* plot = new ContourPlot(x);
    var r = plot->exportGraphics();
    
    delete plot;
    return r;
}

void Initialize() {
    static bool Initialized = false;
    if (Initialized) return;
#define T(x) var TAG(x) = Sym(L###x,System);Externals[TAG(x)] = WRAP(x);
    T(Plot)
    T(Plot3D)
    T(ListPlot)
    T(ParametricCurvePlot)
    T(ParametricRegionPlot)
    T(PolarPlot)
    T(ParametricCurvePlot3D)
    T(ParametricSurfacePlot3D)
    T(ContourPlot3D)
    T(ContourPlot)
#undef T
#define T(x) var TAG(x) = Sym(L###x,System);Functionals[TAG(x)] = WRAP(x);
#undef T
#define T(x,y) Attributes[TAG(x)].insert(y);
    T(Plot, HoldAll)
    T(Plot3D, HoldAll)
    T(ListPlot, HoldAll)
    T(ParametricCurvePlot, HoldAll)
    T(ParametricRegionPlot, HoldAll)
    T(PolarPlot, HoldAll)
    T(ParametricCurvePlot3D, HoldAll)
    T(ParametricSurfacePlot3D, HoldAll)
    T(ContourPlot3D, HoldAll)
    T(ContourPlot, HoldAll)
#undef T
    Initialized = true;
}
}
}
DLLMAIN(mU::Graphics::Initialize)
