#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "sieve.h"

namespace {
uint const get_mask[8] = {1, 2, 4, 8, 16, 32, 64, 128};
uint const set_mask[8] = {254, 253, 251, 247, 239, 223, 191, 127};
uint const MODULO = 30;
uint const BITS_PER_INTERVAL = 8;
uint const multiplyQ[BITS_PER_INTERVAL][BITS_PER_INTERVAL] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 2, 3, 3, 4, 5, 6},
    {0, 2, 4, 4, 6, 6, 8, 10},
    {0, 3, 4, 5, 7, 8, 9, 12},
    {0, 3, 6, 7, 9, 10, 13, 16},
    {0, 4, 6, 8, 10, 12, 14, 18},
    {0, 5, 8, 9, 13, 14, 17, 22},
    {0, 6, 10, 12, 16, 18, 22, 28}
};
uint const multiplyR[BITS_PER_INTERVAL][BITS_PER_INTERVAL] = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {1, 5, 4, 0, 7, 3, 2, 6},
    {2, 4, 0, 6, 1, 7, 3, 5},
    {3, 0, 6, 5, 2, 1, 7, 4},
    {4, 7, 1, 2, 5, 6, 0, 3},
    {5, 3, 7, 1, 6, 0, 4, 2},
    {6, 2, 3, 7, 0, 4, 5, 1},
    {7, 6, 5, 4, 3, 2, 1, 0}
};
uint const table[BITS_PER_INTERVAL] = {1, 7, 11, 13, 17, 19, 23, 29};
int const map[MODULO] = { -1, 0, -1, -1, -1, -1, -1, 1, -1, -1, -1, 2, -1, 3, -1, -1, -1, 4, -1, 5, -1, -1, -1, 6, -1, -1, -1, -1, -1, 7};
typedef unsigned char uchar;
}
void sieve::init(uint N) {
    uint i, j, k, x, y;
    uint buf_size = N / MODULO + 1;
    uchar *buf = (uchar*)malloc(buf_size * sizeof(uchar)), *buf_end = buf + buf_size, *bufp = 0;
    for (i = 0; i < buf_size; ++i) buf[i] = 0xFF;
    buf[0] &= set_mask[0];
    uint *p = (uint*)malloc((BITS_PER_INTERVAL * buf_size + 3) * sizeof(uint)), *q = p;
    *q++ = 2;
    *q++ = 3;
    *q++ = 5;
    uint rN = (uint)sqrt((double)N) / MODULO + 1;
    for (i = 0, x = 0; i < rN; ++i, x += MODULO)
        for (j = 0; j < BITS_PER_INTERVAL; ++j) {
            if (buf[i] & get_mask[j]) {
                y = x + table[j];
                *q++ = y;
                for (k = 0; k < BITS_PER_INTERVAL; ++k) {
                    uint mask = set_mask[multiplyR[j][k]];
                    bufp = buf + (k < j ?
                                  i * (i + 1) * MODULO + i * table[k] + (i + 1) * table[j] + multiplyQ[j][k] :
                                  i * i * MODULO + i * table[k] + i * table[j] + multiplyQ[j][k]);
                    while (bufp < buf_end) {
                        *bufp &= mask;
                        bufp += y;
                    }
                }
            }
        }
    for (; i < buf_size; ++i, x += MODULO)
        for (j = 0; j < BITS_PER_INTERVAL; ++j) {
            if (buf[i] & get_mask[j])
                *q++ = x + table[j];
        }
    v.assign(p, q);
    free(p);
    free(buf);
}