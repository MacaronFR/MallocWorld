#ifndef MALLOCWORLD_GAME_H
#define MALLOCWORLD_GAME_H

#include <player.h>
#include <perlin.h>
#include <respawn.h>
#include <save.h>
#include <item.h>
#include <ctype.h>
#include <time.h>
#include <craft.h>


level *createGame(int portal[4][2], player *p1, item **listItem, size_t nItem, int *lv);
void inGame(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn **respawnList, int nbMap);
int playerTurn(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn **respawnList);

int tryMove(player *player, level *map, direction direction,storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn **respawnList, int x, int y);
int checkCaseIdType(int id, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster);
int tryRecolte(player *player, item **listItem, size_t nItem, resource **listResource, size_t nResource, respawn **listRespawn, int id,  int x, int y);
int fight(player *player, monster *monster, respawn **listRespawn, int32_t x, int32_t y);
void interactWithPNJ(player *player, storage *storage, item **listItem, size_t nItem);
void goToStorage(player *player, storage *storage);
void goToCrafting(player *player, item **listCraftableItem);

void gameOver();
void winGame();


// --------------------------------- AFFICHAGE ---------------------------------
void printStartMenu();
void tempPrintMap(level *map);
void printMapLineSeparator(int count);
void printInterfacePlayer();
void printInterfacePNJ();
void printInterfaceStorage(storage *storage);
void printInterfaceCrafting(item **listCraftableItem);
void printStorage(storage *storage);
void printCredit();

#endif //MALLOCWORLD_GAME_H
