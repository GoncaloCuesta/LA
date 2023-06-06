#ifndef PLAYER_H
#define PLAYER_H

#include "./../includes/map.h"
#include "./../includes/mobs.h"

typedef enum d {
    up,
    down,
    left,
    right
} DIR;

typedef struct _pla {
    int x, y;
    int max_health, health;
    int atk;
    int money;
    DIR dir;
} PLAYER;

PLAYER *create_player (int x, int y, int max_health, int health, int atk, int money);

void free_player (PLAYER *p);

void move_character(MAP *map, PLAYER *player, MOBLIST *l, char in);

void print_player (PLAYER *player);

#endif