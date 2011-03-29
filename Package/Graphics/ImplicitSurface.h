#pragma once

#include "common.h"
#include "F3P.h"

#define RES 10 /* # converge iterations */
#define _L 0 /* left direction: -x, -i */
#define _R 1 /* right direction: +x, +i */
#define _B 2 /* bottom direction: -y, -j */
#define _T 3 /* top direction: +y, +j */
#define _N 4 /* near direction: -z, -k */
#define _F 5 /* far direction: +z, +k */
#define LBN 0 /* left bottom near corner */
#define LBF 1 /* left bottom far corner */
#define LTN 2 /* left top near corner */
#define LTF 3 /* left top far corner */
#define RBN 4 /* right bottom near corner */
#define RBF 5 /* right bottom far corner */
#define RTN 6 /* right top near corner */
#define RTF 7 /* right top far corner */
/* the LBN corner of cube (i, j, k), corresponds with location
* (start.x+(i-.5)*size, start.y+(j-.5)*size, start.z+(k-.5)*size) */
#define RAND() ((rand()&32767)/32767.) /* random number between 0 and 1 */
#define HASHBIT (5)
#define HASHSIZE (size_t)(1<<(3*HASHBIT)) /* hash table size (32768) */
#define MASK ((1<<HASHBIT)-1)
#define HASH(i,j,k) ((((((i)&MASK)<<HASHBIT)|((j)&MASK))<<HASHBIT)|((k)&MASK))
#define BIT(i, bit) (((i)>>(bit))&1)
#define FLIP(i,bit) ((i)^1<<(bit)) /* flip the given bit of i */

#define LB 0 /* left bottom edge */
#define LT 1 /* left top edge */
#define LN 2 /* left near edge */
#define LF 3 /* left far edge */
#define RB 4 /* right bottom edge */
#define RT 5 /* right top edge */
#define RN 6 /* right near edge */
#define RF 7 /* right far edge */
#define BN 8 /* bottom near edge */
#define BF 9 /* bottom far edge */
#define TN 10 /* top near edge */
#define TF 11 /* top far edge */

#define CUBE_NUM 9


class ImplicitSurface {

    typedef struct test { /* test the function for a signed value */
        Point3d p; /* location of test */
        double value; /* function value at p */
        int ok; /* if value is of correct sign */
    } TEST;
    typedef struct vertex { /* surface vertex */
        Point3d position, normal; /* position and surface normal */
    } VERTEX;
    typedef struct corner { /* corner of a cube */
        int i, j, k; /* (i, j, k) is index within lattice */
        double x, y, z, value; /* location and function value */
    } CORNER;
    typedef struct cube { /* partitioning cell (cube) */
        int i, j, k; /* lattice location of cube */
        CORNER *corners[8]; /* eight corners */
    } CUBE;
    typedef struct cubes { /* linked list of cubes acting as stack */
        CUBE cube; /* a single cube */
        struct cubes *next; /* remaining elements */
    } CUBES;
    typedef struct centerlist { /* list of cube locations */
        int i, j, k; /* cube location */
        struct centerlist *next; /* remaining elements */
    } CENTERLIST;
    typedef struct cornerlist { /* list of corners */
        int i, j, k; /* corner id */
        double value; /* corner value */
        struct cornerlist *next; /* remaining elements */
    } CORNERLIST;
    typedef struct edgelist { /* list of edges */
        int i1, j1, k1, i2, j2, k2; /* edge corner ids */
        int vid; /* vertex id */
        struct edgelist *next; /* remaining elements */
    } EDGELIST;
    typedef struct intlist { /* list of integers */
        int i; /* an integer */
        struct intlist *next; /* remaining elements */
    } INTLIST;
    typedef struct intlists { /* list of list of integers */
        INTLIST *list; /* a list of integers */
        struct intlists *next; /* remaining elements */
    } INTLISTS;

public:
    int m_bounds; /* cube range within lattice */
    Point3d m_start; /* start point on surface */
    CUBES *m_cubes; /* active cubes */
    ArrayList<VERTEX*> m_vertices; /* surface vertices */
    ArrayList<int> m_triangleindices;
    ArrayList<int> intlist;
    CENTERLIST **m_centers; /* cube center hash table */
    CORNERLIST **m_corners; /* corner value hash table */
    EDGELIST **m_edges; /* edge and vertex id hash table */
    F3P *f;
    double spx, spy, spz, sizex, sizey, sizez;

public:
    ImplicitSurface(F3P* f3p, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
    ~ImplicitSurface();

private:
    TEST find (int sign, double x, double y, double z);
    void converge (Point3d *p1, Point3d *p2, double v, Point3d *p);
    int setcenter(CENTERLIST *table[], int i, int j, int k);
    CORNER *setcorner (int i, int j, int k);
    void setedge (EDGELIST *table[], int i1, int j1, int k1, int i2, int j2, int k2, int vid);
    int getedge (EDGELIST *table[], int i1, int j1, int k1, int i2, int j2, int k2);
    void dotet (CUBE *cube, int c1, int c2, int c3, int c4);
    void testface (int i, int j, int k, CUBE *old, int face, int c1, int c2, int c3, int c4);
    int otherface (int edge, int face);
    int docube (CUBE *cube);
    int nextcwedge (int edge, int face);
    int vertid (CORNER *c1, CORNER *c2);
    void addtovertices (VERTEX *v);
    void triangle (int i1, int i2, int i3);
    void free_all();
};