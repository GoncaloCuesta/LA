#include <stdlib.h>
#include <stdio.h>

#include "./../includes/map.h"
#include "./../includes/bfs.h"

#define INVALID -2
#define UNSEEN  -1


void bfs_rec (DMAP *ptr, int n, int x, int y) {
    
    if (ptr->mat[y][x] == INVALID) return;

    if ((ptr->mat[y][x] == UNSEEN) || (n < ptr->mat[y][x])) {
        ptr->mat[y][x] = n;

        bfs_rec(ptr, n+1, x, y+1);
        bfs_rec(ptr, n+1, x, y-1);
        bfs_rec(ptr, n+1, x+1, y);
        bfs_rec(ptr, n+1, x-1, y);
    }
}

DMAP *bfs (MAP *map, DMAP *ptr, int x, int y) {
    
    if (ptr == NULL) {
        ptr = malloc(sizeof(MAP));
        ptr->hei = map->hei;
        ptr->wid = map->wid;
        
        ptr->mat = malloc(sizeof(int*) * ptr->hei);
        for (int i = 0; i < ptr->hei; i++) {
            ptr->mat[i] = malloc(sizeof(int) * ptr->wid);
        }

        for (int i = 0; i < ptr->hei; i++) {
            for (int j = 0; j < ptr->wid; j++) {
                if (map->mat[i][j] == '#' || 
                    map->mat[i][j] == '.' ){
                    ptr->mat[i][j] = INVALID;
                } else {
                    ptr->mat[i][j] = UNSEEN;
                }
            }
        }

    }

    bfs_rec(ptr, 0, x, y);

    return ptr;
}

void print_dmap (DMAP *ptr) {
    for (int i = 0; i < ptr->hei; i++) {
        for (int j = 0; j < ptr->wid; j++) {
            if (ptr->mat[i][j] == INVALID)
                printf(". ");
            else if (ptr->mat[i][j] == UNSEEN)
                printf("? ");
            else 
                printf("%d ", ptr->mat[i][j]);
        }
        putchar('\n');
    }
}

void free_dmap (DMAP *ptr) {
    for (int i = 0; i < ptr->hei; i++) {
        free(ptr->mat[i]);
    }
    free(ptr->mat);
    free(ptr);
}
