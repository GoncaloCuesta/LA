#ifndef COMBAT_H
#define COMBAT_H

#include "./../includes/mobs.h"
#include "./../includes/player.h"

void hit (MOBLIST *mobs, int x, int y, int dmg);

void atk1 (PLAYER *player, MOBLIST *mobs);

void atk2 (PLAYER *player, MOBLIST *mobs);

#endif