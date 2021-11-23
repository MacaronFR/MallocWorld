#ifndef MALLOCWORLD_GAME_H
#define MALLOCWORLD_GAME_H

#include <player.h>
#include <perlin.h>
#include <respawn.h>




void inGame(player *player1, level *map, storage *storage, item **listItem, resource **listResource, monster **listMonster);
int playerTurn(player *player, level *map, storage *storage, item **listItem, resource **listResource, monster **listMonster);



void fight(player *player, monster *monster, respawn **list, int32_t x, int32_t y, int8_t lvl);



// --------------------------------- AFFICHAGE ---------------------------------
void printStartMenu();
void tempPrintMap(level *map);
void printMapLineSeparator(int count);
void printPlayerInterface();
void printCredit();

#endif //MALLOCWORLD_GAME_H
