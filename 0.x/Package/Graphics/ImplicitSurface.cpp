#include "ImplicitSurface.h"

/* edge: LB, LT, LN, LF, RB, RT, RN, RF, BN, BF, TN, TF */
//static int corner1[12] = {LBN,LTN,LBN,LBF,RBN,RTN,RBN,RBF,LBN,LBF,LTN,LTF};
//static int corner2[12] = {LBF,LTF,LTN,LTF,RBF,RTF,RTN,RTF,RBN,RBF,RTN,RTF};
static int leftface[12] = {_B, _L, _L, _F, _R, _T, _N, _R, _N, _B, _T, _F};
/* face on left when going corner1 to corner2 */
static int rightface[12] = {_L, _T, _N, _L, _B, _R, _R, _F, _B, _F, _N, _T};

ImplicitSurface::ImplicitSurface(F3P* f3p, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) {
    int n;
    TEST in, out;
    f = f3p;
    spx = xmax - xmin;
    spy = ymax - ymin;
    spz = zmax - zmin;
    m_start.x = (xmax + xmin) / 2;
    m_start.y = (ymax + ymin) / 2;
    m_start.z = (zmax + zmin) / 2;
    sizex = spx / CUBE_NUM;
    sizey = spy / CUBE_NUM;
    sizez = spz / CUBE_NUM;
    m_bounds = (CUBE_NUM - 1) / 2;
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
    /* find point on surface, beginning search at (x, y, z): */
    //不从曲面上的点开始搜索
// 	srand(1);
// 	in = find(1,(xmax+xmin)/2,(ymax+ymin)/2,(zmax+zmin)/2);
// 	out = find(0,(xmax+xmin)/2,(ymax+ymin)/2,(zmax+zmin)/2);
// 	if (!in.ok || !out.ok) {
// 		//error
//
// 		return;
// 	}
//
// 	converge(&in.p, &out.p, in.value, &m_start);
//
//
//
    /* push initial cube on stack: */
    m_cubes = new CUBES; /* list of 1 */
    m_cubes->cube.i = m_cubes->cube.j = m_cubes->cube.k = 0;
    m_cubes->next = NULL;
    /* set corners of initial cube: */
    //
    for (n = 0; n < 8; n++)
        m_cubes->cube.corners[n] = setcorner(BIT(n, 2), BIT(n, 1), BIT(n, 0));
    //
    setcenter(m_centers, 0, 0, 0);
    //
    while (m_cubes != NULL) { /* process active cubes till none left */
        CUBE c;
        CUBES *temp = m_cubes;
        c = m_cubes->cube;
        //
        dotet(&c, LBN, LTN, RBN, LBF);
        dotet(&c, RTN, LTN, LBF, RBN);
        dotet(&c, RTN, LTN, LTF, LBF);
        dotet(&c, RTN, RBN, LBF, RBF);
        dotet(&c, RTN, LBF, LTF, RBF);
        dotet(&c, RTN, LTF, RTF, RBF);
        /* pop current cube from stack */
        m_cubes = m_cubes->next;
        delete[] temp;
        /* test six face directions, maybe add to stack: */
        //
        testface(c.i - 1, c.j, c.k, &c, _L, LBN, LBF, LTN, LTF);
        testface(c.i + 1, c.j, c.k, &c, _R, RBN, RBF, RTN, RTF);
        testface(c.i, c.j - 1, c.k, &c, _B, LBN, LBF, RBN, RBF);
        testface(c.i, c.j + 1, c.k, &c, _T, LTN, LTF, RTN, RTF);
        testface(c.i, c.j, c.k - 1, &c, _N, LBN, LTN, RBN, RTN);
        testface(c.i, c.j, c.k + 1, &c, _F, LBF, LTF, RBF, RTF);
    }
}

ImplicitSurface::~ImplicitSurface() {
    free_all();
    for (unsigned int i = 0; i < m_vertices.size(); i++) {
        delete m_vertices[i];
    }
    m_vertices.clear();
    m_triangleindices.clear();
}

