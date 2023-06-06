#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>


#include "../includes/map.h"
#include "../includes/mobs.h"
#include "../includes/player.h"

void hit (MOBLIST *mobs, int y, int x, int dmg) {
    mvaddch(y, 2*x, '+');
    while(mobs) {
        if(mobs->mob->x == x && mobs->mob->y == y) {
            mobs->mob->hp -= dmg;
        }
        mobs = mobs->next;
    }
}

void atk1 (PLAYER *player, MOBLIST *mobs) {
    int dmg = player->atk * 1;

    for(int i = player->y - 1; i <= player->y + 1; i++) {
        for(int j = player->x - 1; j <= player->x + 1; j++) {
            hit(mobs, i, j, dmg);
        }
    }

    print_player(player);

    refresh();

    usleep(100000);
}

void atk2 (PLAYER *player, MOBLIST *mobs) {
    int dmg = player->atk * 2;

    int x = player->x;
    int y = player->y;

    if (player->dir == up) {
        hit(mobs, y  , x  , dmg);
        hit(mobs, y-1, x  , dmg);
        hit(mobs, y-2, x-1, dmg);
        hit(mobs, y-2, x  , dmg);
        hit(mobs, y-2, x+1, dmg);
    } else if (player->dir == down) {
        hit(mobs, y  , x  , dmg);
        hit(mobs, y+1, x  , dmg);
        hit(mobs, y+2, x-1, dmg);
        hit(mobs, y+2, x  , dmg);
        hit(mobs, y+2, x+1, dmg);
    } else if (player->dir == left) {
        hit(mobs, y  , x, dmg);
        hit(mobs, y  , x-1, dmg);
        hit(mobs, y-1, x-2, dmg);
        hit(mobs, y  , x-2, dmg);
        hit(mobs, y+1, x-2, dmg);
    } else if (player->dir == right) {
        hit(mobs, y  , x  , dmg);
        hit(mobs, y  , x+1, dmg);
        hit(mobs, y-1, x+2, dmg);
        hit(mobs, y  , x+2, dmg);
        hit(mobs, y+1, x+2, dmg);
    } 


    print_player(player);

    refresh();

    usleep(100000);
}
