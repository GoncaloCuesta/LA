#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>

#include "../includes/map.h"
#include "../includes/bfs.h"
#include "../includes/mobs.h"
#include "../includes/player.h"

int plotLineLow(int x0, int y0, int x1, int y1, MAP *map) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int D = (2 * dy) - dx;
    int y = y0;

    for (int x = x0; x <= x1; x++) {

        if (map->mat[y][x] == '#') {
            return 1;
        }

        if (D > 0) {
            y = y + yi;
            D = D + (2 * (dy - dx));
        } else {
            D = D + 2*dy;
        }
    }

    return 0;
}

int plotLineHigh(int x0, int y0, int x1, int y1, MAP *map) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int D = (2 * dx) - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++) {
        
        if (map->mat[y][x] == '#') {
            return 1;
        }
        
        if (D > 0) {
            x = x + xi;
            D = D + (2 * (dx - dy));
        } else {
            D = D + 2*dx;
        }
    }

    return 0;
}

int plotLine(int x0, int y0, int x1, int y1, MAP *map) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            return plotLineLow(x1, y1, x0, y0, map);
        } else {
            return plotLineLow(x0, y0, x1, y1, map);
        }
    } else {
        if (y0 > y1) {
            return plotLineHigh(x1, y1, x0, y0, map);
        } else {
            return plotLineHigh(x0, y0, x1, y1, map);
        }
    }

    return 0;
}

void print_dark (MAP *map, PLAYER *player) {

    for (int i = 0; i < map->hei; i++) {
        for (int j = 0; j < map->wid; j++) {

            if (plotLine(j, i, player->x, player->y, map)) {
                mvaddch(i, 2*j, '#');
            }

        }
    }
    
}

void print_info(PLAYER *player, int x, int y) {
    
    char *health = malloc(sizeof(char) * 30);
    snprintf(health, 30-1, "health: %d / %d", player->health, player->max_health);
    mvaddstr(y, 0, health);
    
    char *money = malloc(sizeof(char) * 30);
    snprintf(money, 30-1, "money: %d ", player->money);
    mvaddstr(y, x-strlen(money)+1, money);

    refresh();
    
    free(health);
    free(money);
}

void print_everything (MAP *map, PLAYER *player, MOBLIST *mobs) {

    clear();

    print_map(map);
    print_mobs(mobs);
    print_dark(map, player);
    print_player(player);
    print_info(player, map->wid, map->hei);

    refresh();
}
