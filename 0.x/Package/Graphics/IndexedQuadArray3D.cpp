#include "IndexedQuadArray3D.h"

IndexedQuadArray3D::IndexedQuadArray3D() {
    verts.clear();
    quads.clear();
    meshs.clear();
}

IndexedQuadArray3D::~IndexedQuadArray3D() {
    for (unsigned int i = 0; i < verts.size(); i++) {
        delete verts[i];
    }
    verts.clear();
    for (unsigned int i = 0; i < quads.size(); i++) {
        delete quads[i];
    }
    quads.clear();
    for (unsigned int i = 0; i < meshs.size(); i++) {
        delete meshs[i];
    }
    meshs.clear();
}