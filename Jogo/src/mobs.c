#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#include "./../includes/map.h"
#include "./../includes/bfs.h"
#include "./../includes/player.h"
#include "./../includes/mobs.h" 

MOB* create_mob (MOB_TYPE type, int x, int y) {
    MOB* mob = malloc(sizeof(MOB));

    mob->type = type;
    mob->x = x;
    mob->y = y;

    if (mob->type == creeper) {
        mob->hp = 15;
        mob->dmg = 50;
        mob->speed = 0.3;
        mob->cooldown = 0;
    } else if (mob->type == skeleton) {
        mob->hp = 35;
        mob->dmg = 20;
        mob->speed = 0.2;
        mob->cooldown = 0;
    } else if (mob->type == zombie) {
        mob->hp = 25;
        mob->dmg = 25;
        mob->speed = 0.4;
        mob->cooldown = 0;
    }

    return mob;
}

int money_drop (MOB_TYPE type) {

    if (type == creeper) {
        return 5;
    } else if (type == skeleton) {
        return 15;
    } else if (type == zombie) {
        return 10;
    }

    return 0;
}

MOBLIST* init_mobs (MAP *map, int attempts) {
    MOBLIST* list = NULL;

    int attempts_creeper = attempts * 1.5;

    int attempts_skeleton = attempts * 2.0;

    int attempts_zombie = attempts * 2.5;
    
    for(int i = 0; i < attempts_creeper; i++) {
        int x = 1 + rand() % (map->wid - 2);
        int y = 1 + rand() % (map->hei - 2);

        if (map->mat[y][x] == ' ') {
            MOBLIST* nodo = malloc(sizeof(MOBLIST));
            nodo->mob = create_mob(creeper, x, y);
            nodo->next = list;
            list = nodo;
        }
    }

    for(int i = 0; i < attempts_skeleton; i++) {
        int x = 1 + rand() % (map->wid - 2);
        int y = 1 + rand() % (map->hei - 2);

        if (map->mat[y][x] == ' ') {
            MOBLIST* nodo = malloc(sizeof(MOBLIST));
            nodo->mob = create_mob(skeleton, x, y);
            nodo->next = list;
            list = nodo;
        }
    }
    
    for(int i = 0; i < attempts_zombie; i++) {
        int x = 1 + rand() % (map->wid - 2);
        int y = 1 + rand() % (map->hei - 2);

        if (map->mat[y][x] == ' ') {
            MOBLIST* nodo = malloc(sizeof(MOBLIST));
            nodo->mob = create_mob(zombie, x, y);
            nodo->next = list;
            list = nodo;
        }
    }

    return list;
}

void print_mobs (MOBLIST *l) {

    while (l) {
        char c;
        if (l->mob->type == creeper      ) c = 'c';
        else if (l->mob->type == skeleton) c = 's';
        else if (l->mob->type == zombie  ) c = 'z';
        
        mvaddch(l->mob->y, 2*l->mob->x, c);

        l = l->next;
    }
}

int colides_mob (MOBLIST *l, int x, int y) {
    while (l) {
        if ((l->mob->x == x) && (l->mob->y == y)) { return 1; }
        l = l->next;
    }
    return 0;
}

void move_mob (MOBLIST *l, MOB *mob, MAP *map, DMAP *dmap) {
    int least = dmap->mat[mob->y][mob->x];

    if ((least > dmap->mat[mob->y][mob->x+1]) && 
        (dmap->mat[mob->y][mob->x+1] != INVALID)) 
        least = dmap->mat[mob->y][mob->x+1];

    if ((least > dmap->mat[mob->y][mob->x-1]) && 
        (dmap->mat[mob->y][mob->x-1] != INVALID)) 
        least = dmap->mat[mob->y][mob->x-1];

    if ((least > dmap->mat[mob->y+1][mob->x]) && 
        (dmap->mat[mob->y+1][mob->x] != INVALID)) 
        least = dmap->mat[mob->y+1][mob->x];

    if ((least > dmap->mat[mob->y-1][mob->x]) && 
        (dmap->mat[mob->y-1][mob->x] != INVALID)) 
        least = dmap->mat[mob->y-1][mob->x];
    

    if ( map->mat[mob->y][mob->x+1] == ' ' && 
        !colides_mob(l, mob->x+1, mob->y) &&
        (least == dmap->mat[mob->y][mob->x+1])) { 
        mob->x++; 
    } else if ( map->mat[mob->y][mob->x-1] == ' ' && 
        !colides_mob(l, mob->x-1, mob->y) &&
        (least == dmap->mat[mob->y][mob->x-1])) { 
        mob->x--; 
    } else if ( map->mat[mob->y+1][mob->x] == ' ' && 
        !colides_mob(l, mob->x, mob->y+1) &&
        (least == dmap->mat[mob->y+1][mob->x])) { 
        mob->y++; 
    } else if ( map->mat[mob->y-1][mob->x] == ' ' && 
        !colides_mob(l, mob->x, mob->y-1) &&
        (least == dmap->mat[mob->y-1][mob->x])) { 
        mob->y--; 
    }

}

