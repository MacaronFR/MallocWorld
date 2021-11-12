#ifndef MALLOCWORLD_PLAYER_H
#define MALLOCWORLD_PLAYER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <inventory.h>
#include <item.h>
#include <stdint.h>

typedef struct s_player {
    inventory *inventory;
    uint16_t life;
    uint16_t maxLife;
    uint16_t exp;
    uint8_t level;
}player;

player* createPlayer();
void initPlayer(player* player);
void freePlayer(player* player);

bool isDead(player* player);
void takeDamage(player* player, int damage);
void usePotion(player* player, int id);
void winExp(player* player, uint16_t exp);
void levelUp(player* player);

item* getArmor(player* player, int id);
item* getWeapon(player* player, int id);
item* getRessource(player* player, int id, int nb);

//---------------------- Affichage ----------------------
void printPlayer(player* player);
void printExp(player* player);
void printLife(player* player);



#endif //MALLOCWORLD_PLAYER_H
