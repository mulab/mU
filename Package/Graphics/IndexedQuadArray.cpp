#include "IndexedQuadArray.h"

IndexedQuadArray::IndexedQuadArray() {
    verts.clear();
    quads.clear();
    edge.clear();
}

IndexedQuadArray::~IndexedQuadArray() {
    for (unsigned int i = 0; i < verts.size(); i++) {
        delete verts[i];
    }
    verts.clear();
    for (unsigned int i = 0; i < quads.size(); i++) {
        delete quads[i];
    }
    quads.clear();
}