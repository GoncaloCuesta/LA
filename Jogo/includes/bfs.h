#ifndef BFS_H
#define BFS_H

#include "./../includes/map.h"

#define INVALID -2
#define UNSEEN  -1

typedef struct _dmp {
    int **mat;
    int hei, wid;
} DMAP;


DMAP *bfs (MAP *map, DMAP *ptr, int x, int y);

void print_dmap (DMAP *ptr);

void free_dmap (DMAP *ptr);

#endif
