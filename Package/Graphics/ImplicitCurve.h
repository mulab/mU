#pragma once

#include "common.h"
#include "F2P.h"

#define RES_CURVE 30 /* # converge iterations */
#define _L 0 /* left direction: -x, -i */
#define _R 1 /* right direction: +x, +i */
#define _B 2 /* bottom direction: -y, -j */
#define _T 3 /* top direction: +y, +j */
#define LB 0 /* left bottom corner */
#define LT 1 /* left top corner */
#define RB_CURVE 2 /* right bottom corner */
#define RT_CURVE 3 /* right top corner */

#define RAND() ((rand()&32767)/32767.) /* random number between 0 and 1 */
#define HASHBIT (5)
#define HASHSIZE (size_t)(1<<(3*HASHBIT)) /* hash table size (32768) */
#define MASK ((1<<HASHBIT)-1)
#define HASH(i,j,k) ((((((i)&MASK)<<HASHBIT)|((j)&MASK))<<HASHBIT)|((k)&MASK))
#define BIT(i, bit) (((i)>>(bit))&1)
#define FLIP(i,bit) ((i)^1<<(bit)) /* flip the given bit of i */

#define LE 0 /* left edge */
#define RE 1 /* right edge */
#define BE 2 /* bottom edge */
#define TE 3 /* top edge */

#define CUBE_NUM_CURVE 29




class ImplicitCurve {

    typedef struct test { /* test the function for a signed value */
        Point2d p; /* location of test */
        double value; /* function value at p */
        int ok; /* if value is of correct sign */
    } TEST;
    typedef struct vertex { /* surface vertex */
        Point2d position; /* position and surface normal */
    } VERTEX;
    typedef struct corner { /* corner of a cube */
        int i, j; /* (i, j, k) is index within lattice */
        double x, y, value; /* location and function value */
    } CORNER;
    typedef struct cube { /* partitioning cell (cube) */
        int i, j; /* lattice location of cube */
        CORNER *corners[4]; /* eight corners */
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
        int i, j; /* corner id */
        double value; /* corner value */
        struct cornerlist *next; /* remaining elements */
    } CORNERLIST;
    typedef struct edgelist { /* list of edges */
        int i1, j1, i2, j2; /* edge corner ids */
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
    Point2d m_start; /* start point on surface */
    CUBES *m_cubes; /* active cubes */
    ArrayList<VERTEX*> m_vertices; /* surface vertices */
    ArrayList<int> m_line;
    ArrayList<int> intlist;
    CENTERLIST **m_centers; /* cube center hash table */
    CORNERLIST **m_corners; /* corner value hash table */
    EDGELIST **m_edges; /* edge and vertex id hash table */
    F2P *f;
    double spx, spy, sizex, sizey;

public:
    ImplicitCurve(F2P* f2p, double xmin, double xmax, double ymin, double ymax);
    ~ImplicitCurve();

private:
    TEST find (int sign, double x, double y);
    void converge (Point2d *p1, Point2d *p2, double v, Point2d *p);
    int setcenter(CENTERLIST *table[], int i, int j);
    CORNER *setcorner (int i, int j);
    void setedge (EDGELIST *table[], int i1, int j1, int i2, int j2, int vid);
    int getedge (EDGELIST *table[], int i1, int j1, int i2, int j2);
    void testface (int i, int j, CUBE *old, int face, int c1, int c2);
    void dotriangle (CUBE *cube, int c1, int c2, int c3);
    int nextcwedge (int edge, int face);
    int vertid (CORNER *c1, CORNER *c2);
    void addtovertices (VERTEX *v);
    void line (int i1, int i2);
    void free_all();
};
