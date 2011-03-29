#include "Curve3D.h"

Curve3D::~Curve3D() {
    for (unsigned int i = 0; i < vert.size(); i++) {
        delete vert[i];
    }
    vert.clear();
}