#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>

#include "./../includes/map.h"
#include "./../includes/bfs.h"

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

void alloc_map (MAP *map) {
    
    map->mat = malloc(sizeof(char*) * map->hei);
    for (int i = 0; i < map->hei; i++) {
        map->mat[i] = malloc(sizeof(char) * map->wid);
    }

}

void init_map (MAP *map) {

    for (int i = 0; i < map->hei; i++) {
        for (int j = 0; j < map->wid; j++) {
            map->mat[i][j] = '#';
        }
    }

}

int border1 (MAP *map, int num) {
    int count = 0;
    for (int i = 0; i < map->hei; i++) {
        if (map->mat[i][1] == ' ') count++;
        if (map->mat[i][map->wid-2] == ' ') count++;
    }
    
    for (int i = 0; i < map->wid; i++) {
        if (map->mat[1][i] == ' ') count++;
        if (map->mat[map->hei-2][i] == ' ') count++;
    }

    if (count < num) 
        return 0;
    return 1;
}

void drunk (MAP *map, double factor) {

    if (factor > 1) factor = 1;

    int tiles = map->wid * map->hei;
    int desired_floor = tiles * factor;

    int retry = 1;
    while(retry) {

        init_map(map);

        int wx = map->wid / 2;
        int wy = map->hei / 2;

        int floor_counter = 1;

        map->mat[wy][wx] = ' ';

        while (floor_counter < desired_floor) {
            int dir = rand() % 4; 

            switch(dir) {
                case UP:
                    if (wy - 1 > 0) {
                        wy--; 
                        if (map->mat[wy][wx] == '#') {
                            map->mat[wy][wx] = ' ';
                            floor_counter++;
                        }
                    }
                    break;
                case DOWN:
                    if (wy + 1 < map->hei - 1) {
                        wy++; 
                        if (map->mat[wy][wx] == '#') {
                            map->mat[wy][wx] = ' ';
                            floor_counter++;
                        }
                    }
                    break;
                case LEFT:
                    if (wx - 1 > 0) {
                        wx--; 
                        if (map->mat[wy][wx] == '#') {
                            map->mat[wy][wx] = ' ';
                            floor_counter++;
                        }
                    }
                    break;
                case RIGHT:
                    if (wx + 1 < map->wid - 1) {
                        wx++; 
                        if (map->mat[wy][wx] == '#') {
                            map->mat[wy][wx] = ' ';
                            floor_counter++;
                        }
                    }
                    break;
            }
        }
    
        retry = border1(map, (map->hei+map->wid) / 4 );

    }

}

void spawn (MAP *map) {
    int sp_w = 3;
    int sp_h = 3;

    int x = (map->wid/2) - (sp_w/2);
    int y = (map->hei/2) - (sp_h/2);

    for (int i = y; i < y+sp_h; i++) {
        for (int j = x; j < x + sp_w; j++) {
            map->mat[i][j] = '_';
        }
    }

}

int shop_cond (MAP *map, int x, int y, int sh_w, int sh_h) {

    if (y > map->hei-sh_h) return 0;
    if (x > map->wid-sh_w) return 0;

    int cx = x + sh_w/2;
    int cy = y + sh_h/2;

    double min_dist = 0.5;

    int dy1 = abs(map->hei - cy);
    int dx1 = abs(map->wid - cx);
    int r1 = min_dist * (map->hei + map->wid) / 2;

    if ((dy1*dy1) + (dx1*dx1) > (r1*r1)) return 0;

    int count = 0;

    for (int i = y; i < y+sh_h; i++) {
        for (int j = x; j < x+sh_w; j++) {
            if (map->mat[i][j] == '#') {
                count++;
            } else if (map->mat[i][j] != ' ') {
                return 0;
            }
        }
    }

    int total = sh_h * sh_w;

    if ((count > total * 0.85) &&
        (count < total * 0.95)) {    
        return 1;
    }

    return 0;
}

void shop_draw (MAP *map, int x, int y, int sh_w, int sh_h) {
    for (int i = y; i < y+sh_h; i++) {
        for (int j = x; j < x+sh_w; j++) {
            map->mat[i][j] = '.';
        }
    }
    map->mat[y+(sh_h/2)][x+(sh_w/2)] = '0';
}

int shop (MAP *map, int attempts) {
    int shop_wid = 5;
    int shop_hei = 5;

    for (int i = 0; i < attempts; i++) {
        int x = 1 + rand() % (map->wid - shop_wid - 1);
        int y = 1 + rand() % (map->hei - shop_hei - 1);

        if (shop_cond(map, x, y, shop_wid, shop_hei)) {
            shop_draw(map, x, y, shop_wid, shop_hei);
            return 1;
        }

    }

    return 0;
}

void fill (MAP *map, int x, int y, int n) {

    if (n < 0) return;
    if (map->mat[y][x] != ' ') return;

    map->mat[y][x] = '*';

    fill(map, x+1, y, n-1);
    fill(map, x-1, y, n-1);
    fill(map, x, y+1, n-1);
    fill(map, x, y-1, n-1);

}

int portal (MAP *map) {
    
    DMAP *ptr = bfs(map, NULL, map->wid/2, map->hei/2);

    int max = 0;
    int x = 0;
    int y = 0;

    for (int i = 0; i < ptr->hei; i++) {
        for (int j = 0; j < ptr->wid; j++) {
            if (ptr->mat[i][j] >= 0) {
                if (ptr->mat[i][j] > max) {
                    max = ptr->mat[i][j];
                    y = i;
                    x = j;
                }
            }
        }
    }

    fill(map, x, y, 5);

    free_dmap(ptr);

    return 0;
}

MAP *create_map (int height, int width, double factor) {
    
    if (height < 2) height = 2;
    if (width  < 2) width  = 2;

    MAP *map = malloc(sizeof(MAP));

    map->hei = height;
    map->wid = width;

    alloc_map(map);
    init_map(map);
    drunk(map, factor);
    spawn(map);
    shop(map, 100);
    portal(map);

    return map;
}

void print_map (MAP *map) {

    for (int i = 0; i < map->hei; i++) {
        for (int j = 0; j < map->wid; j++) {
            mvaddch(i, 2*j, map->mat[i][j]);
        }
    }
}

void free_map (MAP *map) {

    for (int i = 0; i < map->hei; i++) {
        free(map->mat[i]);
    }

    free(map->mat);

    free(map);
}