/* free_all: free allthe memory we’ve allocated (except cubetable) */
void ImplicitSurface::free_all () {
    int i, index;
    CORNERLIST *l, *lnext;
    CENTERLIST *cl, *clnext;
    EDGELIST *edge, *edgenext;
    INTLISTS *lists, *listsnext;
    INTLIST *ints, *intsnext;
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
void ImplicitSurface::testface (int i, int j, int k, CUBE *old, int face, int c1, int c2, int c3, int c4) {
    CUBE newc;
    CUBES *oldcubes = m_cubes;
    static int facebit[6] = {2, 2, 1, 1, 0, 0};
    int n, pos = old->corners[c1]->value > 0.0 ? 1 : 0, bit = facebit[face];
    /* test if no surface crossing, cube out of bounds, or already visited: */
    //这个优化不利于限定绘图范围
// 	if ((old->corners[c2]->value > 0) == pos &&
// 		(old->corners[c3]->value > 0) == pos &&
// 		(old->corners[c4]->value > 0) == pos)
// 		return;
    if (abs(i) > m_bounds || abs(j) > m_bounds || abs(k) > m_bounds)
        return;
    if (setcenter(m_centers, i, j, k))
        return;
    /* create new cube: */
    newc.i = i;
    newc.j = j;
    newc.k = k;
    for (n = 0; n < 8; n++) newc.corners[n] = NULL;
    newc.corners[FLIP(c1, bit)] = old->corners[c1];
    newc.corners[FLIP(c2, bit)] = old->corners[c2];
    newc.corners[FLIP(c3, bit)] = old->corners[c3];
    newc.corners[FLIP(c4, bit)] = old->corners[c4];
    for (n = 0; n < 8; n++)
        if (newc.corners[n] == NULL)
            newc.corners[n] = setcorner(i + BIT(n, 2), j + BIT(n, 1), k + BIT(n, 0));
    /*add cube to top of stack: */
    m_cubes = new CUBES;
    m_cubes->cube = newc;
    m_cubes->next = oldcubes;
}

/* setcorner: return corner with the given lattice location set (and cache) its function value */
ImplicitSurface::CORNER* ImplicitSurface::setcorner (int i, int j, int k) {
    /* for speed, do corner value caching here */
    CORNER *c = new CORNER;
    int index = HASH(i, j, k);
    CORNERLIST *l = m_corners[index];
    c->i = i;
    c->x = m_start.x + ((double)i - .5) * sizex;
    c->j = j;
    c->y = m_start.y + ((double)j - .5) * sizey;
    c->k = k;
    c->z = m_start.z + ((double)k - .5) * sizez;
    for (; l != NULL; l = l->next) {
        if (l->i == i && l->j == j && l->k == k) {
            c->value = l->value;
            return c;
        }
    }
    l = new CORNERLIST;
    l->i = i;
    l->j = j;
    l->k = k;
    l->value = c->value = f->getSingleData(c->x, c->y, c->z);
    l->next = m_corners[index];
    m_corners[index] = l;
    return c;
}

/* setcenter: set (i,j,k) entry of table[]
* return 1 if already set; otherwise, set and return 0 */
int ImplicitSurface::setcenter(CENTERLIST *table[], int i, int j, int k) {
    int index = HASH(i, j, k);
    CENTERLIST *newc, *l, *q = table[index];
    for (l = q; l != NULL; l = l->next)
        if (l->i == i && l->j == j && l->k == k)
            return 1;
    newc = new CENTERLIST;
    newc->i = i;
    newc->j = j;
    newc->k = k;
    newc->next = q;
    table[index] = newc;
    return 0;
}

/* find: search for point with value of given sign (0: neg, 1: pos) */
ImplicitSurface::TEST ImplicitSurface::find (int sign, double x, double y, double z) {
    int i;
    TEST test;
    double drand48();
    double rangex = sizex;
    double rangey = sizey;
    double rangez = sizez;
    test.ok = 1;
    for (i = 0; i < 10000; i++) {
        test.p.x = x + rangex * (RAND() - 0.5);
        test.p.y = y + rangey * (RAND() - 0.5);
        test.p.z = z + rangez * (RAND() - 0.5);
        test.value = f->getSingleData(test.p.x, test.p.y, test.p.z);
        if (sign == (test.value > 0.0))
            return test;
        rangex *= 1.05; /* slowly expand search outwards */
        rangey *= 1.05;
        rangez *= 1.05;
    }
    test.ok = 0;
    return test;
}

/**** Tetrahedral Polygonization ****/
/* dotet: triangulate the tetrahedron
* b, c, d should appear clockwise when viewed from a
* return 0 if client aborts, 1 otherwise */
void ImplicitSurface::dotet (CUBE *cube, int c1, int c2, int c3, int c4) {
    //
    CORNER *a = cube->corners[c1];
    CORNER *b = cube->corners[c2];
    CORNER *c = cube->corners[c3];
    CORNER *d = cube->corners[c4];
    int index = 0, apos, bpos, cpos, dpos, e1, e2, e3, e4, e5, e6;
    if (apos = (a->value > 0.0)) index += 8;
    if (bpos = (b->value > 0.0)) index += 4;
    if (cpos = (c->value > 0.0)) index += 2;
    if (dpos = (d->value > 0.0)) index += 1;
    /* index is now 4-bit number representing one of the 16 possible cases */
    if (apos != bpos) e1 = vertid(a, b);
    if (apos != cpos) e2 = vertid(a, c);
    if (apos != dpos) e3 = vertid(a, d);
    if (bpos != cpos) e4 = vertid(b, c);
    if (bpos != dpos) e5 = vertid(b, d);
    if (cpos != dpos) e6 = vertid(c, d);
    /* 14 productive tetrahedral cases (0000 and 1111 do not yield polygons */
    //
    switch (index) {
    case 1:
        triangle(e5, e6, e3);
        break;
    case 2:
        triangle(e2, e6, e4);
        break;
    case 3:
        triangle(e3, e5, e4);
        triangle(e3, e4, e2);
        break;
    case 4:
        triangle(e1, e4, e5);
        break;
    case 5:
        triangle(e3, e1, e4);
        triangle(e3, e4, e6);
        break;
    case 6:
        triangle(e1, e2, e6);
        triangle(e1, e6, e5);
        break;
    case 7:
        triangle(e1, e2, e3);
        break;
    case 8:
        triangle(e1, e3, e2);
        break;
    case 9:
        triangle(e1, e5, e6);
        triangle(e1, e6, e2);
        break;
    case 10:
        triangle(e1, e3, e6);
        triangle(e1, e6, e4);
        break;
    case 11:
        triangle(e1, e5, e4);
        break;
    case 12:
        triangle(e3, e2, e4);
        triangle(e3, e4, e5);
        break;
    case 13:
        triangle(e6, e2, e4);
        break;
    case 14:
        triangle(e5, e3, e6);
        break;
    }
}

/* otherface: return face adjoining edge that is not the given face */
int ImplicitSurface::otherface (int edge, int face) {
    int other = leftface[edge];
    return face == other ? rightface[edge] : other;
}

/* setcenter: set (i,j,k) entry of table[]
* return 1 if already set; otherwise, set and return 0 */
// int ImplicitSurface::setcenter(CENTERLIST *table[],int i,int j,int k)
// {
// 	int index = HASH(i, j, k);
// 	CENTERLIST *newc, *l, *q = table[index];
// 	for (l = q; l != NULL; l = l->next)
// 		if (l->i == i && l->j == j && l->k == k)
// 			return 1;
// 	newc = new CENTERLIST;
// 	newc->i = i; newc->j = j; newc->k = k; newc->next = q;
// 	table[index] = newc;
// 	return 0;
// }

/* setedge: set vertex id for edge */
void ImplicitSurface::setedge (EDGELIST *table[], int i1, int j1, int k1, int i2, int j2, int k2, int vid) {
    unsigned int index;
    EDGELIST *newe;
    if (i1 > i2 || (i1 == i2 && (j1 > j2 || (j1 == j2 && k1 > k2)))) {
        int t = i1;
        i1 = i2;
        i2 = t;
        t = j1;
        j1 = j2;
        j2 = t;
        t = k1;
        k1 = k2;
        k2 = t;
    }
    index = HASH(i1, j1, k1) + HASH(i2, j2, k2);
    newe = new EDGELIST;
    newe->i1 = i1;
    newe->j1 = j1;
    newe->k1 = k1;
    newe->i2 = i2;
    newe->j2 = j2;
    newe->k2 = k2;
    newe->vid = vid;
    newe->next = table[index];
    table[index] = newe;
}

/* getedge: return vertex id for edge; return -1 if not set */
int ImplicitSurface::getedge (EDGELIST *table[], int i1, int j1, int k1, int i2, int j2, int k2) {
    //
    EDGELIST *q;
    if (i1 > i2 || (i1 == i2 && (j1 > j2 || (j1 == j2 && k1 > k2)))) {
        int t = i1;
        i1 = i2;
        i2 = t;
        t = j1;
        j1 = j2;
        j2 = t;
        t = k1;
        k1 = k2;
        k2 = t;
    }
    //
    //
    //
    //
    q = table[HASH(i1, j1, k1)+HASH(i2, j2, k2)];
    for (; q != NULL; q = q->next) {
        if (q->i1 == i1 && q->j1 == j1 && q->k1 == k1 &&
                q->i2 == i2 && q->j2 == j2 && q->k2 == k2)
            return q->vid;
    }
    return -1;
}

/* nextcwedge: return next clockwise edge from given edge around given face */
int ImplicitSurface::nextcwedge (int edge, int face) {
    switch (edge) {
    case LB:
        return (face == _L) ? LF : BN;
    case LT:
        return (face == _L) ? LN : TF;
    case LN:
        return (face == _L) ? LB : TN;
    case LF:
        return (face == _L) ? LT : BF;
    case RB:
        return (face == _R) ? RN : BF;
    case RT:
        return (face == _R) ? RF : TN;
    case RN:
        return (face == _R) ? RT : BN;
    case RF:
        return (face == _R) ? RB : TF;
    case BN:
        return (face == _B) ? RB : LN;
    case BF:
        return (face == _B) ? LB : RF;
    case TN:
        return (face == _T) ? LT : RN;
    case TF:
        return (face == _T) ? RT : LF;
    }
}

/**** Vertices ****/
/* vertid: return index for vertex on edge:
* c1->value and c2->value are presumed of different sign
* return saved index if any; else compute vertex and save */
int ImplicitSurface::vertid (CORNER *c1, CORNER *c2) {
    //
    Point3d a, b;
    //
    //
    //
    int vid = getedge(m_edges, c1->i, c1->j, c1->k, c2->i, c2->j, c2->k);
    if (vid != -1) {
        //
        return vid; /* previously computed */
    }
    //
    a.x = c1->x;
    a.y = c1->y;
    a.z = c1->z;
    b.x = c2->x;
    b.y = c2->y;
    b.z = c2->z;
    VERTEX *v = new VERTEX;
    converge(&a, &b, c1->value, &(v->position)); /* position */
    addtovertices(v); /* save vertex */
    vid = m_vertices.size() - 1;
    setedge(m_edges, c1->i, c1->j, c1->k, c2->i, c2->j, c2->k, vid);
    return vid;
}

/* addtovertices: add v to sequence of vertices */
void ImplicitSurface::addtovertices (VERTEX *v) {
    //
    m_vertices.add(v);
}

/* converge: from two points of differing sign, converge to zero crossing */
void ImplicitSurface::converge (Point3d *p1, Point3d *p2, double v, Point3d *p) {
    int i = 0;
    Point3d pos, neg;
    if (v < 0) {
        pos.x = p2->x;
        pos.y = p2->y;
        pos.z = p2->z;
        neg.x = p1->x;
        neg.y = p1->y;
        neg.z = p1->z;
    } else {
        pos.x = p1->x;
        pos.y = p1->y;
        pos.z = p1->z;
        neg.x = p2->x;
        neg.y = p2->y;
        neg.z = p2->z;
    }
    while (1) {
        p->x = 0.5 * (pos.x + neg.x);
        p->y = 0.5 * (pos.y + neg.y);
        p->z = 0.5 * (pos.z + neg.z);
        if (i++ == RES) return;
        if ((f->getSingleData(p->x, p->y, p->z)) > 0.0) {
            pos.x = p->x;
            pos.y = p->y;
            pos.z = p->z;
        } else {
            neg.x = p->x;
            neg.y = p->y;
            neg.z = p->z;
        }
    }
}

void ImplicitSurface::triangle (int i1, int i2, int i3) {
    m_triangleindices.add(i1);
    m_triangleindices.add(i2);
    m_triangleindices.add(i3);
}