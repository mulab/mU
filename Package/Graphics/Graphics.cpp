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

    DEF_TAG_SYM_WRAPPED_CPROC(Plot)
    DEF_TAG_SYM_WRAPPED_CPROC(Plot3D)
    DEF_TAG_SYM_WRAPPED_CPROC(ListPlot)
    DEF_TAG_SYM_WRAPPED_CPROC(ParametricCurvePlot)
    DEF_TAG_SYM_WRAPPED_CPROC(ParametricRegionPlot)
    DEF_TAG_SYM_WRAPPED_CPROC(PolarPlot)
    DEF_TAG_SYM_WRAPPED_CPROC(ParametricCurvePlot3D)
    DEF_TAG_SYM_WRAPPED_CPROC(ParametricSurfacePlot3D)
    DEF_TAG_SYM_WRAPPED_CPROC(ContourPlot3D)
    DEF_TAG_SYM_WRAPPED_CPROC(ContourPlot)

    SET_ATTR(Plot, HoldAll)
    SET_ATTR(Plot3D, HoldAll)
    SET_ATTR(ListPlot, HoldAll)
    SET_ATTR(ParametricCurvePlot, HoldAll)
    SET_ATTR(ParametricRegionPlot, HoldAll)
    SET_ATTR(PolarPlot, HoldAll)
    SET_ATTR(ParametricCurvePlot3D, HoldAll)
    SET_ATTR(ParametricSurfacePlot3D, HoldAll)
    SET_ATTR(ContourPlot3D, HoldAll)
    SET_ATTR(ContourPlot, HoldAll)

    Initialized = true;
}
}
}
DLLMAIN(mU::Graphics::Initialize)
