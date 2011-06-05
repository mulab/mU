#include "ImplicitCurve.h"

ImplicitCurve::ImplicitCurve(F2P* f2p, double xmin, double xmax, double ymin, double ymax) {
    int n;
    TEST in, out;
    f = f2p;
    spx = xmax - xmin;
    spy = ymax - ymin;
    m_start.x = (xmax + xmin) / 2;
    m_start.y = (ymax + ymin) / 2;
    sizex = spx / CUBE_NUM_CURVE;
    sizey = spy / CUBE_NUM_CURVE;
    m_bounds = (CUBE_NUM_CURVE - 1) / 2;
    /* allocate hash tables and build cube polygon table: */
    m_centers = new CENTERLIST*[HASHSIZE*2];
    for (int i = 0; i < HASHSIZE*2; i++) {
        m_centers[i] = 0;
    }
    m_corners = new CORNERLIST*[HASHSIZE*2];
    for (int i = 0; i < HASHSIZE*2; i++) {
        m_corners[i] = 0;
    }
    m_edges = new EDGELIST*[HASHSIZE*2];
    for (int i = 0; i < HASHSIZE*2; i++) {
        m_edges[i] = 0;
    }
    m_cubes = new CUBES; /* list of 1 */
    m_cubes->cube.i = m_cubes->cube.j = 0;
    m_cubes->next = NULL;
    /* set corners of initial cube: */
    for (n = 0; n < 4; n++)
        m_cubes->cube.corners[n] = setcorner(BIT(n, 1), BIT(n, 0));
    setcenter(m_centers, 0, 0);
    while (m_cubes != NULL) { /* process active cubes till none left */
        CUBE c;
        CUBES *temp = m_cubes;
        c = m_cubes->cube;
        //
        dotriangle(&c, 0, 1, 2);
        dotriangle(&c, 1, 2, 3);
        /* pop current cube from stack */
        m_cubes = m_cubes->next;
        delete[] temp;
        /* test six face directions, maybe add to stack: */
        //
        testface(c.i - 1, c.j, &c, _L, LB, LT);
        testface(c.i + 1, c.j, &c, _R, RB_CURVE, RT_CURVE);
        testface(c.i, c.j - 1, &c, _B, LB, RB_CURVE);
        testface(c.i, c.j + 1, &c, _T, LT, RT_CURVE);
    }
}

ImplicitCurve::~ImplicitCurve() {
    free_all();
    for (unsigned int i = 0; i < m_vertices.size(); i++) {
        delete m_vertices[i];
    }
    m_vertices.clear();
    m_line.clear();
}

/* free_all: free allthe memory we¡¯ve allocated (except cubetable) */
void ImplicitCurve::free_all () {
    int index;
    CORNERLIST *l, *lnext;
    CENTERLIST *cl, *clnext;
    EDGELIST *edge, *edgenext;
    for (index = 0; index < HASHSIZE*2; index++) {
        for (l = m_corners[index]; l; l = lnext) {
            lnext = l->next;
            delete[] l; /* free CORNERLIST */
        }
        for (cl = m_centers[index]; cl; cl = clnext) {
            clnext = cl->next;
            delete[] cl; /* free CENTERLIST */
        }
        for (edge = m_edges[index]; edge; edge = edgenext) {
            edgenext = edge->next;
            delete[] edge; /* free EDGELIST */
        }
    }
    delete[] m_edges; /* free array of EDGELIST pointers */
    delete[] m_corners; /* free array of CORNERLIST pointers */
    delete[] m_centers; /* free array of CENTERLIST pointers */
}

/* testface: given cube at lattice (i, j, k), and four corners of face,
* if surface crosses face, compute other four corners of adjacent cube
* and add new cube to cube stack */
void ImplicitCurve::testface (int i, int j, CUBE *old, int face, int c1, int c2) {
    CUBE newc;
    CUBES *oldcubes = m_cubes;
    static int facebit[4] = {1, 1, 0, 0};
    int n, bit = facebit[face];
    if (abs(i) > m_bounds || abs(j) > m_bounds)
        return;
    if (setcenter(m_centers, i, j))
        return;
    /* create new cube: */
    newc.i = i;
    newc.j = j;
    for (n = 0; n < 4; n++) newc.corners[n] = NULL;
    newc.corners[FLIP(c1, bit)] = old->corners[c1];
    newc.corners[FLIP(c2, bit)] = old->corners[c2];
    for (n = 0; n < 4; n++)
        if (newc.corners[n] == NULL)
            newc.corners[n] = setcorner(i + BIT(n, 1), j + BIT(n, 0));
    /*add cube to top of stack: */
    m_cubes = new CUBES;
    m_cubes->cube = newc;
    m_cubes->next = oldcubes;
}

/* setcorner: return corner with the given lattice location set (and cache) its function value */
ImplicitCurve::CORNER* ImplicitCurve::setcorner (int i, int j) {
    /* for speed, do corner value caching here */
    CORNER *c = new CORNER;
    int index = HASH(i, j, 0);
    CORNERLIST *l = m_corners[index];
    c->i = i;
    c->x = m_start.x + ((double)i - .5) * sizex;
    c->j = j;
    c->y = m_start.y + ((double)j - .5) * sizey;
    for (; l != NULL; l = l->next) {
        if (l->i == i && l->j == j) {
            c->value = l->value;
            return c;
        }
    }
    l = new CORNERLIST;
    l->i = i;
    l->j = j;
    l->value = c->value = f->getSingleData(c->x, c->y);
    l->next = m_corners[index];
    m_corners[index] = l;
    return c;
}

