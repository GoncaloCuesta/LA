#ifndef MAP_H
#define MAP_H

typedef struct _map {
    char **mat;
    int hei, wid;
} MAP;

MAP *create_map (int height, int width, double factor);

void print_map (MAP *map);

void free_map (MAP *map);

#endif