void move_mobs (MOBLIST *l, MAP *map, int plx, int ply) {

    DMAP *dmap = bfs(map, NULL, plx, ply);

    while (l) {

        if (rand() < RAND_MAX * l->mob->speed) {

            int mx = l->mob->x;
            int my = l->mob->y;
                
            int dx = abs(mx-plx);
            int dy = abs(my-ply);

            if (l->mob->type == creeper) {

                int out_radius = 5;
                int in_radius = 1;

                int out_range = (dx*dx) + (dy*dy) <= (out_radius*out_radius);
                int in_range = (dx*dx) + (dy*dy) <= (in_radius*in_radius);

                if (!in_range && out_range) {
                    move_mob(l, l->mob, map, dmap);
                }
            } else if (l->mob->type == zombie) {

                int out_radius = 5;
                int in_radius = 1;

                int out_range = (dx*dx) + (dy*dy) <= (out_radius*out_radius);
                int in_range = (dx*dx) + (dy*dy) <= (in_radius*in_radius);

                if (!in_range && out_range) {
                    move_mob(l, l->mob, map, dmap);
                }
            } else if (l->mob->type == skeleton) {

                int out_radius = 4;

                int out_range = (dx*dx) + (dy*dy) <= (out_radius*out_radius);

                if (out_range) {
                    move_mob(l, l->mob, map, dmap);
                }
            } 
        }

        l = l->next;
    }

    free_dmap(dmap);
}

int range (int x1, int y1, int x2, int y2, int r) {
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);

    int f = (dx*dx) + (dy*dy) <= (r*r);

    return f;
}

void attack_mobs (MOBLIST *l, MAP *map, int plx, int ply, int *dmg) {

    (void) map;

    while (l) {

        if (l->mob->type == creeper) {
            
            if (range(l->mob->x, l->mob->y, plx, ply, 2)) {
                if (l->mob->cooldown >= 3) {
                    l->mob->cooldown = 0;
                    *dmg += l->mob->dmg;
                    l->mob->hp = 0;
                } else {
                    l->mob->cooldown++;
                }
            }

        } else if (l->mob->type == zombie) {

            if (range(l->mob->x, l->mob->y, plx, ply, 1)) {
                if (l->mob->cooldown >= 2) {
                    l->mob->cooldown = 0;
                    *dmg += l->mob->dmg;
                } else {
                    l->mob->cooldown++;
                }
            }

        } else if (l->mob->type == skeleton) {

            if (range(l->mob->x, l->mob->y, plx, ply, 4)) {
                if (l->mob->cooldown >= 5) {
                    l->mob->cooldown = 0;
                    *dmg += l->mob->dmg;
                } else {
                    l->mob->cooldown++;
                }
            }

        } 

        l = l->next;
    }

}

MOBLIST *remove_dead (MOBLIST *l, int *c) {
  
    if (l == NULL) return NULL;
  
    if (l->mob->hp <= 0) {
        MOBLIST *tmp = remove_dead (l->next, c);

        (*c) += money_drop (l->mob->type);

        free(l->mob);
        free(l);

        return tmp;
    }

    l->next = remove_dead (l->next, c);
    return l;
}

void update_mobs (MOBLIST **l, MAP *map, int plx, int ply, int *money, int *dmg) {

    attack_mobs(*l, map, plx, ply, dmg);

    *l = remove_dead(*l, money);

    move_mobs(*l, map, plx, ply);

}

void free_mobs (MOBLIST *l) {
    MOBLIST *prev = l;

    while (l) {
        prev = l;
        l = l->next;
        free(prev->mob);
        free(prev);
    }
}