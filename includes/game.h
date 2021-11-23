#ifndef MALLOCWORLD_GAME_H
#define MALLOCWORLD_GAME_H

#include <player.h>
#include <perlin.h>
#include <respawn.h>
#include <save.h>
#include <item.h>




void inGame(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn *respawnList, int nbMap);
int playerTurn(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn *respawnList);
void gameOver();
void winGame();

bool checkMoov(player *player, level *map, direction direction, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster);
int checkCaseIdType(int id, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster);
bool checkRecolte(player *player, resource *resource);

void fight(player *player, monster *monster, respawn **list, int32_t x, int32_t y, int8_t lvl);



// --------------------------------- AFFICHAGE ---------------------------------
void printStartMenu();
void tempPrintMap(level *map);
void printMapLineSeparator(int count);
void printPlayerInterface();
void printCredit();

#endif //MALLOCWORLD_GAME_H
