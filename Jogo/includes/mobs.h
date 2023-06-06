#ifndef MOBS_H
#define MOBS_H

#include "./../includes/map.h"

typedef enum _mob_type {
    creeper,
    skeleton,
    zombie
} MOB_TYPE;

typedef struct _mob {
    MOB_TYPE type;
    int hp;
    int dmg;
    int x, y;
    double speed;
    int cooldown;
} MOB;

typedef struct _mobList {
    MOB* mob;
    struct _mobList *next; 
} MOBLIST;

int colides_mob (MOBLIST *l, int x, int y);

MOBLIST* init_mobs (MAP *map, int attempts);

void update_mobs (MOBLIST **l, MAP *map, int x, int y, int *money, int *dmg);

void print_mobs (MOBLIST *l);

void free_mobs (MOBLIST *l);

#endif