#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>


#include "../includes/map.h"
#include "../includes/mobs.h"
#include "../includes/menus.h"
#include "../includes/vision.h"
#include "../includes/player.h"
#include "../includes/combat.h"



void input (char c, MAP *map, PLAYER *player, MOBLIST *mobs) {
    if(c == '8' || c == '6' || c == '4' || c == '2') {
        move_character(map, player, mobs, c);
    } else if(c == 'x') {
        atk1(player, mobs);
    } else if(c == 'z') {
        atk2(player, mobs);
    } 
}

char newLevel (MAP **map, PLAYER **player, MOBLIST **mobs, int *mob_attempts) {

    int height = (*map)->hei;
    int width = (*map)->wid;

    clear();

    int ly = (*map)->hei/2;
    char *l1 = strdup("Entering new world");
    mvaddstr(ly, 2*(width/2)-(strlen(l1)/2), l1);
    free(l1);

    refresh();

    PLAYER *new_player = create_player (width/2, height/2, (*player)->max_health, (*player)->health, (*player)->atk, (*player)->money);     
    free_player(*player);
    *player = new_player;

    free_map(*map);
    *map = create_map (height, width, 0.4);

    free_mobs(*mobs);
    *mob_attempts *= 1.3;
    *mobs = init_mobs (*map, *mob_attempts);

    char *cont = strdup("Presse any key to continue");
    mvaddstr(ly+2, 2*(width/2)-(strlen(cont)/2), cont);
    refresh();
    free(cont);

    char c = getch();
    return c;
}

int main () {

    srand(time(NULL));

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    int height, width;
    getmaxyx(stdscr, height, width);
    height--;
    width/=2;

    char c = init_screen(width, height);
    if (c == 'q') return 0;

    MAP *map = create_map(height, width, 0.4);

    PLAYER *player = create_player(width/2, height/2, 100, 100, 10, 0);

    int nm = 25;

    int mob_attempts = (map->hei * map->wid) / (nm * nm);
    MOBLIST *mobs = init_mobs(map, mob_attempts);

    print_everything(map, player, mobs);  

    c = '\0';

    while(c != 'q') {
        c = getch();
        input(c, map, player, mobs);
        
        int dmg = 0;
        update_mobs(&mobs, map, player->x, player->y, &(player->money), &dmg);
        player->health -= dmg;

        if (map->mat[player->y][player->x] == '*') {
            c = newLevel(&map, &player, &mobs, &mob_attempts);
        }

        if (map->mat[player->y][player->x] == '0') {
            shop_menu(player, width, height);
        }

        if (player->health <= 0) {
            death_menu (width, height);
            c = getch();
            
            if (c != 'q') {    
                free_map(map);
                map = create_map(height, width, 0.4);

                free_player(player);
                player = create_player(width/2, height/2, 100, 100, 10, 0);

                free_mobs(mobs);
                mob_attempts = (map->hei * map->wid) / (nm * nm);
                mobs = init_mobs(map, mob_attempts);
            }
        }

        print_everything (map, player, mobs); 

    }

    free_player(player);
    free_mobs(mobs);
    free_map(map);

    endwin();

    return 0;
}