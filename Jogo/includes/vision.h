#ifndef VISION_H
#define VISION_H

#include "../includes/map.h"
#include "../includes/mobs.h"
#include "../includes/player.h"

void print_everything (MAP *map, PLAYER *player, MOBLIST *mobs);

void print_info(PLAYER *player, int x, int y);

#endif