/* setcenter: set (i,j,k) entry of table[]
* return 1 if already set; otherwise, set and return 0 */
int ImplicitCurve::setcenter(CENTERLIST *table[], int i, int j) {
    int index = HASH(i, j, 0);
    CENTERLIST *newc, *l, *q = table[index];
    for (l = q; l != NULL; l = l->next)
        if (l->i == i && l->j == j)
            return 1;
    newc = new CENTERLIST;
    newc->i = i;
    newc->j = j;
    newc->next = q;
    table[index] = newc;
    return 0;
}

/* find: search for point with value of given sign (0: neg, 1: pos) */
ImplicitCurve::TEST ImplicitCurve::find (int sign, double x, double y) {
    int i;
    TEST test;
    double drand48();
    double rangex = sizex;
    double rangey = sizey;
    test.ok = 1;
    for (i = 0; i < 10000; i++) {
        test.p.x = x + rangex * (RAND() - 0.5);
        test.p.y = y + rangey * (RAND() - 0.5);
        test.value = f->getSingleData(test.p.x, test.p.y);
        if (sign == (test.value > 0.0))
            return test;
        rangex *= 1.05; /* slowly expand search outwards */
        rangey *= 1.05;
    }
    test.ok = 0;
    return test;
}

/**** Tetrahedral Polygonization ****/
/* dotet: triangulate the tetrahedron
* b, c, d should appear clockwise when viewed from a
* return 0 if client aborts, 1 otherwise */
void ImplicitCurve::dotriangle (CUBE *cube, int c1, int c2, int c3) {
    CORNER *a = cube->corners[c1];
    CORNER *b = cube->corners[c2];
    CORNER *c = cube->corners[c3];
    int index = 0, apos, bpos, cpos, e1, e2, e3;
    if (apos = (a->value > 0.0)) index += 4;
    if (bpos = (b->value > 0.0)) index += 2;
    if (cpos = (c->value > 0.0)) index += 1;
    /* index is now 4-bit number representing one of the 16 possible cases */
    if (apos != bpos) e1 = vertid(a, b);
    if (apos != cpos) e2 = vertid(a, c);
    if (bpos != cpos) e3 = vertid(b, c);
    if (index > 3) index ^= 0x7;
    switch (index) {
    case 1:
        line(e2, e3);
        break;
    case 2:
        line(e1, e3);
        break;
    case 3:
        line(e1, e2);
        break;
    }
}

/* setedge: set vertex id for edge */
void ImplicitCurve::setedge (EDGELIST *table[], int i1, int j1, int i2, int j2, int vid) {
    unsigned int index;
    EDGELIST *newe;
    if (i1 > i2 || (i1 == i2 && j1 > j2)) {
        int t = i1;
        i1 = i2;
        i2 = t;
        t = j1;
        j1 = j2;
        j2 = t;
    }
    index = HASH(i1, j1, 0) + HASH(i2, j2, 0);
    newe = new EDGELIST;
    newe->i1 = i1;
    newe->j1 = j1;
    newe->i2 = i2;
    newe->j2 = j2;
    newe->vid = vid;
    newe->next = table[index];
    table[index] = newe;
}

/* getedge: return vertex id for edge; return -1 if not set */
int ImplicitCurve::getedge (EDGELIST *table[], int i1, int j1, int i2, int j2) {
    //
    EDGELIST *q;
    if (i1 > i2 || (i1 == i2 && j1 > j2)) {
        int t = i1;
        i1 = i2;
        i2 = t;
        t = j1;
        j1 = j2;
        j2 = t;
    }
    q = table[HASH(i1, j1, 0)+HASH(i2, j2, 0)];
    for (; q != NULL; q = q->next) {
        if (q->i1 == i1 && q->j1 == j1 &&
                q->i2 == i2 && q->j2 == j2)
            return q->vid;
    }
    return -1;
}

/**** Vertices ****/
/* vertid: return index for vertex on edge:
* c1->value and c2->value are presumed of different sign
* return saved index if any; else compute vertex and save */
int ImplicitCurve::vertid (CORNER *c1, CORNER *c2) {
    //
    Point2d a, b;
    int vid = getedge(m_edges, c1->i, c1->j, c2->i, c2->j);
    if (vid != -1) {
        //
        return vid; /* previously computed */
    }
    //
    a.x = c1->x;
    a.y = c1->y;
    b.x = c2->x;
    b.y = c2->y;
    VERTEX *v = new VERTEX;
    converge(&a, &b, c1->value, &(v->position)); /* position */
    addtovertices(v); /* save vertex */
    vid = m_vertices.size() - 1;
    setedge(m_edges, c1->i, c1->j, c2->i, c2->j, vid);
    return vid;
}

/* addtovertices: add v to sequence of vertices */
void ImplicitCurve::addtovertices (VERTEX *v) {
    //
    m_vertices.add(v);
}

/* converge: from two points of differing sign, converge to zero crossing */
void ImplicitCurve::converge (Point2d *p1, Point2d *p2, double v, Point2d *p) {
    int i = 0;
    Point2d pos, neg;
    if (v < 0) {
        pos.x = p2->x;
        pos.y = p2->y;
        neg.x = p1->x;
        neg.y = p1->y;
    } else {
        pos.x = p1->x;
        pos.y = p1->y;
        neg.x = p2->x;
        neg.y = p2->y;
    }
    while (1) {
        p->x = 0.5 * (pos.x + neg.x);
        p->y = 0.5 * (pos.y + neg.y);
        if (i++ == RES_CURVE) return;
        if ((f->getSingleData(p->x, p->y)) > 0.0) {
            pos.x = p->x;
            pos.y = p->y;
        } else {
            neg.x = p->x;
            neg.y = p->y;
        }
    }
}

void ImplicitCurve::line (int i1, int i2) {
    m_line.add(i1);
    m_line.add(i2);
}
