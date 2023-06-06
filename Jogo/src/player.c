#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#include "./../includes/map.h"
#include "./../includes/player.h"
#include "./../includes/mobs.h"


PLAYER *create_player (int x, int y, int max_health, int health, int atk, int money) {
    PLAYER *p = malloc(sizeof(PLAYER));
    p->x = x;
    p->y = y;
    p->max_health = max_health;
    p->health = health;
    p->atk = atk;
    p->money = money;
    p->dir = up;
    return p;
}

void free_player (PLAYER *p) {
    free(p);
}

void move_character(MAP *map, PLAYER *player, MOBLIST *l, char in) {

    int x = player->x;
    int y = player->y;

    if (in == '8') {
        player->dir = up;
        if ((map->mat[y - 1][x] != '#') &&
            !colides_mob(l, x, y-1)) {
            player->y--;
        }
        return;
    } 
    if (in == '2') {
        player->dir = down;
        if ((map->mat[y + 1][x] != '#') &&
            !colides_mob(l, x, y+1)) {
            player->y++;
        }
        return;
    } 
    if (in == '4') {
        player->dir = left;
        if ((map->mat[y][x - 1] != '#') &&
            !colides_mob(l, x-1, y)) {
            player->x--;
        }
        return;
    } 
    if (in == '6') {
        player->dir = right;
        if ((map->mat[y][x + 1] != '#') &&
            !colides_mob(l, x+1, y)) {
            player->x++;
        }
        return;
    }
}

void print_player (PLAYER *player) {
    mvaddch(player->y, 2*player->x, '@');
}