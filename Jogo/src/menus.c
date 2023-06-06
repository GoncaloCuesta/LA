#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "./../includes/player.h"
#include "./../includes/vision.h"

char init_screen(int x, int y) {

    clear();

    x*=2;

    int ly = (y/2) - (16/2);
    
    char *l1 = strdup("Roguelite");
    attron(A_BOLD);
    mvaddstr(ly, (x/2)-(strlen(l1)/2), l1);
    attroff(A_BOLD);
    free(l1);
    
    ly++;
    ly++;

    char *l2 = strdup("Navigate through the map until you find the portal to advance to the next stage,");
    mvaddstr(ly, (x/2)-(strlen(l2)/2), l2);
    free(l2);

    ly++;

    char *l3 = strdup("killing enemies along the way or getting killed in the process...");
    mvaddstr(ly, (x/2)-(strlen(l3)/2), l3);
    free(l3);
    ly++;

    ly++;
    ly++;
    
    char *l4 = strdup("Portal to next level: char \'*\'"); 
    char *l5 = strdup("Shop to buy upgrades: char \'0\'");
    mvaddstr(ly, (x/2)-(strlen(l4)/2), l4);
    ly++;
    mvaddstr(ly, (x/2)-(strlen(l5)/2), l5);
    ly++;

    free(l4);
    free(l5);

    ly++;
    ly++;
    
    char *l6 = strdup("Controls:"); 
    char *l7 = strdup("Use the numbers 2, 4, 6, 8 in the numpad to walk");
    char *l8 = strdup("Use the letter x to make an attack that deals damage around you"); 
    char *l9 = strdup("Use the letter z to make a stronger directional attack"); 
    mvaddstr(ly, (x/2)-(strlen(l8)/2), l6);
    ly++;
    mvaddstr(ly, (x/2)-(strlen(l8)/2), l7);
    ly++;
    mvaddstr(ly, (x/2)-(strlen(l8)/2), l8);
    ly++;
    mvaddstr(ly, (x/2)-(strlen(l8)/2), l9);
    ly++;

    free(l6);
    free(l7);
    free(l8);
    free(l9);
    
    ly++;
    ly++;

    char *l10 = strdup("Presse any key to continue");
    mvaddstr(ly, (x/2)-(strlen(l10)/2), l10);
    free(l10);
    
    refresh();

    char c = getch();
    return c;
}

void shop_menu_prints (PLAYER *p, int x, int y) {

    clear();

    int ly = (y/2) - (10/2);
        
    char *l1 = strdup("SHOP");
    attron(A_BOLD);
    mvaddstr(ly, (x/2)-(strlen(l1)/2), l1);
    attroff(A_BOLD);
    free(l1);
        
    ly++;
    ly++;

    char *l2 = strdup("option 1 | 10 $ | heal 10 health"); 
    char *l3 = strdup("option 2 | 20 $ | increase max health by 10 percent");
    char *l4 = strdup("option 3 | 25 $ | increase max attack by 10 percent"); 

    int len = strlen(l4);

    mvaddstr(ly, (x/2)-(len/2), l2);
    ly++;
    mvaddstr(ly, (x/2)-(len/2), l3);
    ly++;
    mvaddstr(ly, (x/2)-(len/2), l4);
    ly++;

    free(l2);
    free(l3);
    free(l4);

    ly++;
    ly++;

    char *l5 = strdup("q - quit");
    mvaddstr(ly, (x/2)-(len/2), l5);
    free(l5);

    print_info(p, x, y);

}

void shop_menu (PLAYER *p, int x, int y) {

    x*=2;

    char c = '\0';

    shop_menu_prints(p, x, y);

    while (c != 'q') {

        c = getch();

        switch (c) {
            case '1':
                if (p->money >= 10) {
                    p->money = p->money - 10;
                    p->health = p->health + 10;
                    if (p->health > p->max_health) {
                        p->health = p->max_health;
                    }
                }
                break;
            case '2':
                if (p->money >= 20) {
                    p->money = p->money - 20;
                    p->max_health = p->max_health * 1.10;
                }
                break;
            case '3':
                if (p->money >= 25) {
                    p->money = p->money - 25;
                    p->atk = p->atk * 1.10;
                }
                break;
            default:
                break;    
        }

        shop_menu_prints(p, x, y);

    }
}

void death_menu (int x, int y) {
    clear();

    x *= 2;

    int ly = (y/2) - (5/2);
        
    char *l1 = strdup("YOU'VE DIED");
    attron(A_BOLD);
    mvaddstr(ly, (x/2)-(strlen(l1)/2), l1);
    attroff(A_BOLD);
    free(l1);
        
    ly++;
    ly++;
    ly++;

    char *l2 = strdup("Press q to quit or any other key to start over");
    mvaddstr(ly, (x/2)-(strlen(l2)/2), l2);
    free(l2);
}