#ifndef MALLOCWORLD_PLAYER_H
#define MALLOCWORLD_PLAYER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <inventory.h>

typedef struct s_player {
    inventory *inventory;
    uint16_t life;
    uint16_t maxLife;
    uint16_t exp;
    uint8_t level;
}player;

bool isDead(player* player);
void usePotion(player* player, int id);
void winExp(player* player, uint16_t exp);
void levelUp(player* player);




#endif //MALLOCWORLD_PLAYER_H
