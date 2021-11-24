#ifndef MALLOCWORLD_GAME_H
#define MALLOCWORLD_GAME_H

#include <player.h>
#include <perlin.h>
#include <respawn.h>
#include <save.h>
#include <item.h>




void inGame(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn *respawnList, int nbMap);
int playerTurn(player *player, level *map, storage *storage, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn *respawnList);

bool move(player *player, level *map, direction direction, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn *respawnList);
bool tryMove(player *player, level *map, direction direction, item **listItem, size_t nItem, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster, respawn *respawnList, int x, int y);
int checkCaseIdType(int id, resource **listResource, size_t nResource, monster **listMonster, size_t nMonster);
item *checkRecolte(player *player, resource *resource);

int fight(player *player, monster *monster, respawn **list, int32_t x, int32_t y, int8_t lvl);
void gameOver();
void winGame();


// --------------------------------- AFFICHAGE ---------------------------------
void printStartMenu();
void tempPrintMap(level *map);
void printMapLineSeparator(int count);
void printPlayerInterface();
void printCredit();

#endif //MALLOCWORLD_GAME_H
