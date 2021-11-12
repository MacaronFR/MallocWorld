
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint-gcc.h>

#include "inventory.h"
#include "player.h"


//---------------------- Creation et Destruction ----------------------
player* createPlayer() {
    player* player1 = malloc(sizeof(player));
    initPlayer(player1);
    return player1;
}

void initPlayer(player* player) {
    inventory* inventory1 = createInventory();
    initInventory(inventory1);
    player->inventory = inventory1;
    player->level = 1;
    player->life = 100;
    player->maxLife = 100;
    player->exp = 0;
}

void freePlayer(player* player) {
    freeInventory(player->inventory);
    free(player);
}

//---------------------- Test du player ----------------------
bool isDead(player* player) {
    return player->life <= 0;
}



//---------------------- Affichage ----------------------
void printPlayer(player* player) {

}




